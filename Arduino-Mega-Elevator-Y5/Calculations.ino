
/*Different calculations for the RoeBot*/

int calculateDistance(int X0, int X1, int Y0, int Y1) {
  long calculatedDistance;
  int deltaX = X1 - X0;
  int deltaY = Y1 - Y0;
  calculatedDistance = sqrt(sq(deltaX) + sq(deltaY));
  return calculatedDistance;
}
/******************************************************/
int calculateQuotient(int X0, int X1, int Y0, int Y1) {
  long calculatedQuotient;
  int deltaX = X1 - X0;
  int deltaY = Y1 - Y0;
  calculatedQuotient = (deltaX / deltaY);
  return calculatedQuotient;
}


/** Calculates mm input to step output for given axis.
*/
int MMToSteps(float millimeter) {
  int steps = 0;
  steps = STEP_PER_MM * millimeter;
  return steps;
}

/** Calculates step input to mm output for given axis.
*/
float stepsToMM(int steps) {
  float mm = 0;
  mm = (steps / STEP_PER_MM);
  return mm;
}



/******************************************************/
//Calculate from steps to mm
float stepsToMMCalculation(int stepsPerRev, float driverMicrosteps, int belt_pitch, int pulley_teeth) {
  float result;
  result = (stepsPerRev * driverMicrosteps) / (belt_pitch * pulley_teeth);
  return result;
}
//Calcuate from mm to steps
float MMtoStepsCalculation(int belt_pitch, int pulley_teeth, int stepsPerRev, float driverMicrosteps) {
  float result;
  result = (belt_pitch * pulley_teeth) / (stepsPerRev * driverMicrosteps);
  return result;
}


/******************************************************/
//Can be used to decide the direction of the wanted motors

//DIRECTION TO MOVE IN Z
/*
   Direction to move for "LEFT" motor
*/
int dirZLeft(int newPos) {
  int directionZ;
  if (currentPos.zLeft > newPos)
  {
    directionZ = LEFT_DIR_DOWN;
  }
  else
  {
    directionZ = LEFT_DIR_UP;
  }
  return directionZ;
}
/**
   Direction to move for "RIGHT" motor
*/
int dirZRight(int newPos) {

  int directionZ;
  if (currentPos.zRight < newPos)
  {
    directionZ = RIGHT_DIR_DOWN;
  }
  else
  {
    directionZ = RIGHT_DIR_UP;
  }

  return directionZ;
}


/******************************************************/



