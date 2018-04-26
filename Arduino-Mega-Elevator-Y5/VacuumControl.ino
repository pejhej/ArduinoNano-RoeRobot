/* Controlls the vacuum pump */
// Turns on vacumm pump
void turnVacuumOn() {
  digitalWrite(PUMP, HIGH);
}
// Turns off vacumm pump
void turnVacuumOff() {
  digitalWrite(PUMP, LOW);
}

