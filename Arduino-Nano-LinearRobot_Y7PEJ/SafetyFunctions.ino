


/**
    This function is called when one of the limit switches are triggered
*/
/*
  void xHomeLimitTrig()
  {
  interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200)
  {
     xHomeSwitch = toggleBool(xHomeSwitch);
  }
  last_interrupt_time = interrupt_time;
  }
*/



void stopMotors() {
  mainCommand = stopRobot;
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


/**
   Checks the limit switches and sets the appropriate bools
*/
bool limitTrigged()
{
  bool limitIsTrigged = false;


  if (!digitalRead(X_HOME_SWITCH))
  {
    xHomeSwitch = true;
    limitIsTrigged = true;
  }
  else
  {
    xHomeSwitch = false;
  }
  if (!digitalRead(X_OUTER_SWITCH))
  { xAwaySwitch = true;
    limitIsTrigged = true;
  }
  else
  {
    xAwaySwitch = false;
  }
  if (!digitalRead(Y_HOME_SWITCH))
  { yHomeSwitch = true;
    limitIsTrigged = true;
  }
  else
  {
    yHomeSwitch = false;
  }
  if (!digitalRead(Y_OUTER_SWITCH))
  {
    yAwaySwitch = true;
    limitIsTrigged = true;
  }
  else
  {
    yAwaySwitch = false;
  }


  return limitIsTrigged;
}





