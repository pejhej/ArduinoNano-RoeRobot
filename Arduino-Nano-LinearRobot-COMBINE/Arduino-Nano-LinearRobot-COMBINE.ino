#include <Stepper.h>
#include <Wire.h>

/******** Stepper ***********/
#define MOTOR_ENABLE 8
#define X_MOTOR_STEP 9
#define X_MOTOR_DIR 5
#define X_HOME_SWITCH 2
#define X_AWAY_SWITCH 3
#define Y_MOTOR_STEP 12
#define Y_MOTOR_DIR 6
#define Y_HOME_SWITCH 10
#define Y_AWAY_SWITCH 11
#define DIR_LEFT -1
#define DIR_RIGHT 1

#define STEP_PER_REVOLUTION 400
#define DRIVER_MICROSTEPS 0.25
#define BELT_PRESENT_PITCH 3 //mm 
#define PULLEY_TOOTH 20

/******** I2C Communication ***********/
#define ANSWERSIZE 3
#define ADDRESS 0x03



//Holds steps per mm
float stepPerMM = 0;

bool cal = true;

//Default stepper speed
int const stepSpeed = 450;
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




//Debug flag
boolean debug = true;


//Set steppers
Stepper motorX(STEP_PER_REVOLUTION, X_MOTOR_STEP, X_MOTOR_DIR);
Stepper motorY(STEP_PER_REVOLUTION, Y_MOTOR_STEP, Y_MOTOR_DIR);






 //-------------------------------ENUMS------------------------------
//ENUMS to keep track of what has been sent to the arduino, and what it should do.
//The state of the arduino
enum state {
  busy = 0x50,
  readyToRecieve = 0x51,
  emc = 0x60,
  encoderOutOfSync = 0x65,
  linearBotLimitTrigged = 0x64,
  safetySwitchLower = 0x61,
  safetySwitchUpper = 0x62,
  parameter = 0x70
};

/**
 * *COMMANDS* the Master can send to the slave.
*/
enum command {
  moveRobot = 0x05,
  doCalibrate = 0x10,
  doSuction = 0x61,
  turnOnLight = 0x11,
  gripperControl = 0x22,
  changeVelocity = 0x20,
  changeAcceleration = 0x21,
   //Main loop states
  idle = 0x01,
  EMC = 0x02,
  failure = 0x03
};

//ENUM for use in requestEvent,
enum requestCommand {

  retrieveState = 0x30,
  calibParam = 0x31,
};



//Create enums
enum state inState;             //To keep track of which STATE the arduino currently is in. Busy when doing stuff, ready/idle when not. EMC, encoder failure, ETC...
enum command mainCommand;       //Switch case command for the main loop
enum command recieveCommand;    //Switch case command for the RECIEVE command, reading storing values to be done later.
enum requestCommand reqCommand;
//enum command checkCommand;





void setup() {
  if(debug)
  {
    Serial.begin(19200);
  }
  
  //Set switch modes
  pinMode(X_HOME_SWITCH, INPUT_PULLUP);
  pinMode(Y_HOME_SWITCH, INPUT_PULLUP);
  pinMode(X_AWAY_SWITCH, INPUT_PULLUP);
  pinMode(Y_AWAY_SWITCH, INPUT_PULLUP);
  //Enables signal
  pinMode(MOTOR_ENABLE, HIGH);

  //Initialize the safety interrupts
  //When the limit switches are reached(pressed)
  attachInterrupt(digitalPinToInterrupt(X_HOME_SWITCH), xHomeLimitTrig, FALLING);
  attachInterrupt(digitalPinToInterrupt(X_AWAY_SWITCH), xAwayLimitTrig, FALLING);
 // attachInterrupt(digitalPinToInterrupt(Y_HOME_SWITCH), yHomeLimitTrig(), FALLING);
 // attachInterrupt(digitalPinToInterrupt(Y_AWAY_SWITCH), yAwayLimitTrig(), FALLING);
  //When the limit switches are released
  attachInterrupt(digitalPinToInterrupt(X_HOME_SWITCH), xHomeLimitTrig, RISING);
  attachInterrupt(digitalPinToInterrupt(X_AWAY_SWITCH), xAwayLimitTrig, RISING);
//  attachInterrupt(digitalPinToInterrupt(Y_HOME_SWITCH), yHomeLimitTrig(), RISING);
//  attachInterrupt(digitalPinToInterrupt(Y_AWAY_SWITCH), yAwayLimitTrig(), RISING);

  
  
  //Calculate step per mm
  stepPerMM = stepsToMMCalculation(STEP_PER_REVOLUTION, DRIVER_MICROSTEPS, BELT_PRESENT_PITCH, PULLEY_TOOTH);


  //Set default positions
  currentPos.x = 0;
  currentPos.y = 0;
  maxPos.x = 5000;
  maxPos.y = 3000;
  newPos.y = 0;
  newPos.x = 0;




  Wire.begin(ADDRESS);                // join i2c bus with address
  //Add the interrupts
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
}











//-------------------------------------------------------------------------------------------------------





void loop()
{
  

  if (debug)
  {
   /*
    if (!cal)
    {
      calibration();
      cal = true;
    }
    */
    /*
    newPos.x = 50;
    newPos.y = 75;
  */
    
 /*   while (moveStep(newPos.x, newPos.y))
    {
      Serial.println("Trying move");
      //Serial.println("Current x pos " + currentPos.x);
      Serial.println(currentPos.x);
      Serial.println(currentPos.y);
      //Serial.println("Current y pos " + currentPos.y);
      delay(100);
    }
   */ 
   Serial.print("xHomeSwitch ");
     Serial.println(xHomeSwitch);
     Serial.print("xAwaySwitch ");
     Serial.println(xAwaySwitch);
  }









      //Switch command for the main loop
    //--------------------------------------------------------------------------------------------------
    switch (mainCommand)
    {
      case moveRobot:
        //Set state to busy
        inState = busy;
        while(moveStep(newPos.x, newPos.y))
        {}
        
        if(!moveStep(newPos.x, newPos.y)
          mainCommand = idle;

          break;
      //Calibrate the robot
      case doCalibrate:
        inState = busy;
        
        //Do the calibration
        calibration();
        //Set the state to idle
        mainCommand = idle;
        break;

      //Do whats required for suction
      case doSuction:
        inState = busy;
          //Serial.println("Suction");
          break;


      case idle:
        inState = readyToRecieve;
        Serial.println("Idle");

        break;
        
      //Emergency stop
      case EMC:
      inState = emc;
      
      break;
      
      //Failure
      case failure:
      
      //check which failure has been triggered
      //if(limitSwitch())
      //if(
      break;
      default:
          //Serial.println("In default");
          break;
        //--------------------------------------------------------------------------------------------------
    }
    
}


//--------------------------------------------------------------------------------------------------


 




