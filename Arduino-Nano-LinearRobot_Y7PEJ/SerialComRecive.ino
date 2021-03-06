/**Function for handeling inn and out data on the serial communication.
  It contains an switch case holding on all possible commands and requests.
  Thius devise til only read and handle data ment for it by compering the indata
  with the DEVICE_NAME. If the data isn for this device the buffer will be flushed,

**/                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        


    void receiveSerialEvent() {
  if (debug) {
    // Serial.println("RecieveEvent(W)");
  }
  //Check if wire is ready to be read from/incomming bits
  if (Serial.available() > 0) {
    if (Serial.find(DEVICE_NAME)) {
      if (debug) {
        Serial.println("This massage is for me!!");
      }

      //Nr of bytes in this response
      short nrOfInts = 3;
      int response[nrOfInts] = {0, 0, 0};
      int cnt = 0; 
      // Reads incomming command
      int inCommand = Serial.parseInt(); // Secound Byte represents an command.

      // Serial.write(inCommand);
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
          if (debug) {
            Serial.println(F("*****DO MOVEROBOT TO RECOGNIZED*****"));
          }

          //sendInt(ACK); // Send an Acknowledge

          /***Incomming values parsing****/
          //Set the new incomming positions
          newPos.x = MMToSteps(Serial.parseFloat(), X_AXIS);
          newPos.y = MMToSteps(Serial.parseFloat(), Y_AXIS);
          if (debug) {
            Serial.println(X_STEP_PER_MM);
            Serial.println(Y_STEP_PER_MM);
            Serial.print(F("New x pos in step: " ));
            Serial.println( newPos.x );
            Serial.print(F("New y pos in step: " ));
            Serial.println( newPos.y );
          }
          mainCommand = moveRobotTo;
          break;

        //-------------------------------------------------------------------------
        // Recived cmd for calibrating the robot.
        case stopRobot:
          if (debug) {
            Serial.println(F("**STOP ROBOT***"));
          }
          //sendInt(ACK); // Send an Acknowledge
          mainCommand = stopRobot;
          break;
        //-----------------

        
        //Moves in Y dir to tray endstop is low
        case findTray:
          if (debug) {
            Serial.println(F("***** DO FIND TRAY *****"));
          }
          //sendInt(ACK); // Send an Acknowledge
          mainCommand = findTray;

          break;





        //-------------------------------------------------------------------------
        // Recived cmd for calibrating the robot.
        case doCalibrate:
          if (debug) {
            Serial.println(F("**DO CALIBRATE RECOGNIZED***"));
          }
          //sendInt(ACK); // Send an Acknowledge
          mainCommand = doCalibrate;
          break;
        //-----------------

        //-------------------------------------------------------------------------
        // Recived cmd for calibrating the robot.
        case magnetOn:
          if (debug) {
            Serial.println("*****DO MAGNET NO*****");
          }
          //sendInt(ACK); // Send an Acknowledge
          mainCommand = magnetOn;
          break;
        //-----------------





        //      //-------------------------------------------------------------------------
        // Recived cmd for calibrating the robot.
        case magnetOff:
          if (debug) {
            Serial.println("*****DO MAGNET NO*****");
          }
          //sendInt(ACK); // Send an Acknowledge
          mainCommand = magnetOff;
          break;


        //------------------------------------------------------------------------------------------
        //Chnage color command receved.
        case changeColor:
          if (debug) {
            Serial.println("*****DO CANGE THE COLOR ON THE LEDS*****");
          }
          //sendInt(ACK); // Send an Acknowledge
          for (int ledColorInt = R; ledColorInt <= B; ledColorInt++)
          {
            ledRGB[ledColorInt] = Serial.parseInt();
          }
          mainCommand = changeColor;
          break;


        //-------------------------------------------------------------------------
        //Chnage color command receved.
        case discoLight:
          if (debug) {
            Serial.println("*****DO CANGE THE COLOR ON THE LEDS*****");
          }
         // sendInt(ACK); // Send an Acknowledge
          mainCommand = discoLight;
          break;


        //-------------------------------------------------------------------------
        //Changing velocity command receved
        case changeVelocity:
          if ( debug) {
            Serial.println("***** CANGE VELOCETY  *****");
          }
         // sendInt(ACK); // Send an Acknowledge

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
          if (debug) {
            Serial.println("*******RETRIVE STATE*******:");
          }
         // sendInt(ACK); // Send an Acknowledge
          sendInt(inState);
          break;


        //Send the calibrated parameters
        case calibParam:
          if (debug) {
            Serial.println("********CALIB PARAM *******");
          }
         // sendInt(ACK); // Send an Acknowledge
          
          //***Add the values******
          response[cnt++] =  parameter;
          response[cnt++] =  stepsToMM(maxPos.x, X_AXIS);
          response[cnt++] =  stepsToMM(maxPos.y, Y_AXIS);
          sendIntegers(response, nrOfInts);
          break;


        //-------------------------------------------------------------------------
        //Set the command for calibrating the robot.
        default:
          if (debug) {
            Serial.println("*****DAFAULT CASE*****");
            Serial.println("Invalid or not found command");
          }
         // sendInt(NACK); // Send an Negative-Acknowledge
          break;
          //-----------------
      }
    } else {
      Serial.flush();
    }
  }
}

