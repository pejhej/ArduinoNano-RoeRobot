/*Folder responseble for sending data over an serial communication. */


// Sends one byte over and serial communication using string. 
// Uses devise name in dront and devide the byte witn an , as delimiter
void sendOneByte(                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       byte byteToSend){
  String stringToSend = DEVICE_NAME;
  stringToSend = stringToSend + " , "; 
  stringToSend = stringToSend + (int)byteToSend;
  if(debug){
    Serial.println(stringToSend); 
  }
}



// Send a string.

