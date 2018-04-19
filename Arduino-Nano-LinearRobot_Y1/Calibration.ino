
/**
   method for callibrating the distances in the x and y axes, takes a pointer
   to array as input for returning the number of steps to the call.
*/
//TODO: Remember to check DIR_LEFT & RIGHT
void calibration()
{
    //Enable motors
    enableMotors();
    // declaring states for calibration
    const int FIND_ZERO = 0;
    const int FIND_AREA = 1;
  
    // setting state to go to home position
    int state = FIND_ZERO;
  
    // declaring calibration variable
    bool calibrated = false;
  
    // run calibration until calibration is complete
    while (!calibrated)
    {
      switch (state)
      {
        // motors run until they reach their homepositions (0,0)
        case FIND_ZERO:
  
          // run x-axis motor until home position is reached
          if (ATDConverter(X_HOME_SWITCH))
          {
  
            motorX.step(DEC_DIR);
            motorX.setSpeed(stepSpeed);
          }
  
          // run y-axis motor until home position is reached
          if (ATDConverter(Y_HOME_SWITCH))
          {
            motorY.step(DEC_DIR);
            motorY.setSpeed(stepSpeed);
          }
          // if x and y motor is at their home positions update axis positions to 0,
          // and switch to away state
          if (!ATDConverter(X_HOME_SWITCH) && !ATDConverter(Y_HOME_SWITCH))
          {
            currentPos.x = 0;
            currentPos.y = 0;
  
            //Set state value
            state = FIND_AREA;
            if (calibTest) {
              Serial.println("Reached home");
            }
          }
          break;
  
  
        // motors run until they reach their away positions(x,y)
        case FIND_AREA:
  
          // run x-axis motor and count steps until away position is reached
          if (ATDConverter(X_OUTER_SWITCH))
          {
            motorX.step(INC_DIR);
            motorX.setSpeed(stepSpeed); //low speed for higher accuracy
            currentPos.x++;
          }
  
          // run y-axis motor and count stepil away position is reached
          if (ATDConverter(Y_OUTER_SWITCH))
          {
            motorY.step(INC_DIR);
            motorY.setSpeed(stepSpeed);
            currentPos.y++;
          }
  
  
          // if x and y motor is at their away positions set calibration variable to true
          if (!ATDConverter(X_OUTER_SWITCH) && !ATDConverter(Y_OUTER_SWITCH))
          {
            //Calibration done
            calibrated = true;
            maxPos.x = currentPos.x;
            maxPos.y = currentPos.y;
            if (calibTest) {
              Serial.println("Reached outer limit");
              Serial.print("Max pos X");
              Serial.println(maxPos.x);
              Serial.print("Max pos Y");
              Serial.println(maxPos.y);
            }
  
            //Enable motors
            disableMotors();
            break;
          }
      }
    }
}
