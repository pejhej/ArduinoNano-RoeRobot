


/**
    This function is called when one of the limit switches are triggered
*/
/*
  void leftBottomLimitTrig()
  {
  interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
     leftBottomSwitch = toggleBool(leftBottomSwitch);
  }
  last_interrupt_time = interrupt_time;


  }


  void leftTopLimitTrig()
  {
  interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 500)
  {
     leftTopSwitch = toggleBool(leftTopSwitch);
  }
  last_interrupt_time = interrupt_time;

  }

  void rightBottomLimitTrig()
  {
  //Set the interrupt time
  interrupt_time = millis();

  if(interrupt_time - last_interrupt_time > 500)
  {
     rightBottomSwitch = toggleBool(rightBottomSwitch);
  }

  //Set the last called interrupt time
  last_interrupt_time = interrupt_time;
  }



  void rightTopAwayLimitTrig()
  {
    //Set the interrupt time
  interrupt_time = millis();

  if(interrupt_time - last_interrupt_time > 500)
  {
     rightTopSwitch = toggleBool(rightTopSwitch);
  }

  //Set the last called interrupt time
  last_interrupt_time = interrupt_time;
  }
*/

//------------------------------------------------------------------------

/**
   Checks the limit switches and sets the appropriate bools
*/



void stopMotors() {
  mainCommand = stopRobot;
}


bool limitTrigged()
{
  bool limitIsTrigged = false;


  if (!digitalRead(LEFT_BOTTOM_SWITCH))
  {
    leftBottomSwitch = true;
    limitIsTrigged = true;
  }
  else
  {
    leftBottomSwitch = false;
  }
  if (!digitalRead(LEFT_TOP_SWITCH))
  { leftTopSwitch = true;
    limitIsTrigged = true;
  }
  else
  {
    leftBottomSwitch = false;
  }
  if (!digitalRead(RIGHT_BOTTOM_SWITCH))
  { rightBottomSwitch = true;
    limitIsTrigged = true;
  }
  else
  {
    leftBottomSwitch = false;
  }
  if (!digitalRead(RIGHT_TOP_SWITCH))
  {
    rightTopSwitch = true;
    limitIsTrigged = true;
  }
  else
  {
    leftBottomSwitch = false;
  }


  return limitIsTrigged;
}


/*
   Return the state of the safety barrier and sets the respective booleans
*/
boolean safetyBarrier()
{
  boolean barrierTrigged = false;


  if (!digitalRead(TOP_IR_RELAY))
  {
    topSafetyBarrier = true;
    barrierTrigged = true;
  }
  else
  {
    if (topSafetyBarrier)
    {
      topSafetyBarrier = false;
    }

  }
  if (!digitalRead(BOTTOM_IR_RELAY))
  {
    bottomSafetyBarrier = true;
    barrierTrigged = true;
  }
  else
  {
    if (bottomSafetyBarrier)
    {
      bottomSafetyBarrier = false;
    }
  }

  return barrierTrigged;
}

/**
   Returns state of top barrier
*/
boolean safetyBarrierTop()
{
  boolean barrierTrigged = false;

  if (!digitalRead(TOP_IR_RELAY))
  {
    topSafetyBarrier = true;
    barrierTrigged = true;
  }
  else
  {
    if (topSafetyBarrier)
    {
      topSafetyBarrier = false;
    }
  }

  return barrierTrigged;
}
/**
   Increase the active barriers
*/
void increaseActiveBarrier()
{
  updateCntTopBarrier();
  updateCntBottomBarrier();
}

/**
   Update the top barrier counter
*/
void updateCntTopBarrier()
{
  if (topSafetyBarrier)
    ++barrierCounterTop;

  else
    barrierCounterTop = 0;
}
/**
   Update the bottom barrier counter
*/
void updateCntBottomBarrier()
{
  if (bottomSafetyBarrier)
    ++barrierCounterBottom;

  else
    barrierCounterBottom = 0;
}


/**
   Returns true if the safety counters are exceeded
*/
boolean counterExceeded()
{
  boolean failure = false;

  //Check the counters
  if ((barrierCounterTop > flagDetectionSteps) || (barrierCounterBottom > flagDetectionSteps))
  {
    failure = true;
  }

  return failure;
}


/**
   Returns state of bottom barrier
*/
boolean safetyBarrierBottom()
{
  boolean barrierTrigged = false;

  if (!digitalRead(BOTTOM_IR_RELAY))
  {
    bottomSafetyBarrier = true;
    barrierTrigged = true;
  }
  else
  {
    if (bottomSafetyBarrier)
    {
      bottomSafetyBarrier = false;
    }
  }
  return barrierTrigged;
}



/**
   Function to check if the new direction is safe
*/
boolean isDirectionSafe(Pos newPos)
{
  //The return bool if direction is safe
  boolean safe = false;

  //Check which switch is activated
  if (leftBottomSwitch)
  {
    //Check new direction of given new pos
    if (dirZLeft(newPos.zLeft) == LEFT_DIR_UP)
    {
      safe = true;
    }
  }

  //Check which switch is activated
  if (rightBottomSwitch)
  {
    //Check new direction of given new pos
    if (dirZRight(newPos.zRight) == RIGHT_DIR_UP)
    {
      safe = true;
    }
  }

  //Check which switch is activated
  if (leftTopSwitch)
  {
    //Check new direction of given new pos
    if (dirZLeft(newPos.zLeft) == LEFT_DIR_DOWN)
    {
      safe = true;
    }
  }

  //Check which switch is activated
  if (rightTopSwitch)
  {
    //Check new direction of given new pos
    if (dirZRight(newPos.zRight) == RIGHT_DIR_DOWN)
    {
      safe = true;
    }
  }


  return safe;

}







/**
   Toggle the bool given to this function
*/
boolean toggleBool(boolean toggleThis)
{

  //Check if its high
  if (toggleThis)
  {
    toggleThis = false;
  }
  else if (!toggleThis)
  {
    toggleThis = true;
  }

  return toggleThis;
}













/*if (Y_HOME_SWITCH == triggedSwitch)
  {

  }
  else if (Y_AWAY_SWITCH == triggedSwitch)
  {

  }
  else if (X_HOME_SWITCH == triggedSwitch)
  {

  }
  else if (Y_AWAY_SWITCH == triggedSwitch)
  {

  }
*/

