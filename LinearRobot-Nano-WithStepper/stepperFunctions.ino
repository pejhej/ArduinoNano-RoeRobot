/**
  method for callibrating the distances in the x and y axes, takes a pointer
   to array as input for returning the number of steps to the call.
*/
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
          motorX.step(left);
          motorX.setSpeed(stepSpeed);
        }

        // run y-axis motor until home position is reached
        if (digitalRead(Y_HOME_SWITCH))
        {
          motorY.step(left);
          motorY.setSpeed(stepSpeed);
        }

        // if x and y motor is at their home positions update axis positions to 0,
        // and switch to away state
        if (!digitalRead(X_HOME_SWITCH) && !digitalRead(Y_HOME_SWITCH))
        {
          xPosition = 0;
          yPosition = 0;
          state = FIND_AREA;
          Serial.println("reached home");
        }
        break;


      // motors run until theu reach their away positions(x,y)
      case FIND_AREA:

        // run x-axis motor and count steps until away position is reached
        if (digitalRead(X_AWAY_SWITCH))
        {
          motorX.step(right);
          motorX.setSpeed(stepSpeed);
          xPosition++;
        }

        // run y-axis motor and count steps until away position is reached
        if (digitalRead(Y_AWAY_SWITCH))
        {
          motorY.step(right);
          motorY.setSpeed(stepSpeed);
          yPosition++;
        }

        // if x and y motor is at their away positions set calibration variable to true
        if (!digitalRead(X_AWAY_SWITCH) && !digitalRead(Y_AWAY_SWITCH))
        {
          calibrated = true;
          Serial.println("reached away");
        }
        break;
    }
  }
}

const int stepsCalculation(int stepsPerRev, float driverMicrosteps, int belt_pitch, int pulley_teeth) {
  int result; 
  result = (stepsPerRev*driverMicrosteps)/(belt_pitch*pulley_teeth);
  return result; 
}
