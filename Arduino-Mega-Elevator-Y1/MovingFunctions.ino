/**
   Slope of a line can be expressed as deltaX/deltaY. A number of steps on Y equals a number of steps on X.
   This will help us step along Y and adds deltaX to a counter, when the counter >=Y steps a number on X.'
   The motors will toggle simultaniously to the newX and newY.
*/

boolean moveStep(Pos newZ) {


  int deltaZ = newZ.zLeft - currentPos.zLeft; //distance to move

  //Return bool to indicate if the steppers still need to move to reach position
  boolean stillMoving = false;


  //Check if X should move at all
  if (deltaZ != 0)
  {

    moveLeftMotor(dirZLeft(newZ.zLeft));
    moveRightMotor(dirZRight(newZ.zRight));
    //Increment currentpos
    //   currentPos.zLeft = (currentPos.zLeft + dirZLeft(newZ.zLeft));
    //  currentPos.zRight = (currentPos.zRight + dirZRight(newZ.zRight));

    //Set the return bool to indicate further actions are required
    stillMoving = true;
  }
  //No moving required
  else
  {
    if (debug == 1)
      Serial.println("Done moving");
  }


  //Return the bool which indicated if position is reached or not
  return stillMoving;

}


//Move the left motor
void  moveLeftMotor(int dir)
{
  motor_left.step(dir);
  motor_left.setSpeed(stepSpeed);
  increaseLeftStep(dir);
}

//Move the right motor
void  moveRightMotor(int dir)
{
  motor_right.step(dir);
  motor_right.setSpeed(stepSpeed);
  increaseRightStep(dir);
}

/**
   Increase the step in left direction with one
*/
void increaseLeftStep(int dir)
{
  currentPos.zLeft = (currentPos.zLeft + dir);
}
void increaseRightStep(int dir)
{
  currentPos.zRight = (currentPos.zRight + dir);
}

/******************************************************/


