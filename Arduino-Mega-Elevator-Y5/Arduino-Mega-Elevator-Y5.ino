#include <Stepper.h>
#include <Wire.h>


//debugging bools
bool cal = false;
// Debug over 1 for normal debug,
// Debug = 1, calibration debug
// Debug = 2, move case debug
// Debug = 3 for steps
int debug = false;


/******************** STEPPER VARIABLES ***********************/
#define MOTOR_BREAK 7
#define PUMP 6
//Left motor
#define LEFT_MOTOR_STEP 13
#define LEFT_MOTOR_DIR 12
#define LEFT_MOTOR_ENABLE 11
#define LEFT_BOTTOM_SWITCH 28
#define LEFT_TOP_SWITCH 26
#define LEFT_DIR_UP 1
#define LEFT_DIR_DOWN -1
//Right motor
#define RIGHT_MOTOR_STEP 10
#define RIGHT_MOTOR_DIR 9
#define RIGHT_MOTOR_ENABLE 8
#define RIGHT_BOTTOM_SWITCH 36
#define RIGHT_TOP_SWITCH 35
#define RIGHT_DIR_UP -1
#define RIGHT_DIR_DOWN 1

/******** STOP INTERRUPT PIN ***********/
#define STOP_INTERRUPT_PIN 2

//Safety IR
#define TOP_IR_RELAY 18
#define BOTTOM_IR_RELAY 19

/******** STEPPER SPESIFICATIONS ***********/
#define STEP_PER_REVOLUTION 200 // Stepps per rev on the motor (FULL STEP) 
#define DRIVER_MICROSTEPS_FACTOR 4 // Micro stepp factor.
#define PULLET_RADIUS 12.16
#define PULLER_CIRCUMFERENCE 2*PULLET_RADIUS*PI
#define BELT_PRESENT_PITCH 5 //mm 
#define PULLEY_TOOTH 16 // Number of tooths on pulley. 

//Variable to hold steps pr mm                         /* EASY FIX!! TODO: FIND OUT WHY THE MOTOR NEEDS TO RUN 4 TIME MORE STEPS. (Maybe the lib?) */
#define STEP_PER_MM ((STEP_PER_REVOLUTION * DRIVER_MICROSTEPS_FACTOR * 4) / (PULLER_CIRCUMFERENCE))


// Falgs size in Z diraction measured in mm
#define FLAG_SIZE 25 //mm 
//Default steps recognized as flags
const int flagDetectionSteps = FLAG_SIZE * STEP_PER_MM;
// 5% of the falag size to be used for safy barriere
const int safetyFactorOffset = flagDetectionSteps * 0.05;


/************** I2C Communication *************/
#define ANSWERSIZE 3
#define ADDRESS 0x03
/************** Serial Communication *************/
#define DEVICE_NAME "dev2"

/************** The maximun number of trays posible for this system *************/
#define MAX_NR_OF_TRAYS 10



//Default stepper speed
int stepSpeed = 200;
//Save traveled distance here
int travelledDistance;


int barrierCounter = 0;
int barrierCounterTop = 0;
int barrierCounterBottom = 0;



/********************** STRUCTURES **************************/

//Structure to hold positions
typedef struct Pos {
  int zLeft;
  int zRight;
} Pos;


/**
  Struct to represent tray
*/
typedef struct Tray
{
  Pos zPos;
  int trayNumber;
} Tray;

/**
   Struct to hold all the trays
*/
typedef struct TrayRegister
{
  int nrOfTrays;
  Tray trayList[MAX_NR_OF_TRAYS];
} TrayRegister;

//Create tray register
TrayRegister trayReg;


//The position variables
Pos currentPos;
Pos maxPos;
Pos newPos;
Pos lastFlagPos;


/************ SWITCH variables ************/
//Limit switch booleans
boolean leftBottomSwitch = false;
boolean leftTopSwitch = false;
boolean rightBottomSwitch = false;
boolean rightTopSwitch = false;
//Safety barrier
boolean topSafetyBarrier = false;
boolean bottomSafetyBarrier = false;
//Bool to keep track if the brake is on or not
boolean brake = false;


//Set up the motors
Stepper motor_left(STEP_PER_REVOLUTION * DRIVER_MICROSTEPS_FACTOR, LEFT_MOTOR_STEP, LEFT_MOTOR_DIR);
Stepper motor_right(STEP_PER_REVOLUTION * DRIVER_MICROSTEPS_FACTOR, RIGHT_MOTOR_STEP, RIGHT_MOTOR_DIR);


//-------------------------------ENUMS----------------------------------------------
//ENUMS to keep track of what has been sent to the arduino, and what it should do.
//The state of the arduino
enum state {
  busy = 0x50,
  readyToRecieve = 0x51,
  emc = 0x60,
  fail = 0x61,
  encoderOutOfSync = 0x65,
  linearBotLimitTrigged = 0x64,
  stopped = 0x52,
  safetySwitchLower = 0x61,
  safetySwitchUpper = 0x62,
  parameter = 0x70
};

/**
 * *COMMANDS* the Master can send to the slave.
*/
enum command {
  moveRobot = 0x05,
  // doSuction = 0x06,
  stopRobot = 0x07,
  doCalibrate = 0x10,
  changeVelocity = 0x20,
  changeAcceleration = 0x21,
  vacuumOn = 0x24,
  vacuumOff = 0x25,
  retrieveState = 0x30,
  calibParam = 0x31,

  //Main loop states
  idle = 0x01,
  EMC = 0x02,
  failure = 0x03
};


/**
   The enum for deciding the moving conditions
*/
enum moveState
{
  normalMove = 0,
  safetyMoving = 1,
  endStopMoving = 2,
  moveNotPossible = 3,
  doneMoving = 4
};


//ENUM For acknowledgement  og negative-acknowledgement
enum acknowlagement {ACK = 1, NACK = 0};


//Create enums
enum state inState;             //To keep track of which STATE the arduino currently is in. Busy when doing stuff, ready/idle when not. EMC, encoder failure, ETC...
enum command mainCommand;       //Switch case command for the main loop
enum command recieveCommand;    //Switch case command for the RECIEVE command, reading storing values to be done later.
enum moveState movingState;
//enum command checkCommand;



int lastPrintStep;






//-----------------------------SETUP---------------------------------------------------------------------

void setup() {

  // Start Serial communication
  Serial.begin(19200);

  if (debug > 0)
  {
    currentPos.zLeft = 0;
    currentPos.zRight = 0;
    //    maxPos.zRight =  MMToSteps(2000);
    //    maxPos.zLeft =  MMToSteps(2000);
    //    currentPos.zLeft = MMToSteps(200);
    //    currentPos.zRight = MMToSteps(200);
    //    createTrayOnPos();
    //    currentPos.zLeft = MMToSteps(400);
    //    currentPos.zRight = MMToSteps(400);
    //    createTrayOnPos();
    //    currentPos.zLeft = MMToSteps(700);
    //    currentPos.zRight = MMToSteps(700);
    //    createTrayOnPos();
    Serial.println(stepsToMM(currentPos.zLeft));
  }
  if (!debug) {
    //Setup the tray register
    trayReg.nrOfTrays = 0;
  }

  //Set the pin modes
  pinMode(LEFT_BOTTOM_SWITCH, INPUT);
  pinMode(LEFT_TOP_SWITCH, INPUT);
  pinMode(RIGHT_BOTTOM_SWITCH, INPUT);
  pinMode(RIGHT_TOP_SWITCH, INPUT);
  pinMode(MOTOR_BREAK, OUTPUT);

  pinMode(TOP_IR_RELAY, INPUT_PULLUP);
  pinMode(BOTTOM_IR_RELAY, INPUT_PULLUP);


  /******** INTERRUPT PIN ***********/
  pinMode(STOP_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(STOP_INTERRUPT_PIN), stopMotors, FALLING);

  turnOnBrake();
  mainCommand = idle;

}




//--------------------------------------------------------------------------------------------------
void loop()
{
  if (cal)
  {
    //
    turnOffBrake();
    cal = false;
  }
  // Recives an event for serial com
  receiveSerialEvent();



  //Switch command for the
  //--------------------------------------------------------------------------------------------------
  switch (mainCommand)
  {
    //--------------------------------------------------------------------------------------------------
    //The arduino has nothing to do, and is therefore in idle
    case idle:
      if (debug) {
        Serial.println(F("Main: Idle"));
      }
      inState = readyToRecieve;
      turnOnBrakeDisableMotors();
      if (debug) {
        delay(1000);
      }
      break;


    case moveRobot:
      if (debug) {
        Serial.println(F("Main: move robot"));

      }
      //Set state to busy
      inState = busy;

      //If endstop is reached check if it can continue to move
      //Check which endstop is reached, and which direction it can move
      //If it cant move in the preferred direction(set to by move)
      //Switch state to endstopReached, and bypass the moveRobot command
      //Do the moving


      //Switch case for controlling the moving
      switch (movingState)
      {
        case normalMove:
          if ((!safetyBarrier()) && !limitTrigged())
          {
            //Check if should continue moving, also moves while checking if
            //Do normal moving
            if (moveStep(newPos))
            {
              //Moving normally so reset the barrier counter
              updateCntBottomBarrier();
              updateCntTopBarrier();
            }
            //Done moving
            else
            {
              movingState = doneMoving;
            }

          }
          //One of the end stops are triggered
          else if (limitTrigged())
          {
            movingState = endStopMoving;
          }
          //The safety barrier is trigged
          else if (safetyBarrier())
          {
            movingState = safetyMoving;
          }
          break;

        //Moving while the safety barrier is trigged
        /*Cases that occur while moving when safety is trigged:
          1. End-stop sensors can be triggered, direction has to be checked before moving then
          2. Can go outside of pre-determined safety steps -> Send to failure/not possible to move
          3. While inside of pre-determined safety steps -> check if safety barrier is still active, it its not. Then the "obstacle" was a flag, and save the z positions to a new tray /-> Continues to move if needed.
        */
        case safetyMoving:
          //Maybe add check of which sensor is hit - should be added when
          //Safety barrier is hit

          //Check if endstop is hit and is direction safe!
          if (limitTrigged() && !isDirectionSafe(newPos))
          {
            if (debug == 2)
            {
              Serial.println("Direction not safe");
            }

            movingState = moveNotPossible;
          }

          //Check if the safety margins are still OK
          else if (counterExceeded())
          {
            //Check if safetybarrier is still activated, if its not, it means the robot is barrier outside any danger zone
            if (!safetyBarrier())
            {
              if (debug == 2)
              {
                Serial.println("SafetyBarrier exited");
              }

              //Return to normal moving
              movingState = normalMove;
            }
            //Default steps are not overrided yet, so continue moving
            else if (moveStep(newPos))
            {
              updateCntBottomBarrier();
              updateCntTopBarrier();
            }
            //Done moving
            else
            {
              movingState = doneMoving;
            }

          }
          //BarrierCounter exceeded
          else
          {
            //Set moving state to not possible
            movingState = moveNotPossible;
          }
          break;

        //End stops are triggered
        case endStopMoving:
          if (debug == 2)
          {
            Serial.println("EndstopMoving");
          }
          //check if endstop is still active
          if (limitTrigged())
          {
            //Check direction
            if (isDirectionSafe(newPos))
            {
              if (debug == 2)
              {
                Serial.println("dir safe - moving");
              }

              //Do moving and check if moving is done
              if (!moveStep(newPos))
              {
                if (debug == 2)
                {
                  Serial.println("moving done");
                }

                movingState = doneMoving;
              }
            }
            //Moving is not possible because of direction
            else
            {
              if (debug == 2)
              {
                Serial.println("dir unsafe");
              }

              movingState = moveNotPossible;
            }
          }
          //Move normally
          else
          {
            movingState = normalMove;
          }
          break;

        //Not possible to move - SafetyMoving has exceed move range or try to move in same direction as the triggered end stop
        case moveNotPossible:
          if (debug == 2)
          {
            Serial.println("MoveNotPossible");
          }
          mainCommand = failure;
          break;

        //The position has been reached
        case doneMoving:
          if (debug == 2)
          {
            Serial.println("doneMoving");
          }
          turnOnBrake();
          mainCommand = idle;
          break;

        default:
          break;
      }
      break;


    //--------------------------------------------------------------------------------------------------
    //Calibrate the robot
    case doCalibrate:
      if (debug) {
        Serial.println(F("Main: Do calibrate"));
      }
      //Set state
      inState = busy;
      //Calibrate
      calibrationWithSafety();
      //Set to idle
      mainCommand = idle;
      break;


    //-------------------------------------------------------------------------
    // Turn On vacuum.
    case vacuumOn:
      if (debug) {
        Serial.println("Main: DO SUCTION NO*****");
      }
      sendInt(ACK); // Send an Acknowledge
      turnVacuumOn();
      mainCommand = idle;
      break;

    //-------------------------------------------------------------------------
    // Turn Off vacuum.
    case vacuumOff:
      if (debug) {
        Serial.println("Main: DO SUCTION OFF *****");
      }
      sendInt(ACK); // Send an Acknowledge
      turnVacuumOff();
      mainCommand = idle;
      break;


    //--------------------------------------------------------------------------------------------------
    //The arduino has recieved an STOP command
    case stopRobot:
      if (debug) {
        Serial.println(F("Main: Stopped"));
        Serial.println(F("Fucntion not made"));
      }
      turnOnBrakeDisableMotors();
      inState = stopped;

      break;


    //--------------------------------------------------------------------------------------------------
    //The is in failure mode has recieved an STOP command
    case failure:
      if (debug) {
        Serial.println(F("Failure"));
      }
      inState = checkFailure();
      turnOnBrakeDisableMotors();
      /**** TODO: Make delay function.
        STOP shold be handled in an stop function.
        This function turns on brake and disables
        the motors afte T given time****/
      if (debug) {
        Serial.println("DELAY IS USED FOR HOLDING THE MOTOR UNTIL THE BRAKE IS SAFLY TURND ON");
      }
      delay(100);
      disableMotors();
      break;

    //--------------------------------------------------------------------------------------------------

    default:
      if (debug == 3)
      {
        Serial.println("Command not recognised");
        Serial.println("In default");
      }
      break;
      //--------------------------------------------------------------------------------------------------
  }


  //--------------------------------------------------------------------------------------------------

}







