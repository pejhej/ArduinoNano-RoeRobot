
/**
   method for callibrating the distances in the x and y axes, takes a pointer
   to array as input for returning the number of steps to the call.
*/

//TODO: MAKE DETECTION OF TRAYS ONLY BY BOTTOM IR-SENSOR


void calibration() {


  //states for moving
  /**** TODO: Make an enum ***/

  const int FIND_BOTTOM = 0;
  const int FIND_TOP = 1;
  const int FIND_FLAGS = 2;
  bool moving = false;

  //setting state to go to bottom
  int state = FIND_BOTTOM;

  //Turn off the brakes

  while (!moving)
  {
    //running this state until it it is completed
    switch (state)
    {

      case FIND_BOTTOM:
        turnOffBrake();
        //finding (0,0) position, which is bottom
        if (digitalRead(LEFT_BOTTOM_SWITCH))
        {

          moveLeftMotor(LEFT_DIR_DOWN);
        }

        if (digitalRead(RIGHT_BOTTOM_SWITCH))
        {
          moveRightMotor(RIGHT_DIR_DOWN);
        }

        if (!digitalRead(LEFT_BOTTOM_SWITCH) && !digitalRead(RIGHT_BOTTOM_SWITCH))
        {
          turnOnBrake();
          currentPos.zLeft = 0;
          currentPos.zRight = 0;
          state = FIND_BOTTOM;
          if (debug == 1)
            Serial.println("reached bottom");
        }
        break;

      //-------------------------------------------------------------------------
      case FIND_TOP:

        turnOffBrake();
        // run left motor and count steps until away position is reached
        if (digitalRead(LEFT_TOP_SWITCH))
        {
          motor_left.step(LEFT_DIR_UP);
          motor_left.setSpeed(stepSpeed); //low speed for higher accuracy
          currentPos.zLeft++;
        }

        // run y-axis motor and count stepil away position is reached
        if (digitalRead(RIGHT_TOP_SWITCH))
        {
          motor_right.step(RIGHT_DIR_UP);
          motor_right.setSpeed(stepSpeed);
          currentPos.zRight++;
        }


        // if left and right motor is at their top positions set variable to true
        if (!digitalRead(LEFT_TOP_SWITCH) && !digitalRead(RIGHT_TOP_SWITCH))
        {
          //Turn brakes on
          turnOnBrake();
          //Exit the move loop
          moving = true;

          if (debug == 1)
            Serial.println("reached top");

        }
        break;


    }
  }
}





bool calibrationWithSafety()
{

  // Turns on motors and disabled the brakes
  turnOnMotorsDisableBrakes();
  bool calibrationFinished = true;

  //states for moving
  const int FIND_BOTTOM = 0;
  const int FIND_TOP = 1;
  const int FIND_FLAGS = 2;
  const int FAILURE = 3;
  const int DONE = 4;
  bool moving = true;

  //Calib pos
  /* Pos calibPos;
    calibPos.zLeft = currentPos.zLeft;
    calibPos.zRight = currentPos.zRight;
  */
  int barrierCounterLeft = 0;
  int barrierCounterRight = 0;
  //setting state to go to bottom
  int state = FIND_BOTTOM;



  while (moving)
  {
    //running this state until it it is completed
    switch (state)
    {

      case FIND_BOTTOM:
        //finding (0,0) position, which is bottom
        if (digitalRead(LEFT_BOTTOM_SWITCH))
        {
          //IF safetybarrier is activated, check the barrier counter
          if (safetyBarrierTop() || safetyBarrierBottom())
          {
            if (barrierCounterLeft < flagDetectionSteps && !counterExceeded())
            {
              moveLeftMotor(LEFT_DIR_DOWN);
              ++barrierCounterLeft;
            }
            else
            {
              state = FAILURE;
            }
          }
          //No problem moving
          else
          {
            if (debug == 1) {
              Serial.println("Moving LEFT with safety OK");
            }

            moveLeftMotor(LEFT_DIR_DOWN);
            barrierCounterLeft = 0;
          }
        }

        if (digitalRead(RIGHT_BOTTOM_SWITCH))
        {
          //IF safetybarrier is activated, check the barrier counter
          if (safetyBarrierTop() || safetyBarrierBottom())
          {
            //Check the safety counters
            if ((barrierCounterRight < flagDetectionSteps) && !counterExceeded())
            {
              moveRightMotor(RIGHT_DIR_DOWN);
              ++barrierCounterRight;
            }
            else
            {
              state = FAILURE;
            }
          }
          //No problem moving
          else
          {
            if (debug == 1) {
              Serial.println("Moving RIGHT with safety OK");
            }

            moveRightMotor(RIGHT_DIR_DOWN);
            barrierCounterRight = 0;
          }
        }
        //Both endstops are activated, so bottom is found
        if (!digitalRead(LEFT_BOTTOM_SWITCH) && !digitalRead(RIGHT_BOTTOM_SWITCH))
        {
          currentPos.zLeft = 0;
          currentPos.zRight = 0;
          state = FIND_TOP;
          if (debug == 1) {
            Serial.println("reached bottom");
          }
        }
        break;

      //-------------------------------------------------------------------------
      //TODO: Is barriercounter left/right necessary??
      case FIND_TOP:
        // run left motor and count steps until away position is reached
        if (digitalRead(LEFT_TOP_SWITCH))
        {
          //IF safetybarrier is activated, check the barrier counter
          if (safetyBarrierTop() || safetyBarrierBottom())
          {
            if ((barrierCounterLeft < flagDetectionSteps) && !counterExceeded())
            {
              moveLeftMotor(LEFT_DIR_UP);
              ++barrierCounterLeft;
            }
            else
            {
              state = FAILURE;
            }
          }
          //No problem moving, move normally
          else
          {
            moveLeftMotor(LEFT_DIR_UP);
            barrierCounterLeft = 0;
          }

        }

        // run y-axis motor and count stepil away position is reached
        if (digitalRead(RIGHT_TOP_SWITCH))
        {
          //IF safetybarrier is activated, check the barrier counter
          if (safetyBarrierTop() || safetyBarrierBottom())
          {
            if ((barrierCounterRight < flagDetectionSteps) && !counterExceeded())
            {
              moveRightMotor(RIGHT_DIR_UP);
              ++barrierCounterRight;
            }
            else
            {
              state = FAILURE;
            }
          }
          //No problem moving
          else
          {
            moveRightMotor(RIGHT_DIR_UP);
            barrierCounterRight = 0;
          }
        }


        // if left and right motor is at their top positions set variable to true
        if (!digitalRead(LEFT_TOP_SWITCH) || !digitalRead(RIGHT_TOP_SWITCH))
        {

          //Save the found variables
          maxPos.zLeft = currentPos.zLeft;
          maxPos.zRight = currentPos.zRight;

          state = FIND_FLAGS;

          if (debug == 1)
          {
            Serial.println("reached top");
          }

        }
        break;

      //Go down and find the trays
      case FIND_FLAGS:
        if (debug == 1)
        {
          Serial.println("Finding flag");
        }
        //IF safetybarrier is activated, check the barrier counter
        if (digitalRead(LEFT_BOTTOM_SWITCH) && digitalRead(RIGHT_BOTTOM_SWITCH))
        {
          if (debug == 1)
          {
            printSteps();
          }

          //IF safetybarrier is activated, check the barrier counter
          if (safetyBarrierTop() || safetyBarrierBottom())
          {
            if (debug == 1)
              Serial.println("FindFlag - SafetyBarr");
            //Check for the bottom safety barrier
            if (safetyBarrierBottom())
            {
              if (debug == 1) {
                Serial.println("Safety Bottom");
              }

              //Check if counter is not exceeded, then move the motors and update counters
              if (!counterExceeded())
              {
                moveLeftMotor(LEFT_DIR_DOWN);
                moveRightMotor(RIGHT_DIR_DOWN);
                updateCntBottomBarrier();
                updateCntTopBarrier();

                //Check if safety barrier is still active
                //IF NOT - it means a flag has been detected
                if (!safetyBarrierBottom())
                {
                  if (debug == 1) {
                    Serial.println("Creating tray");
                  }
                  createTrayOnPos();
                  if (debug) {
                    Serial.print("Nr of trays in reg: ");
                    Serial.print(trayReg.nrOfTrays);
                  }
                }
              }
              //Set state ti failure as counter safety counter has exceed its limits
              else
              {
                state = FAILURE;
              }


            }
            //Top safety barrier was not activated
            else
            {
              //Check if counter is not exceeded, then move the motors and update counters
              if (!counterExceeded())
              {
                moveLeftMotor(LEFT_DIR_DOWN);
                moveRightMotor(RIGHT_DIR_DOWN);
                updateCntBottomBarrier();
                updateCntTopBarrier();
              }
              else
              {
                state = FAILURE;
              }
            }


          }
          //No problem moving
          else
          {
            if (debug == 1) {
              Serial.println("Moving normal");
            }

            moveRightMotor(RIGHT_DIR_DOWN);
            barrierCounterRight = 0;
            moveLeftMotor(LEFT_DIR_DOWN);
            barrierCounterLeft = 0;

            //Update the counter barrier
            updateCntBottomBarrier();
            updateCntTopBarrier();
          }
        }
        else
        {
          state = DONE;
        }


        break;
      //Exit the moving loop
      case DONE:
        moving = false;
        break;
      //Failure happened during the calibration
      case FAILURE:
        calibrationFinished = false;
        moving = false;
        break;
    }
  }
  turnOnBrakeDisableMotors();
  return calibrationFinished;
}











