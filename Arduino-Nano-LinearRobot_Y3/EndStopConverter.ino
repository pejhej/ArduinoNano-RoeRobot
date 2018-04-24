// Converts an analog innput value to an bool.
// To be used for digital switches connected to analog inputs. (Analog to digital converter) .
bool ATDConverter(short value) {
  bool val = false;
  if (analogRead(value) >= 512) {
    val = true;
  } else {
    val = false;
  }
  return val;
}

// Prints the status og all endstops.
void printEndstopsString() {
  Serial.print("Magnet Endstop:  ");
  Serial.println(ATDConverter(MAGNET_ENDSTOP));
  Serial.print("X home:  ");
  Serial.println(ATDConverter(X_HOME_SWITCH));
  Serial.print("X Away:  ");
  Serial.println(ATDConverter(X_OUTER_SWITCH));
  Serial.print("Y home:  ");
  Serial.println(ATDConverter(Y_HOME_SWITCH));
  Serial.print("Y Away:  ");
  Serial.println(ATDConverter(Y_OUTER_SWITCH));

}

