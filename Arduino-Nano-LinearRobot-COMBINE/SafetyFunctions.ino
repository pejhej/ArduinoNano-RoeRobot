


/**
*   This function is called when one of the limit switches are triggered
*/
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


void xAwayLimitTrig()
{
  interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 500) 
 {
     xAwaySwitch = toggleBool(xAwaySwitch);
 }
 last_interrupt_time = interrupt_time;
  
}

void yHomeLimitTrig()
{
 //Set the interrupt time
  interrupt_time = millis();
  
 if(interrupt_time - last_interrupt_time > 500) 
 {
     yHomeSwitch = toggleBool(yHomeSwitch);
 }
 
 //Set the last called interrupt time
 last_interrupt_time = interrupt_time;
}

volatile yAwayLimitTrig()
{
    //Set the interrupt time
  interrupt_time = millis();
  
 if(interrupt_time - last_interrupt_time > 500) 
 {
     yAwaySwitch = toggleBool(yAwaySwitch);
 }
 
 //Set the last called interrupt time
 last_interrupt_time = interrupt_time;
}



/**
 * Toggle the bool given to this function
 */
boolean toggleBool(boolean toggleThis)
{
  
  //Check if its high
  if(toggleThis)
  {
    toggleThis = false;
  }
  else if(!toggleThis)
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
