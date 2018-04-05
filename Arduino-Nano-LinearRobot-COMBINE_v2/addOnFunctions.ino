
/**
   Add the Tray given in parameter to the Tray Register
*/
/*
  void addToRegister(Tray addTray)
  {
  int sizeOfReg = trayReg.nrOfTrays;
  int newSize = (sizeOfReg+1);
  //Copy trays to a new register with new array
  TrayRegister newRegTray;
  newRegTray.nrOfTrays = newSize;
  //Create new array
  newRegTray.trayList[newSize];

  addTray.trayNumber = newSize;

  //Copy the existing trays
  for(int i=0; i<sizeOfReg; ++i)
  {
   newRegTray.trayList[i] = trayReg.trayList[i];
  }
  //Add the new tray
  newRegTray.trayList[newSize] = addTray;

  trayReg = newRegTray;
  }
*/
/*
   Add a new tray to the register



  /**
   Check failure and return the given failure state
*/
state checkFailure()
{
  state returnState = fail;

  if (xHomeSwitch || xAwaySwitch || yHomeSwitch || yAwaySwitch )
  {
    inState = linearBotLimitTrigged;
  }

  return returnState;
}
/**
   Returns true if the robot is in same position as new-position. Ergo no moving required
*/
boolean robotInPosition()
{
  boolean inPos = false;
  if ((currentPos.x == newPos.x) && (currentPos.y == newPos.y))
  {
    inPos = true;
  }

  return inPos;
}
















