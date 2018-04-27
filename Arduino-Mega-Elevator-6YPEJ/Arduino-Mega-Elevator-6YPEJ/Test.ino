
void printSensors()
{
  Serial.print("Right TOP Sensor: ");
  Serial.println(digitalRead(RIGHT_TOP_SWITCH));
  Serial.print("Right BOTTOM Sensor: ");
  Serial.println(digitalRead(RIGHT_BOTTOM_SWITCH));

  Serial.print("Left TOP Sensor: ");
  Serial.println(digitalRead(LEFT_TOP_SWITCH));
  Serial.print("Left BOTTOM Sensor: ");
  Serial.println(digitalRead(LEFT_BOTTOM_SWITCH));
}


void printLimitTrigged()
{
  Serial.print("Limittrigged ");
  Serial.println(limitTrigged());
}

void resetCurrentPos()
{
  currentPos.zLeft = 0;
  currentPos.zRight = 0;
}

void changeNewPos(int newpos)
{
  newPos.zLeft = newpos;
  newPos.zRight = newpos;
}


void printSafetybarrier()
{
  safetyBarrier();
  Serial.print("Top barrier: ");
  Serial.println(topSafetyBarrier);
  Serial.print("Bottom barrier: ");
  Serial.println(bottomSafetyBarrier);
}

void printSafetyDigitalRead()
{
  Serial.print("Top barrier: ");
  Serial.println(digitalRead(TOP_IR_RELAY));

  Serial.print("Bottom barrier: ");
  Serial.println(digitalRead(BOTTOM_IR_RELAY));
}

void printSteps()
{
  Serial.print("Step: ");
  Serial.print(currentPos.zLeft);
  Serial.print(" ");
  Serial.println(currentPos.zRight);
}


void printTrayReg()
{
  for(int i=0; i < trayReg.nrOfTrays; ++i)
  {
    Tray currTray = trayReg.trayList[i];
    
    Serial.print("TRAY: ");
    Serial.print(currTray.trayNumber);
    Serial.print(". POS: ");
    Serial.println(currTray.zPos.zLeft); 
    
  }
  Serial.print("Nr of Trays: ");
  Serial.println(trayReg.nrOfTrays);
}


void printMaxPos()
{
   Serial.print("Maxpos: ");
    Serial.print("LEFT ");
    Serial.print(maxPos.zLeft); 
    Serial.print(". RIGHT ");
    Serial.println(maxPos.zRight);
}



void moveStep() {

  //states for moving
  const int FIND_BOTTOM = 0;
  const int FIND_TOP = 1;
  bool moving = false;

  //setting state to go to bottom
  int state = FIND_BOTTOM;
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
          Serial.println("reached bottom");
        }
        break;

      //----------
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
          Serial.println("reached top");

        }
        break;

    }
  }
}





void moveStepInterrupt() {

  //states for moving
  const int FIND_BOTTOM = 0;
  const int FIND_TOP = 1;
  bool moving = false;

  //setting state to go to bottom
  int state = FIND_BOTTOM;
  while (!moving)
  {
    //running this state until it it is completed
    switch (state)
    {

      case FIND_BOTTOM:
        //finding (0,0) position, which is bottom
        if (digitalRead(LEFT_BOTTOM_SWITCH))
        {
          motor_left.step(LEFT_DIR_DOWN);
          motor_left.setSpeed(stepSpeed);
        }
        if (digitalRead(RIGHT_BOTTOM_SWITCH))
        {
          motor_right.step(RIGHT_DIR_DOWN);
          motor_right.setSpeed(stepSpeed);
        }

        if (!digitalRead(LEFT_BOTTOM_SWITCH) && !digitalRead(RIGHT_BOTTOM_SWITCH))
        {
          currentPos.zLeft = 0;
          currentPos.zRight = 0;
          state = FIND_TOP;
          
          Serial.println("reached bottom");
        }
        break;

      //----------
      case FIND_TOP:
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
          moving = true;
          Serial.println("reached top");

        }
        break;


    }
  }
}

