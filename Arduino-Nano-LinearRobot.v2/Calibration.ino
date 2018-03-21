
/**
   method for callibrating the distances in the x and y axes, takes a pointer
   to array as input for returning the number of steps to the call.
*/
//TODO: Remember to check DIR_LEFT & RIGHT
void calibration()
{
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
        if (digitalRead(X_HOME_SWITCH))
        {
          motorX.step(DIR_LEFT);
          motorX.setSpeed(stepSpeed);
        }

        // run y-axis motor until home position is reached
        if (digitalRead(Y_HOME_SWITCH))
        {
          motorY.step(DIR_LEFT);
          motorY.setSpeed(stepSpeed);
        }

        if (digitalRead(Z_HOME_SWITCH))
        {
          motorZ_1.step(DIR_LEFT); 
          motorZ_1.step(stepSpeed);
          motorZ_2.step(DIR_RIGHT); 
          motorZ_2.step(stepSpeed); 
        }
        // if x and y motor is at their home positions update axis positions to 0,
        // and switch to away state
        if (!digitalRead(X_HOME_SWITCH) && !digitalRead(Y_HOME_SWITCH) && !digitalRead(Z_HOME_SWITCH))
        {
          currentPos.x = 0;
          currentPos.y = 0;
          currentPos.z = 0; 
          state = FIND_AREA;
          Serial.println("reached home");
        }
        break;


      // motors run until they reach their away positions(x,y)
      case FIND_AREA:

        // run x-axis motor and count steps until away position is reached
        if (digitalRead(X_AWAY_SWITCH))
        {
          motorX.step(DIR_RIGHT);
          motorX.setSpeed(100); //low speed for higher accuracy 
          currentPos.x++;
        }

        // run y-axis motor and count stepil away position is reached
        if (digitalRead(Y_AWAY_SWITCH))
        {
          motorY.step(DIR_RIGHT);
          motorY.setSpeed(101);
          currentPos.y++;
        }

        if (digitalRead(Z_AWAY_SWITCH)) 
        {
          motorZ_1.step(DIR_RIGHT); 
          motorZ_1.setSpeed(100); 
          motorZ_2.step(DIR_LEFT); 
          motorZ_2.setSpeed(100); 
          
        }

        // if x and y motor is at their away positions set calibration variable to true
        if (!digitalRead(X_AWAY_SWITCH) && !digitalRead(Y_AWAY_SWITCH) && !digitalRead(Z_AWAY_SWITCH))
        {
          calibrated = true;
          Serial.println("reached away");
          
        }
        break;
    }
  }
}
