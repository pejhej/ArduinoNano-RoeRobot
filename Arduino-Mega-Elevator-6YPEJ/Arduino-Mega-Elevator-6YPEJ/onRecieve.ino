///**
//   function that executes whenever data is received from master
//   this function is registered as an event, see setup()
//   When this event is called. Master wants to WRITE something to the arduino
//   It is ALWAYS an command, that has to be performed.
//
//*/
//
//void receiveEvent() {
//
//  Serial.println("RecieveEvent(W)");
//
//  //Byte to store the register address from master to
//  byte addr;
//  //Check if wire is ready to be read from/incomming bits
//  if (Wire.available() > 0)
//    addr = Wire.read();
//
//  
//  //Byte[] to store incomming bytes on
//  //TODO: Make incByte dynamic from how much is incomming
//  int defaultBytes = 4;
//  byte incBytes[defaultBytes];
//  int cnt = 0;
//
//
//Serial.println("Addr " + addr);
//
//
//
//  //--------------------------------------------------------------------------------------------------
//  //SET ADDRESS TO THE COMMAND ENUM
//  recieveCommand = (command) addr;
//  reqCommand = (requestCommand) addr;
//
//  
////Switch command to perform the tasks given by address
//  switch (recieveCommand)
//  {
//    //------------------------------------------------------------------
//    //Command for moving the robot
//    //TODO: READ INCOMMING BYTES AND STORE THEM IN DESIRED VARIABLES
//    case moveRobot:
//      Serial.println("*****DO MOVEROBOT RECOGNIZED*****");
//
//      //The first byte is the amount of bytes incomming
//      byte incByteSize;
//      incByteSize = Wire.read();
//      incBytes[incByteSize];
//      
//      //READ THE INCOMMING BYTES..
//      //TODO: CHANGE AND STORE IN VARIABLES INSTEAD.
//      while (0 < Wire.available() || (cnt <= incByteSize))
//      {
//        incBytes[cnt] = Wire.read();
//        ++cnt;
//      }
//      
//      /***Incomming values parsing****/
//      //INCOMMING VALUES ARE SHORT, SO STORE THEM IN SHORT(two bytes) VARIABLES
//      short zVal;
//      zVal = incBytes[1] * 256 + incBytes[2];
//
//      //Print the values
//      Serial.print("z value ");
//      Serial.println(zVal);
//
//      mainCommand = recieveCommand;
//      break;
//
//
//
//      
//    //-------------------------------------------------------------------------
//    //Set the command for calibrating the robot.
//    case doCalibrate:
//      Serial.println("*****DO CALIBRATE RECOGNIZED*****");
//      mainCommand = recieveCommand;
//      break;
//      //-----------------
//
//
//     //-------------------------------------------------------------------------
//    //Set the command for calibrating the robot.
//    case turnOnLight:
//      Serial.println("*****DO TURN ON LIGHT RECOGNIZED*****");
//      mainCommand = recieveCommand;
//      break;
//      //-----------------
//
//
//     //-------------------------------------------------------------------------
//    //Set the command for calibrating the robot.
//    case changeVelocity:
//      Serial.println("*****CHANGE VELOCITY RECOGNIZED*****");
//      mainCommand = recieveCommand;
//      break;
//      //-----------------
//
//     //-------------------------------------------------------------------------
//    //Set the command for calibrating the robot.
//    case changeAcceleration:
//      Serial.println("*****CHANGE VELOCITY RECOGNIZED*****");
//      mainCommand = recieveCommand;
//      break;
//      //-----------------
//
//
////-------------------------------------------------------------------------
//    //Set the command for calibrating the robot.
//    case stopRobot:
//      Serial.println("*****Stop ROBOT recognized****");
//      mainCommand = recieveCommand;
//      break;
//      
//       default:
//      Serial.println("Recieve command not recognized");
//  }
//
//  //--------------------------------------------------------------------------------------------------
//
////Remove the wire buffer
//Wire.flush();
//
//
//
//
//}

