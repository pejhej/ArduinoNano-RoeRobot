// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Receives data as an I2C/TWI slave device
// Refer to the "Wire Master Writer" example for use with this

// Created 29 March 2006

// This example code is in the public domain.
#include <Wire.h>

#define ANSWERSIZE 3
#define ADDRESS 0x03

//Boolean if state is requested.. dont know if needed
boolean requestedState = false;
boolean failure = false;



/**
   Structure to hold position
*/
typedef struct Pos
{
  short x;
  short y;
  short z;
} Pos;

/***Defining variables**/
Pos lastPos;
Pos newPos;
Pos currentPos;
Pos maxRange;











 //-------------------------------ENUMS------------------------------
//ENUMS to keep track of what has been sent to the arduino, and what it should do.
//The state of the arduino
enum state {
  busy = 0x50,
  readyToRecieve = 0x51,
  emc = 0x61,
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
  idle = 0x01,
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
  Serial.begin(9600);           // start serial for output
  Wire.begin(ADDRESS);                // join i2c bus with address #8
  Serial.println("Setup begin");
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent);
  Serial.println("Setup done");

}







void loop() {
  //Serial.println("Looping ");
  //debug for failchecking
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
        for (int i = 0; i <= 3; ++i)
          Serial.println("Moving cmd");
        break;

      //Calibrate the robot
      case doCalibrate:
        inState = busy;
        

        maxRange.x = 22222;
        maxRange.y = 25000;
        maxRange.z = 15560;
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





