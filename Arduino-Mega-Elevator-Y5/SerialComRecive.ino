/**
   function that executes whenever data is received from master
   this function is registered as an event, see setup()
   When this event is called. Master wants to WRITE something to the arduino
   It is ALWAYS an command, that has to be performed.

*/

void receiveSerialEvent() {
  if (debug) {
    Serial.println("RecieveEvent(W)");
  }
  //Check if wire is ready to be read from/incomming bits
  if (Serial.available() > 0) {
    if (Serial.find(DEVICE_NAME)) {
      if (debug) {
        Serial.println("This massage is for me!!");
      }

      //Byte[] to store incomming bytes on

      // Response array made for returning calib param
      // The calib param contains in state,  maxPos.zLeft and (GONEmaxPos.zRigth) + the pos of each tray found. (Left)
      // Therefor we need an array of 1(instate) +  1(maxPos) + (numberOftrays)
      int arrayLength = (2 + (trayReg.nrOfTrays ));
      int response[arrayLength];
      int cnt = 0;

      //--------------------------------------------------------------------------------------------------
      // Reads incomming command
      int inCommand = Serial.parseInt(); // Secound Byte represents an command.
      // Updates the commands
      recieveCommand = inCommand;

      //Switch command to perform the tasks given by address
      switch (recieveCommand)
      {
        //------------------------------------------------------------------
        //Command for moving the robot
        //TODO: READ INCOMMING BYTES AND STORE THEM IN DESIRED VARIABLES
        case moveRobot:
          if (debug) {
            Serial.println("*****DO MOVEROBOT RECOGNIZED*****");
          }
          sendInt(ACK); // Send an Acknowledge
          /***Incomming values parsing****/
          //Set the new incomming positions
          newPos.zLeft = MMToSteps(Serial.parseFloat());
          newPos.zRight = -newPos.zLeft;

          if (debug) {
            //Print the values
            Serial.print("Z left value ");
            Serial.println(newPos.zLeft);
            Serial.print("Z rigth value ");
            Serial.println(newPos.zRight);
          }
          movingState = normalMove;
          // Main case to move robot
          mainCommand = recieveCommand;
          break;




        //-------------------------------------------------------------------------
        //Set the command for calibrating the robot.
        case doCalibrate:
          if (debug) {
            Serial.println("*****DO CALIBRATE RECOGNIZED*****");
          }
          sendInt(ACK); // Send an Acknowledge
          mainCommand = recieveCommand;
          break;
        //-----------------


        //-------------------------------------------------------------------------
        //Set the command for calibrating the robot.
        case stopRobot:
          if (debug) {
            Serial.println("*****Stop ROBOT recognized****");
          }
          sendInt(ACK); // Send an Acknowledge
          mainCommand = recieveCommand;
          break;




        //-------------------------------------------------------------------------
        // Turn On vacuum.
        case vacuumOn:
          if (debug) {
            Serial.println("*****DO SUCTION NO*****");
          }
          sendInt(ACK); // Send an Acknowledge
          mainCommand = recieveCommand;
          break;

        //-------------------------------------------------------------------------
        // Turn Off vacuum.
        case vacuumOff:
          if (debug) {
            Serial.println("*****DO SUCTION NO*****");
          }
          sendInt(ACK); // Send an Acknowledge
          mainCommand = recieveCommand;
          break;



        //-------------------------------------------------------------------------
        //Changing velocity command receved
        case changeVelocity:
          if (debug) {
            Serial.println("***** CANGE VELOCETY  *****");
          }
          sendInt(ACK); // Send an Acknowledge

          // Uppdate the step speed
          stepSpeed = Serial.parseInt();

          if (debug) {
            Serial.print("Z velocety: ");
            Serial.println(stepSpeed);
          }
          break;


        //-------------------------------------------------------------------------
        //Set the command for calibrating the robot.
        case changeAcceleration:
          if (debug) {
            Serial.println("*****CHANGE ACCELERATION RECOGNIZED*****");
          }
          sendInt(ACK); // Send an Acknowledge
          stepSpeed = Serial.parseInt();
          break;
        //-----------------





        /**** REQUEST EVENTS ****/
        //-----------------------------------------------------------------------
        // Retrive state command recived.
        case retrieveState:
          if (debug) {
            Serial.println("*******RETRIVE STATE*******: ");
          }
          sendInt(ACK); // Send an Acknowledge
          sendInt(inState);
          break;


        case calibParam:
          if (debug) {
            Serial.println("CalibParam recognized        --");
          }
          sendInt(ACK); // Send an Acknowledge

          //Add the values
          response[cnt++] = parameter;
          response[cnt++] = stepsToMM(maxPos.zLeft);
          // response[cnt++] = stepsToMM(maxPos.zRight);
          
          for (int i = 0; i < trayReg.nrOfTrays; i++) {
            Tray tray = trayReg.trayList[i];
            Pos pos = tray.zPos;
            // Stor the tray positions in the response
            response[cnt++] = stepsToMM(pos.zLeft);
            // response[cnt++] = stepsToMM(pos.zRight);
          }
          sendIntegers(response, arrayLength);
          // If debug print responce
          if (debug) {
            for (int i = 0; i < arrayLength; i++) {
              Serial.print("Response: " );
              Serial.println(response[i]);
            }
          }
          break;


        default:
          if (debug) {
            Serial.println("Recieve command not recognized");
          }
          sendInt(NACK); // Send an Acknowledge

      }

      //--------------------------------------------------------------------------------------------------

    } else {
      //Remove the serial buffer
      Serial.flush();
    }
  }
}

