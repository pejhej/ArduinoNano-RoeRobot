#include <Stepper.h>


#define MOTOR_ENABLE 8
#define X_MOTOR_STEP 2
#define X_MOTOR_DIR 5
#define X_HOME_SWITCH 12
#define X_AWAY_SWITCH 9
#define Y_MOTOR_STEP 3
#define Y_MOTOR_DIR 6
#define Y_HOME_SWITCH 10
#define Y_AWAY_SWITCH 11
#define DIR_LEFT -1
#define DIR_RIGHT 1

#define STEP_PER_REVOLUTION 400
#define DRIVER_MICROSTEPS 0.25
#define BELT_PRESENT_PITCH 3 //mm 
#define PULLEY_TOOTH 20 

float stepPerMM = 0; 
bool cal = true;
int stepSpeed = 450;
int travelledDistance; 

typedef struct Pos{
  int x;
  int y;
 }Pos;
 
Pos currentPos;
Pos maxPos;
Pos newPos;

Stepper motorX(STEP_PER_REVOLUTION, X_MOTOR_STEP, X_MOTOR_DIR);
Stepper motorY(STEP_PER_REVOLUTION, Y_MOTOR_STEP, Y_MOTOR_DIR);

void setup() {
  Serial.begin(9600);
  pinMode(X_HOME_SWITCH, INPUT_PULLUP);
  pinMode(Y_HOME_SWITCH, INPUT_PULLUP);
  pinMode(Z_HOME_SWITCH, INPUT_PULLUP);
  pinMode(X_AWAY_SWITCH, INPUT_PULLUP);
  pinMode(Y_AWAY_SWITCH, INPUT_PULLUP); 
  pinMode(Z_AWAY_SWITCH, INPUT_PULLUP);
  pinMode(MOTOR_ENABLE, HIGH);
  stepPerMM = stepsToMMCalculation(STEP_PER_REVOLUTION, DRIVER_MICROSTEPS, BELT_PRESENT_PITCH, PULLEY_TOOTH);


  currentPos.x = 0;
  currentPos.y = 0;
  maxPos.x = 5000;
  maxPos.y = 3000;
  
  newPos.y = 0;
  newPos.x = 0;
}

void loop()
{
  if (!cal)
  { 
    calibration();
    cal = true;
  }
  
  newPos.x = 50;
  newPos.y = 75;
  

  
  while(moveStep(newPos.x,newPos.y))
  {
      Serial.println("Trying move");
    //Serial.println("Current x pos " + currentPos.x);
    Serial.println(currentPos.x);
    Serial.println(currentPos.y);
    //Serial.println("Current y pos " + currentPos.y);
    delay(100);
  }

//  Serial.print("travelled distance x direction: ");
//  Serial.print(maxPosXinMM); //xCoordinate
//  Serial.println("travelled distance y direction:  ");
//  Serial.print(maxPosYinMM); //yCoordinate
}




