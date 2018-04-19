#include <Stepper.h>
#include <Wire.h>
#include <FastLED.h>

//Debug flag
bool debug = true;
bool calibTest = true;
bool cal = true;

/******** Stepper ***********/
/******** IO ***********/
#define MOTOR_ENABLE 8
/******** X ***********/
#define X_MOTOR_STEP 2
#define X_MOTOR_DIR 5
#define X_HOME_SWITCH A2
#define X_OUTER_SWITCH A3

/******** Y ***********/
#define Y_MOTOR_STEP 3
#define Y_MOTOR_DIR 6
#define Y_HOME_SWITCH A1
#define Y_OUTER_SWITCH A0

/******** MAGNET ***********/
#define MAGNET_PIN 10
#define MAGNET_ENDSTOP A6

/******** DIRECTIONS ***********/
#define DEC_DIR -1
#define INC_DIR 1

/******** STEPPER SPESIFICATIONS ***********/
#define STEP_PER_REVOLUTION 400
#define DRIVER_MICROSTEPS 0.25
#define BELT_PRESENT_PITCH 3 //mm 
#define X_PULLEY_TOOTH 20
#define Y_PULLEY_TOOTH 20


/******** I2C Communication ***********/
#define ANSWERSIZE 5
#define ADDRESS 0x03

/************** Serial Communication *************/
#define DEVICE_NAME "dev1"

/******** LED ***********/
#define LED_PIN     11
#define NUM_LEDS    42 // 42 on the robot
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;

short ledRGB[] = {0, 0, 0};

//Default stepper speed
int const stepSpeed = 150;
//Travelled distance
int travelledDistance;

//Structure to hold position
typedef struct Pos {
  short x;
  short y;
} Pos;

Pos currentPos;
Pos maxPos;
Pos newPos;

//Limit switch booleans
boolean xHomeSwitch = false;
boolean xAwaySwitch = false;
boolean yHomeSwitch = false;
boolean yAwaySwitch = false;

//Interrupt debounce
static unsigned long last_interrupt_time = 0;
unsigned long interrupt_time = 0;


//Set steppers
Stepper motorX(STEP_PER_REVOLUTION, X_MOTOR_STEP, X_MOTOR_DIR);
Stepper motorY(STEP_PER_REVOLUTION, Y_MOTOR_STEP, Y_MOTOR_DIR);

//-------------------------------ENUMS------------------------------
//ENUMS to keep track of what has been sent to the arduino, and what it should do.
//The state of the arduino
enum state {
  busy = 0x50,
  readyToRecieve = 0x51,
  stopped = 0x52,
  emc = 0x60,
  fail = 0x61,
  linearBotLimitTrigged = 0x64,
  parameter = 0x70
};

/**
 * *COMMANDS* the Master can send to the slave.

   IF AN COMMAND IS ADDED I NEEDS TO BE ADDED TO CommandCheck.
*/
enum command {
  //Main loop states
  idle = 0x01,
  EMC = 0x02,
  failure = 0x03,

  // Genral Cmd
  moveRobot = 0x05,
  doCalibrate = 0x10,
  turnOnLight = 0x11,
  changeColor = 0x12,
  changeVelocity = 0x20,
  changeAcceleration = 0x21,
  magnetControl = 0x22,

  // Request command
  retrieveState = 0x30,
  calibParam = 0x31
};

//ENUM For acknowledgement  og negative-acknowledgement
enum acknowlagement {ACK = 1, NACK = 0};
//ENUM for color on leds
enum RGBEnum {R = 0, G = 1, B = 2};

//Create enums
enum state inState;             //To keep track of which STATE the arduino currently is in. Busy when doing stuff, ready/idle when not. EMC, encoder failure, ETC...
enum command mainCommand;       //Switch case command for the main loop
enum command recieveCommand;    //Switch case command for the RECIEVE command, reading storing values to be done later.
enum acknowlagement acknowlagement;
//enum command checkCommand;

void setup() {

  Serial.begin(19200);
  while (!Serial) {
    ;// wait for serial port to connect. Needed for native USB port only
  }


  if (debug)
  {
    //Set default positions
    currentPos.x = 0;
    currentPos.y = 0;
    maxPos.x = 5000;
    maxPos.y = 3000;
    newPos.y = 0;
    newPos.x = 0;
  }

  // IO setup.
  pinMode(MOTOR_ENABLE, OUTPUT);
  /******** X ***********/
  pinMode(X_MOTOR_STEP, OUTPUT);
  pinMode(X_MOTOR_DIR, OUTPUT);
  pinMode(X_HOME_SWITCH, INPUT_PULLUP);
  pinMode(X_OUTER_SWITCH, INPUT_PULLUP);
  /******** Y ***********/
  pinMode(Y_MOTOR_STEP, OUTPUT);
  pinMode(Y_MOTOR_DIR, OUTPUT);
  pinMode(Y_HOME_SWITCH, INPUT_PULLUP);
  pinMode(Y_OUTER_SWITCH, INPUT_PULLUP);
  /******** MAGNET ***********/
  pinMode(MAGNET_PIN, OUTPUT);
  pinMode(MAGNET_ENDSTOP, INPUT);

  //Enables signal
  pinMode(MOTOR_ENABLE, OUTPUT);

  /******** LED ***********/
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );

  //Initialize the safety interrupts
  //When the limit switches are reached(pressed)

  //Calculate step per mm
  float XstepPerMM = stepsToMMCalculation(STEP_PER_REVOLUTION, DRIVER_MICROSTEPS, BELT_PRESENT_PITCH, X_PULLEY_TOOTH);
  float YstepPerMM = stepsToMMCalculation(STEP_PER_REVOLUTION, DRIVER_MICROSTEPS, BELT_PRESENT_PITCH, Y_PULLEY_TOOTH);

  /******** X ***********/
  // Wire.begin(ADDRESS);                // join i2c bus with address
  // //Add the interrupts
  // Wire.onReceive(receiveEvent); // register event
  // Wire.onRequest(requestEvent);
  if (debug) {
    Serial.println(F("Setup complete"));
  }
}

//-------------------------------------------------------------------------------------------------------



void loop()
{
  if (debug)
  {
    if (cal)
    {
      // setLedColor(0, 10, 10);
      Serial.println("Go to Idle");
      mainCommand = idle;
      cal = false;
    }

  }

  //Switch command for the main loop
  //--------------------------------------------------------------------------------------------------
  switch (mainCommand)
  {

    // Idle state.
    case idle:
      inState = readyToRecieve;
      if (debug) {
        delay(1000);
        Serial.println(F("Main: Idle"));
      }
      receiveSerialEvent();
      //Enable motors
      disableMotors();
      //Set state to idle

      break;


    //---------------------------------------
    case moveRobot:
      if (debug) {
        Serial.print(F("Main: Move Robot"));
      }
      //Set state to busy
      inState = busy; // When in an opperation state is busy.
      //Do a check to see if robot needs to move
      //Make movestep check endstop in between every step, and then return true if moving was succesfull without hitting endstops

      //Check if robot needs to move
      if (!robotInPosition())
      {
        //Move the robot in desired steps. IF this is false, then it was not possible to move
        if (!moveStep(newPos.x, newPos.y))
        {
          mainCommand = failure;
        }
      }
      else
      {
        if (debug) {
          Serial.print("CurrentPos pos X");
          Serial.println(currentPos.x);
          Serial.print("CurrentPos pos Y");
          Serial.println(currentPos.y);
        }
        mainCommand = idle;
      }
      break;





    //Calibrate the robot
    case doCalibrate:
      if (debug) {
        Serial.println(F("Main: Do Calibration "));
      }
      inState = busy; // When in an opperation state is busy.
      // Do the calibration
      calibration();
      /** TODO: MOVE ROBOT TO CENTER OF AREA **/
      //      // Move to senter of area
      //      newPos.x = (maxPos.x / 2);
      //      newPos.y = (maxPos.y / 2);
      //      mainCommand = moveRobot;


      mainCommand = idle;
      break;


    //Emergency stop
    case EMC:
      if (debug) {
        Serial.println(F("Main: EMC"));
      }
      //Set state to in EMC
      inState = emc;
      break;

    //Failure
    case failure:
      if (debug) {
        Serial.println(F("Main: Failure"));
      }
      //Enable motors
      disableMotors();
      inState = checkFailure();
      //check which failure has been triggered
      //if(limitSwitch())
      break;

    //-------------------------------------------------------------------------
    //Set the command for calibrating the robot.
    case changeColor:
      inState = busy; // When in an opperation state is busy.
      Serial.println(F("Main: Change colors on led"));
      setLedColor(ledRGB[R], ledRGB[G], ledRGB[B]);
      mainCommand = idle;
      break;
    //-----------------

    default:
      Serial.println(F("In default"));
      break;
      //--------------------------------------------------------------------------------------------------
  }

}



