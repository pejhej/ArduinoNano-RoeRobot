
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
*/
void addToTrayReg(Tray addTray)
{
  if (trayReg.nrOfTrays == 0)
  {
    addTray.trayNumber = 1;
    trayReg.nrOfTrays = 1;
    trayReg.trayList[0] = addTray;
  }
  else
  {
    int newTotal = (trayReg.nrOfTrays + 1);
    trayReg.nrOfTrays = (trayReg.nrOfTrays + 1);
    addTray.trayNumber = newTotal;
    trayReg.trayList[newTotal - 1] = addTray;
  }
}

/**
   Create a tray and add it to the register
*/
void createTrayOnPos()
{
  Tray newTray;
  //Save the current position
  newTray.zPos.zLeft = currentPos.zLeft;
  newTray.zPos.zRight = currentPos.zRight;

  //Add a new tray to the register
  addToTrayReg(newTray);
}


/**
   Check failure and return the given failure state
*/
state checkFailure()
{
  state returnState = fail;

  if (leftBottomSwitch)
  {
    inState = linearBotLimitTrigged;
  }
  if (leftTopSwitch)
  {
    inState = linearBotLimitTrigged;
  }
  if (rightBottomSwitch)
  {
    inState = linearBotLimitTrigged;
  }
  if (rightTopSwitch)
  {
    inState = linearBotLimitTrigged;
  }
  //Safety barrier
  if (topSafetyBarrier)
  {
    inState = safetySwitchUpper;
  }
  if (bottomSafetyBarrier)
  {
    inState = safetySwitchLower;
  }

  return returnState;
}

