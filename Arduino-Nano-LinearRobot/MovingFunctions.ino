/**
   Slope of a line can be expressed as deltaX/deltaY. A number of steps on Y equals a number of steps on X.
   This will help us step along Y and adds deltaX to a counter, when the counter >=Y steps a number on X.'
   The motors will toggle simultaniously to the newX and newY. 
*/

boolean moveStep(int newX, int newY) {
  int s = 0;
  float calcSteep = 0;
  int deltaX = newX - currentPos.x; //distance to move
  int deltaY = newY - currentPos.y;

  //Return bool to indicate if the steppers still need to move to reach position
  boolean stillMoving = false;


  //Serial.println("Current x pos " + currentPos.x);

  //Check if X should move at all
  if (deltaX != 0 && deltaY != 0)
  {
    //Check if x should move more then Y
    if (deltaX > deltaY)
    {
      calcSteep = deltaX / deltaY;
      
     s = round(calcSteep);
     
      //Do the moving
      motorX.step(dirX()*s); //onestep and direction
      motorY.step(dirY()); //onestep and direction

       //Increment currentpos
      currentPos.x = (currentPos.x + (dirX()*s));
      currentPos.y = (currentPos.y + dirY());
    }

    //Check if Y should move more then X
    else if (deltaY > deltaX) {
      
      calcSteep = deltaY / deltaX; 
      s = round(calcSteep);
       //Move the steppers
      motorY.step(dirY()*s); //onestep and direction
      motorX.step(dirX());

       //Increment currentpos
      currentPos.x = (currentPos.x + dirX());
      currentPos.y = (currentPos.y + (dirY()*s));
    }

    //Set the return bool to indicate further actions are required
    stillMoving = true;
  }
  //IF one of the steppers have reached position
  //Check if one of the steppers still need to move
  //Check stepper X
  else if (deltaX != 0)
  {

    motorX.step(dirX()*deltaX); //onestep and direction
    currentPos.x = (currentPos.x + dirX()*deltaX);
  }
  //Check if stepper Y needs to move
  else if (deltaY != 0)
  {
    motorX.step(dirY()*deltaY); //onestep and direction
     currentPos.y = (currentPos.y + dirY()*deltaY);
  }



  //Return the bool which indicated if position is reached or not
  return stillMoving;


}

/******************************************************/


