/**
   FUNCTIONS TO CONTROL THE MOTORS
*/


void enableMotors()
{
  digitalWrite(LEFT_MOTOR_ENABLE, HIGH);
  digitalWrite(RIGHT_MOTOR_ENABLE, HIGH);
}

void disableMotors()
{
  digitalWrite(LEFT_MOTOR_ENABLE, LOW);
  digitalWrite(RIGHT_MOTOR_ENABLE, LOW);
}

//Turn the brakes on
void turnOnBrake()
{
  digitalWrite(MOTOR_BREAK, LOW);
  brake = true;
}

//Turn the brakes off
void turnOffBrake()
{
  digitalWrite(MOTOR_BREAK, HIGH);
  brake = false;
}

// Turns on brake, waits given time before disable motors.
void turnOnBrakeDisableMotors() {
  turnOnBrake();
  delay(10);
  disableMotors();
}

// Enables motor waits given time before turning of motors.
void turnOnMotorsDisableBrakes() {
  enableMotors();
  turnOffBrake();
}





