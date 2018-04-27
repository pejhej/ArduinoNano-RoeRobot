


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















