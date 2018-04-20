/**Function for handeling inn and out data on the serial communication.
  It contains an switch case holding on all possible commands and requests.
  Thius devise til only read and handle data ment for it by compering the indata
  with the DEVICE_NAME. If the data isn for this device the buffer will be flushed,

**/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        


    void receiveSerialEvent() {
  Serial.println("RecieveEvent(W)");
  //Check if wire is ready to be read from/incomming bits
  if (Serial.available() > 0) {
    if (Serial.find(DEVICE_NAME)) {
      if (debug) {
        Serial.println("This massage is for me!!");
      }

      //Nr of bytes in this response
      short nrOfInts = 3;
      int response[nrOfInts] = {0, 0, 0};

      // Reads incomming command
      byte inCommand = Serial.parseInt(); // Secound Byte represents an command.
      // Updates the commands
      recieveCommand = inCommand;


      //-------------------RECIVE SWITCH CASE----------------------
      //Switch command to perform the tasks given by address
      switch (recieveCommand)
      {
        //-----------------
        //Command for moving the robot
        //TODO: READ INCOMMING BYTES AND STORE THEM IN DESIRED VARIABLES  (SOLVED)
        case moveRobotTo:
          Serial.println("*****DO MOVEROBOT TO RECOGNIZED*****");
          sendInt(ACK); // Send an Acknowledge
          /***Incomming values parsing****/
          //Set the new incomming positions
          newPos.x = Serial.parseInt();
          newPos.y = Serial.parseInt();
          /*       TESTING              */
          //      moveTo(newPos.x , newPos.y);
          //          if (debug) {
          //            //Print the values
          //            Serial.print("X value ");
          //            Serial.println(newPos.x);
          //            Serial.print("Y value ");
          //            Serial.println(newPos.y );
          //          }
          // Main case to move robot
          mainCommand = moveRobotTo;
          break;

        case moveRobot:
          Serial.println("*****DO MOVEROBOT*****");
          sendInt(ACK); // Send an Acknowledge
          /***Incomming values parsing****/
          moveXY(Serial.parseInt(), Serial.parseInt(), Serial.parseInt(), Serial.parseInt());
          // Main case to move robot
          //------ mainCommand = moveRobot;
          break;





        //-------------------------------------------------------------------------
        // Recived cmd for calibrating the robot.
        case doCalibrate:
          Serial.println("*****DO CALIBRATE RECOGNIZED*****");
          sendInt(ACK); // Send an Acknowledge
          mainCommand = doCalibrate;
          break;
        //-----------------

        //-------------------------------------------------------------------------
        // Recived cmd for calibrating the robot.
        case magnetOn:
          Serial.println("*****DO MAGNET NO*****");
          sendInt(ACK); // Send an Acknowledge
          mainCommand = magnetOn;
          break;
        //-----------------





        //      //-------------------------------------------------------------------------
        // Recived cmd for calibrating the robot.
        case magnetOff:
          Serial.println("*****DO MAGNET NO*****");
          sendInt(ACK); // Send an Acknowledge
          mainCommand = magnetOff;
          break;
        //------------------------------------------------------------------------------------------
        //Chnage color command receved.
        case changeColor:
          Serial.println("*****DO CANGE THE COLOR ON THE LEDS*****");
          sendInt(ACK); // Send an Acknowledge
          for (int ledColorInt = R; ledColorInt <= B; ledColorInt++)
          {
            ledRGB[ledColorInt] = Serial.parseInt();
          }
          mainCommand = changeColor;
          break;


        //-------------------------------------------------------------------------
        //Chnage color command receved.
        case discoLight:
          Serial.println("*****DO CANGE THE COLOR ON THE LEDS*****");
          sendInt(ACK); // Send an Acknowledge
          mainCommand = discoLight;
          break;


        //-------------------------------------------------------------------------
        //Changing velocity command receved
        case changeVelocity:
          Serial.println("***** CANGE VELOCETY  *****");
          sendInt(ACK); // Send an Acknowledge

          stepSpeed[X_AXIS] = Serial.parseInt();
          stepSpeed[Y_AXIS] = Serial.parseInt();

          if (debug) {
            Serial.print("X velocety: ");
            Serial.println(stepSpeed[X_AXIS]);
            Serial.print("Y velocety: ");
            Serial.println(stepSpeed[Y_AXIS]);
          }
          break;


        /**** REQUEST EVENTS ****/
        //-----------------------------------------------------------------------
        // Retrive state command recived.
        case retrieveState:
          Serial.println("*******RETRIVE STATE*******: ");
          sendInt(ACK); // Send an Acknowledge
          sendInt(inState);
          if (debug) {
            Serial.print("Response: ");
          }
          break;


        //Send the calibrated parameters
        case calibParam:
          Serial.println("********CALIB PARAM *******");
          sendInt(ACK); // Send an Acknowledge

          // Set state to parameter
          inState = parameter;                //Set the response state to "Parameters"

          //***Add the values******
          response[0] =  inState;
          response[1] =  maxPos.x;
          response[2] =  maxPos.y;
          sendIntegers(response);
          break;


        //-------------------------------------------------------------------------
        //Set the command for calibrating the robot.
        default:
          if (debug) {
            Serial.println("*****DAFAULT CASE*****");
            Serial.println("Invalid or not found command");
          }
          sendInt(NACK); // Send an Negative-Acknowledge
          break;
          //-----------------
      }
    } else {
      Serial.flush();
    }
  }
}

