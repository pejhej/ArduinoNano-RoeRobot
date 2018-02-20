#include <Stepper.h> //stepper library 
#define MAX_buffer (8) //longest message arduino can receive
char buffer[MAX_buffer]; //where the message is stored until it gets called 
int bufferThisMuch; 
const int stepsPerRevolution = 800;  //number of steps per revolution

const int enablePin = 8; 

Stepper myStepper(stepsPerRevolution,2, 5); 

//int stepCount = 0;         // number of steps the motor has taken
//int motorSpeed = 1023; 

void setup() {
  pinMode(enablePin,OUTPUT); 
  Serial.begin(9600); 
  commands(); 
  


/*
  recieve(checkIncommingMsg)
  request()
  

  
  byte moveMotors = 0x50;
 
  byte b = wire.read();
*/
}

void commands() {
  //type all commands here: 
  Serial.print(F("RoeBot")); //F() Arduino-only macro. Put string in program memory only instead of RAM 
  Serial.println(F("Commands:")); 
}



void loop() {
  /* code that works
  digitalWrite(enablePin, LOW); 
  Serial.println("klokkeretning"); 
  myStepper.step(stepsPerRevolution); 
  delay(500); 
*/

if (Serial.available()){ //if something is available 
  char c = Serial.read(); //then get it 
  Serial.print(c); //debugging
  
  //store the byte as long as there is room in the buffer. if the buffer is full some data might get lost
  if(bufferThisMuch < MAX_buffer) buffer[bufferThisMuch++] = c; 
}


}
