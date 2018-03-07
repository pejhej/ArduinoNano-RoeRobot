




#include <Wire.h>

#define ANSWERSIZE 3
#define ADDRESS 0x03


//Constants for sensor pins
const int endstop_bottomLeft_pin = 10;
const int endstop_bottomRight_pin = 11;

const int endstop_topLeft_pin = 12;
const int endstop_topRight_pin = 13;

//Constants for motor pins
const int motor_x_pin = A1;
const int motor_y_pin = A2;

const int servo_pin;

const int debug = 0;

//Boolean if state is requested.. dont know if needed
boolean requestedState = false;
boolean failure = false;
//boolean endstopReached = false;



/**
   Structure to hold position
*/
typedef struct Pos
{
  short x;
  short y;
} Pos;

/***Defining variables**/
Pos lastPos;
Pos newPos;
Pos currentPos;
Pos maxRange;











//-------------------------------ENUMS------------------------------
//ENUMS to keep track of what has been sent to the arduino, and what it should do.

//The state of the arduino
enum State {
  busy = 0x50,
  readyToRecieve = 0x51,
  stopped = 0x52,
  emc = 0x61,
  parameter = 0x70,
  endstopReached = 0x64
};
/**
 * *COMMANDS* the Master can send to the slave.
*/
enum Command {
  moveRobot = 0x05,
  doSuction = 0x06,
  stopRobot = 0x07,
  doCalibrate = 0x10,
  turnOnLight = 0x11,
  changeVelocity = 0x20,
  changeAcceleration = 0x21,
  openTray = 0x22,
  closeTray = 0x23,
  idle = 0x01,
};

//ENUM for use in requestEvent,
enum RequestCommand {
  retrieveState = 0x30,
  calibParam = 0x31,
};



//Create enums
enum State inState;             //To keep track of which STATE the arduino currently is in. Busy when doing stuff, ready/idle when not. EMC, encoder failure, ETC...
enum Command mainCommand;       //Switch case command for the main loop
enum Command recieveCommand;    //Switch case command for the RECIEVE command, reading storing values to be done later.
enum RequestCommand reqCommand; 
//enum command checkCommand;












void setup() {
  //Serial.begin(9600);           // start serial for output
  Wire.begin(ADDRESS);                // join i2c bus with address
  Serial.println("Setup begin");
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  
  //Set the pin mode for the attached devices
  pinMode(endstop_bottomLeft_pin, INPUT);
  pinMode(endstop_bottomRight_pin, INPUT);
  pinMode(endstop_topLeft_pin, INPUT);
  pinMode(endstop_topRight_pin, INPUT);
  
  Serial.println("Setup done");

}


//TESTING VARIABLES
int moveDelay = 50000;
int counter = 0;


//The main loop

void loop() {
  if(debug == 0)
  Serial.println("Looping ");

  
  //debug for failchecking
  checkEndstops();
  checkEncoderSync();

  
  //IF SOMETHING IS OUT OF BOUNDS OR NOT CORRECT
  if (!failure)
  {
    //Switch command for the
    //--------------------------------------------------------------------------------------------------
    switch (mainCommand)
    {
      case moveRobot:
      //Set state to busy
        inState = busy;
        
      checkEndstops();
      if(endstopReached)
      //If endstop is reached check if it can continue to move
      //Check which endstop is reached, and which direction it can move
      //If it cant move in the preferred direction, switch state to endstopReached
      //And bypass the moveRobot command.
      //


     
        //Do the moving
       
          

          checkEndstops();


          //FOR TESTING
         ++counter;
          if(counter > moveDelay)
          //  mainCommand = idle;
          
        break;

//--------------------------------------------------------------------------------------------------

      //Calibrate the robot
      case stopRobot:
      //Set state
      inState = stopped;
      //Save all variables and set state
        
      
        break;
        
 //--------------------------------------------------------------------------------------------------
      //Calibrate the robot
      case doCalibrate:
      //Set state
        inState = busy;
        
        
        maxRange.x = 22222;
        maxRange.y = 25000;
        mainCommand = idle;
        break;
        
 //--------------------------------------------------------------------------------------------------
      //Do whats required for suction
      case doSuction:
        //Set state to busy
        inState = busy;

        //Do the suction
        //Start pump, wait in a given short given timeframe
        //Stop pump.
        //Suction done.
        mainCommand = idle;
          Serial.println("Do Suction");
          break;
          
 //--------------------------------------------------------------------------------------------------
       case turnOnLight:
         //Set state to busy
        inState = busy;
        //Turn on light
        
          //Serial.println("Suction");
          break;
          
 //--------------------------------------------------------------------------------------------------
      //Lock gripper
        case openTray:
        //Set state busy
        inState = busy;
        //Turn the servo for the gripper to lock position

        mainCommand = idle;
          //Serial.println("Suction");
          break;
          
 //--------------------------------------------------------------------------------------------------
    //Release gripper
     case closeTray:
        //Set state busy
        inState = busy;
       //Turn the servo for the gripper to open position
       
       mainCommand = idle; 
          //Serial.println("Suction");
          break;
          
 //--------------------------------------------------------------------------------------------------     
           case changeVelocity:
        inState = busy;
          //Serial.println("Suction");
          break;
          
 //--------------------------------------------------------------------------------------------------
          
           case changeAcceleration:
           inState = busy;
          //Serial.println("Suction");
          break;
 //--------------------------------------------------------------------------------------------------
      //The arduino has nothing to do, and is therefore in idle
        case idle:
        inState = readyToRecieve;
        Serial.println("Idle");
        break;

 //--------------------------------------------------------------------------------------------------

      default:
         Serial.println("Command not recognised");
          //Serial.println("In default");
          break;
        //--------------------------------------------------------------------------------------------------
    }
  }
  else
    inState = emc;

}


//--------------------------------------------------------------------------------------------------





