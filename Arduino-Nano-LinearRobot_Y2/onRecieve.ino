//**
//   function that executes whenever data is received from master
//   this function is registered as an event, see setup()
//   When this event is called. Master wants to WRITE something to the arduino
//   It is ALWAYS an command, that has to be performed.
//
//*/
//void receiveEvent() {
//
//  Serial.println("RecieveEvent(W)");
//
//  //Byte to store the register address from master to
//  byte addr;
//  //Check if wire is ready to be read from/incomming bits
//  if (Serial.available() > 0) {
//    /* TODO: Make string parser for detecting Prefix words. (ADDRESS)  */
//    addr = Serial.read(); // First Byte represents address
//
//
//    // If the incomming adress equals to divice adress.
//    if (addr == ADDRESS) {
//      // Reads incomming command
//      byte inCommand = Serial.read(); // Secound Byte represents an command.
//      // Updates the commands
//      recieveCommand = inCommand;
//
//      /** MÅ ENDRES TIL SEND COMMAND. Må sende commando når en oppgave er utført og ikke på etterspøresl*/
//      reqCommand = inCommand;
//
//      //Byte[] to store incomming bytes on
//      //TODO: Make incByte dynamic from how much is incomming (SOLVED)
//      int defaultBytes = 4;
//      byte incBytes[defaultBytes];
//      int cnt = 0;
//
//      //Switch command to perform the tasks given by address
//      switch (recieveCommand)
//      {
//        //-----------------
//        //Command for moving the robot
//        //TODO: READ INCOMMING BYTES AND STORE THEM IN DESIRED VARIABLES
//        case moveRobot:
//          Serial.println("*****DO MOVEROBOT RECOGNIZED*****");
//
//          //The first byte is the amount of bytes incomming
//          byte incByteSize;
//          incByteSize = Serial.read(); // Third Byte represents number of incomming bytes
//          // Reads incomming bytes and stores them in incBytes.
//        //   Serial.read(incBytes[incByteSize] , incByteSize);
//
//          //          //READ THE INCOMMING BYTES..
//          //          //TODO: CHANGE AND STORE IN VARIABLES INSTEAD.
//          //          while (0 < Serial.available() && (cnt <= incByteSize))
//          //          {
//          //            incBytes[cnt] = Serial.read();
//          //            ++cnt;
//          //          }
//
//          /***Incomming values parsing****/
//          //INCOMMING VALUES ARE SHORT, SO STORE THEM IN SHORT(two bytes) VARIABLES
//          short xVal;
//          xVal = incBytes[1] * 256 + incBytes[2];
//          short yVal;
//          yVal = incBytes[3] * 256 + incBytes[4];
//
//          //Print the values
//          Serial.print("X value ");
//          Serial.println(xVal);
//          Serial.print("Y value ");
//          Serial.println(yVal);
//          //Set the new incomming positions
//          newPos.x = xVal;
//          newPos.y = yVal;
//
//          Serial.flush();
//          /*********************************************************************************************???????????????????????????????????????????????????????*/
//          mainCommand = recieveCommand;
//          break;
//
//        //-------------------------------------------------------------------------
//        //Set the command for calibrating the robot.
//        case doCalibrate:
//          Serial.println("*****DO CALIBRATE RECOGNIZED*****");
//          mainCommand = recieveCommand;
//          break;
//        //-----------------
//
//
//
//        //-------------------------------------------------------------------------
//        //Set the command for calibrating the robot.
//        case changeColor:
//          Serial.println("*****DO CANGE THE COLOR ON THE LEDS*****");
//          mainCommand = recieveCommand;
//          break;
//          //-----------------
//      }
//    }
//  }
//}
