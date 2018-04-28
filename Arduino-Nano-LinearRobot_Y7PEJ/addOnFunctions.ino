



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

/**
 * Update the status to given status and notify over serial com
 */
void updateStatusAndNotify(byte state) {
  int readyCounter = 1;
  if (inState != state) {
    inState = state;
    sendInt(inState);
    //Counter for making sure the controller updates the ready to recieve status - send it multiple times
    if(state == readyToRecieve)
    {
      for(int i=0; i<readyCounter; ++i)
      {
        sendInt(inState);
      }
    }
  }
}















