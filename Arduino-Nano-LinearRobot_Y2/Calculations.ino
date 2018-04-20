
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
/******************************************************/
float stepsToMMCalculation(int stepsPerRev, float driverMicrosteps, int belt_pitch, int pulley_teeth) {
  float result;
  result = (stepsPerRev * driverMicrosteps) / (belt_pitch * pulley_teeth);
  return result;
}

float MMtoStepsCalculation(int belt_pitch, int pulley_teeth, int stepsPerRev, float driverMicrosteps) {
  float result;
  result = (belt_pitch * pulley_teeth) / (stepsPerRev * driverMicrosteps);
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




