
/*Different calculations for the RoeBot*/

int calculateDistance(int X0, int X1, int Y0, int Y1) {
  long calculatedDistance;
  int deltaX = X1 - X0;
  int deltaY = Y1 - Y0;
  calculatedDistance = sqrt(sq(deltaX) + sq(deltaY));
  return calculatedDistance;
}
/******************************************************/
int calculateQuotient(int X0, int X1, int Y0, int Y1, int Z0, int Z1) {
  long calculatedQuotient;
  int deltaX = X1 - X0;
  int deltaY = Y1 - Y0;
  //int deltaZ = Z1-Z0;
  calculatedQuotient = (deltaX / deltaY);
  return calculatedQuotient;
}

/** Calculates mm input to step output for given axis.
*/
int MMToSteps(float millimeter, int axis) {
  int steps = 0;
  if (axis == X_AXIS) {
    steps = X_STEP_PER_MM * millimeter;
  } else if (axis == Y_AXIS) {
    steps = Y_STEP_PER_MM * millimeter;
  }
  return steps;
}

/** Calculates step input to mm output for given axis.
*/
float stepsToMM(int steps, int axis) {
  float mm = 0;
  if (axis == X_AXIS) {
    mm = (steps / X_STEP_PER_MM) ;
  } else if (axis == Y_AXIS) {
    mm = (steps / Y_STEP_PER_MM);
  }
  return mm;
}


/******************************************************/
float stepsPerMM(int stepsPerRev, float driverMicrosteps, int belt_pitch, int pulley_teeth) {
  float result;
  result = ((stepsPerRev * driverMicrosteps) / (belt_pitch * pulley_teeth));
  return result;
}

float MMPerSteps(int belt_pitch, int pulley_teeth, int stepsPerRev, float driverMicrosteps) {
  float result;
  result = ((belt_pitch * pulley_teeth) / (stepsPerRev * driverMicrosteps));
  return result;
}
/******************************************************/
//Find the direction for x movement
int dirX() {
  int directionX;

  if (currentPos.x > newPos.x)
  {
    directionX = DEC_DIR;
  }
  else if (currentPos.x < newPos.x)
  {
    directionX = INC_DIR;
  }
  else if (currentPos.x == newPos.x)
  {
    directionX = 0;
  }
  return directionX;
}

/******************************************************/
//Find the direction for y movement
int dirY() {
  int directionY;
  if (currentPos.y > newPos.y)
  {
    directionY = DEC_DIR;
  }
  else if (currentPos.y < newPos.y)
  {
    directionY = INC_DIR;
  }
  else if (currentPos.y == newPos.y)
  {
    directionY = 0;
  }
  return directionY;
}




