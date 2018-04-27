#include <Stepper.h>
#include <Wire.h>
#include <FastLED.h>

//Debug flag
bool debug = false;
bool calibTest = false;
bool cal = false;

/******** Stepper ***********/
/******** IO ***********/
#define MOTOR_ENABLE 8
/******** X ***********/
#define X_MOTOR_STEP 4
#define X_MOTOR_DIR 7
#define X_HOME_SWITCH A2
#define X_OUTER_SWITCH A3

/******** Y ***********/
#define Y_MOTOR_STEP 3
#define Y_MOTOR_DIR 6
#define Y_HOME_SWITCH A1
#define Y_OUTER_SWITCH A0

/******** MAGNET ***********/
#define MAGNET_PIN 9
#define MAGNET_ENDSTOP A6

/******** DIRECTIONS ***********/
#define DEC_DIR -1
#define INC_DIR 1

/******** STEPPER SPESIFICATIONS ***********/
#define DRIVER_MICROSTEPS_FACTOR  4 // Factor
#define STEP_PER_REVOLUTION 200
#define BELT_PRESENT_PITCH 2//mm 
#define X_PULLEY_TOOTH 20
#define Y_PULLEY_TOOTH 16

//Calculate step per mm
#define X_STEP_PER_MM ((STEP_PER_REVOLUTION*DRIVER_MICROSTEPS_FACTOR) / (BELT_PRESENT_PITCH * X_PULLEY_TOOTH))
#define Y_STEP_PER_MM ((STEP_PER_REVOLUTION*DRIVER_MICROSTEPS_FACTOR) / (BELT_PRESENT_PITCH * Y_PULLEY_TOOTH))


/******** STOP INTERRUPT PIN ***********/
#define INTERRUPT_PIN 2

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
int stepSpeed[] = {120 , 120};

//Travelled distance
int travelledDistance;

//Structure to hold position
typedef struct Pos {
  int x;
  int y;
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
Stepper motorX(STEP_PER_REVOLUTION * DRIVER_MICROSTEPS_FACTOR, X_MOTOR_STEP, X_MOTOR_DIR);
Stepper motorY(STEP_PER_REVOLUTION * DRIVER_MICROSTEPS_FACTOR, Y_MOTOR_STEP, Y_MOTOR_DIR);

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
 * *COMMANDS  * the Master can send to the slave.

   IF AN COMMAND IS ADDED I NEEDS TO BE ADDED TO CommandCheck.
*/
enum command {
  //Main loop states
  idle = 0x01,
  failure = 0x03,

  // Genral Cmd
  moveRobotTo = 0x05,
  moveRobot = 0x06,
  stopRobot = 0x07,
  doCalibrate = 0x10,
  turnOnLight = 0x11,
  changeColor = 0x12,
  discoLight = 0x13,
  findTray  = 0x14,
  changeVelocity = 0x20,
  changeAcceleration = 0x21,
  magnetOn = 0x22,
  magnetOff = 0x23,

  // Request command
  retrieveState = 0x30,
  calibParam = 0x31
};

//ENUM For acknowledgement  og negative-acknowledgement
const byte ACK = 1;
const byte NACK = 0;
//enum acknowlagement {ACK = 1, NACK = 0};
//ENUM for color on leds
enum RGBEnum {R = 0, G = 1, B = 2};
//ENUM for axiz
enum axisEnum {X_AXIS = 0, Y_AXIS = 1};

//Create enums
enum state inState;             //To keep track of which STATE the arduino currently is in. Busy when doing stuff, ready/idle when not. EMC, encoder failure, ETC...
enum command mainCommand = idle;       //Switch case command for the main loop
enum command recieveCommand;    //Switch case command for the RECIEVE command, reading storing values to be done later.
//enum command checkCommand;

void setup() {

  Serial.begin(19200);

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
  pinMode(13, OUTPUT);
  /******** ENABLE SIGNAL ***********/
  pinMode(MOTOR_ENABLE, OUTPUT);


  /******** INTERRUPT PIN ***********/
  //Initialize the safety interrupts
  //When the limit switches are reached(pressed)
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), stopMotors, LOW);

  /******** LED ***********/
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );


  if (debug) {
    Serial.println(F("Setup complete"));
  }

  mainCommand = idle;
}
//-------------------------------------------------------------------------------------------------------


void loop()
{
  receiveSerialEvent();
  //Switch command for the main loop
  //--------------------------------------------------------------------------------------------------
  switch (mainCommand)
  {

    // Idle state.
    case idle:
      if (debug) {
        Serial.println(F("Main: Idle"));
      }
      //
      updateStatusAndNotify(readyToRecieve);
      //Enable motors
      disableMotors();
      //Set state to idle
      break;


    //---------------------------------------
    case moveRobotTo:

      //Set state to busy
      //Make movestep check endstop in between every step, and then return true if moving was succesfull without hitting endstops
      // Update Status and notify
      updateStatusAndNotify(busy);
      //Check if robot needs to move of if the stop interrupt has been trigged.
      if (!robotInPosition()) // Returns true if the robot is in position
      {

        //Move the robot in desired steps. IF this is false, then it was not possible to move
        if (!moveStep(newPos.x, newPos.y))
        {
          mainCommand = failure;
        }
      } else {
        mainCommand = idle;
      }
      break;



    //Calibrate the robot
    case doCalibrate:
      if (debug) {
        Serial.println(F("Main: Do Calibration "));
      }
      // Update Status and notify
      updateStatusAndNotify(busy)
      ;
      // Do the calibration
      calibration();
      if (calibTest) {
        Serial.println("Reached outer limit");
        Serial.print("Max pos X");
        Serial.println(maxPos.x);
        Serial.print("Max pos Y");
        Serial.println(maxPos.y);
      }
      /** TODO: MOVE ROBOT TO CENTER OF AREA **/
      //      // Move to senter of area
      newPos.x = (maxPos.x / 2);
      newPos.y = (maxPos.y / 2);
      mainCommand = moveRobotTo;
      break;

    //Turns magnet ON
    case magnetOn:
      if (debug) {
        Serial.println(F("Main: magnetOn"));
      }

      // Update Status and notify
      updateStatusAndNotify(busy);
      magnetON();
      mainCommand = idle;
      break;



    //Turns magnet OFF
    case magnetOff:
      if (debug) {
        Serial.println(F("Main: magnetOff"));
      }

      // Update Status and notify
      updateStatusAndNotify(busy);

      magnetOFF();
      mainCommand = idle;
      break;


    //Turns magnet OFF
    case findTray:
      if (debug) {
        Serial.println(F("Main: find tray "));
      }

      // Update Status and notify
      updateStatusAndNotify(busy);

      if (!moveToTray()) {
        mainCommand = failure;
      } else {
        mainCommand = idle;
      }
      break;


    // stop
    case stopRobot:
      if (debug) {
        Serial.println(F("Main: stopRobot"));
      }
      //Set state to in EMC
      disableMotors();
      // Update Status and notify
      updateStatusAndNotify(stopped);
      //Set state to in EMC
      disableMotors();
      break;

    //Failure
    case failure:
      if (debug) {
        Serial.println(F("Main: Failure"));
      }
      //Enable motors
      disableMotors();
      updateStatusAndNotify(checkFailure());
      // Sends state update
      sendInt(inState);
      mainCommand = idle;
      break;

    //-------------------------------------------------------------------------
    //Set the command for calibrating the robot.
    case changeColor:
      if (debug) {
        Serial.println(F("Main: Change colors on led"));
      }
      // Update Status and notify
      updateStatusAndNotify(busy);
      setLedColor(ledRGB[R], ledRGB[G], ledRGB[B]);
      mainCommand = idle;
      break;
    //-----------------

    //-------------------------------------------------------------------------
    //Set the command for calibrating the robot.
    case discoLight:
      if (debug) {
        Serial.println(F("Main: Disco mode"));
      }
      // Update Status and notify
      updateStatusAndNotify(busy);
      ledDiscoShow();
      break;
    //-----------------



    default:
      if (debug) {
        Serial.println(F("In default"));
      }
      break;
      //--------------------------------------------------------------------------------------------------
  }

}



