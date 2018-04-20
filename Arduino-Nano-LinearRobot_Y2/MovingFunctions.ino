/**
   Slope of a line can be expressed as deltaX/deltaY. A number of steps on Y equals a number of steps on X.
   This will help us step along Y and adds deltaX to a counter, when the counter >=Y steps a number on X.'
   The motors will toggle simultaniously to the newX and newY.

   RETURNS: This function returns true if it was able to move in the required directions
*/

boolean moveStep(int newX, int newY) {
  //Enable motors
  enableMotors();
  int s = 0;
  float calcSteep = 0;
  int deltaX = newX - currentPos.x; //distance to move
  int deltaY = newY - currentPos.y;

  //Return bool to indicate if the steppers still need to move to reach position
  boolean moving = true;

  /**** CALCULATION ****/

  //Check if X should move at all
  if (deltaX != 0 && deltaY != 0)
  {

    if (deltaX == deltaY) {
      //Check if moving in wanted direction is possible
      if (moveXPossible(dirX()))
      {
        moveXMotor(dirX()); //onestep and direction
      } else {
        moving = false;
      }
      if (moveYPossible(dirY()))
      {
        moveYMotor(dirY()); //onestep and direction
      } else {
        moving = false;
      }
    }


    //Check if x should move more then Y
    else if (deltaX > deltaY)
    {
      calcSteep = deltaX / deltaY;
      //Round up the step
      s = round(calcSteep);
      /****Do the moving******/
      //MOVE X
      for (int i = 0; ((i < abs(dirX()*s)) && moving); i++)
      {
        //Check if moving in wanted direction is possible
        if (moveXPossible(dirX()))
        {
          moveXMotor(dirX());
        }
        else
        {
          moving = false;
        }
      }
      // MOVE Y
      //Check if moving in wanted direction is possible
      if (moveYPossible(dirY()))
      {
        moveYMotor(dirY()); //moves one step and increases currentpos
        
      }
      else
      {
        moving = false;
      }
    }

    //Check if Y should move more then X
    else if (deltaY > deltaX)
    {
      /**** CALCULATION ****/
      calcSteep = deltaY / deltaX;
      s = round(calcSteep);

      /****Do the moving******/
      //MOVE Y
      for (int i = 0; ((i < abs(dirY()*s)) && moving); i++)
      {
        //Check if moving in wanted direction is possible
        if (moveYPossible(dirY()))
        {
          moveYMotor(dirY());
        }
        else
        {
          moving = false;
        }
      }

      // MOVE X
      //Check if moving in wanted direction is possible
      if (moveXPossible(dirX()))
      {
        moveXMotor(dirX());
      }
      else
      {
        moving = false;
      }
    }
  }


  //IF one of the steppers have reached position
  //Check if one of the steppers still need to move
  //Check stepper X
  if (deltaX != 0 && deltaY == 0)
  {
    //MOVE X
    //Check if moving in wanted direction is possible
    if (moveXPossible(dirX()))
    {
      moveXMotor(dirX());
    }
    else
    {
      moving = false;
    }
  }


  //Check if stepper Y needs to move
  if (deltaY != 0 && deltaX == 0)
  {
    //MOVE Y
    //Check if moving in wanted direction is possible
    if (moveYPossible(dirY()))
    {
      moveYMotor(dirY());
    }
    else
    {
      moving = false;
    }
  }
  //Return the bool which indicated if position is reached or not
  return moving;
}

/******************************************************/

//Move the left motor
/*
   Moves the X motor in the wanted direction, with step speed. And increases the current positoin
*/
void  moveXMotor(int dir)
{
  motorX.step(dir);
  motorX.setSpeed(stepSpeed[X_AXIS]);
  increaseXStep(dir);
}

//Move the right motor
void  moveYMotor(int dir)
{
  motorY.step(dir);
  motorY.setSpeed(stepSpeed[Y_AXIS]);
  increaseYStep(dir);
}


/**
   Increase the step in left direction with one
*/
void increaseXStep(int dir)
{
  currentPos.x = (currentPos.x + dir);
}
void increaseYStep(int dir)
{
  currentPos.y = (currentPos.y + dir);
}




/**
  Return if true if possible to move in the wanted direction
*/
boolean moveXPossible(int dir)
{
  boolean possible = true;

  if (inState == stopped) {
    boolean possible = false;
  } else {
    boolean possible = true;

    //Check if limit switches are activated
    if (limitTrigged())
    {
      //Check the home end stop
      if (xHomeSwitch)
      {
        //Check all the direction, set appropriate bool value.
        //True moving in opposite direction is wanted
        if (dir == DEC_DIR)
        {
          possible = false;
        }
        else if (dir == INC_DIR)
        {
          possible = true;
        }
      }
      //Check the away end stop
      //And determine if moving in the desired direction is possible
      else if (xAwaySwitch)
      {
        if (dir == DEC_DIR)
        {
          possible = true;
        }
        else if (dir == INC_DIR)
        {
          possible = false;
        }
      }
    }
  }

  return possible;
}


/**
  Return if true its possible to move in the wanted direction
*/
boolean moveYPossible(int dir)
{
  boolean possible = true;
  if (inState == stopped) {
    boolean possible = false;
  } else {
    //Check if limit switches are activated
    if (limitTrigged())
    {
      //Check the home end stop
      if (yHomeSwitch)
      {
        //Check all the direction, set appropriate bool value.
        //True moving in opposite direction is wanted
        if (dir == DEC_DIR)
        {
          possible = false;
        }
        else if (dir == INC_DIR)
        {
          possible = true;
        }
      }
      //Check the away end stop
      //And determine if moving in the desired direction is possible
      else if (yAwaySwitch)
      {
        if (dir == DEC_DIR)
        {
          possible = true;
        }
        else if (dir == INC_DIR)
        {
          possible = false;
        }
      }
    }
  }
  return possible;
}




