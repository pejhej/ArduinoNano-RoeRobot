///**
//   Request event is called when MASTER sends READ command
//   Should handle all the cases where the master wants information
//   - Calibration parameter
//   - Current status
//*/
//
///**
//   Request event is called when MASTER sends READ command
//   Should handle all the cases where the master wants information
//   - Calibration parameter
//   - Current status
//*/
//void requestEvent() {
//  Serial.println("Request event(R)");
//  
//  //Set the default nr of bytes
//  int nrOfBytes = 3;
//  byte response[nrOfBytes];
//
//  for (int i = 0; i < nrOfBytes; i++)
//    response[i] = -1;
//
//    
//  Serial.print("State: ");
//  Serial.println(inState);
//
//
//  Serial.print("Req inc: ");
//  Serial.println(reqCommand);
//
//
//
//  //--------------------------------------------------------------------------------------------------
//  //Answer the master with what has been asked in Request Command
//  switch (reqCommand)
//  {
//    case retrieveState:
//      Serial.print("RequestState recognized: ");
//      response[0] = inState;
//
//      Serial.print("Response: ");
//      for (byte i = 0; i < sizeof(response); i++)
//      {
//        Serial.println(response[i]);
//      }
//   
//      break;
//
//  //---------------------------------------------
//    //Send the calibrated parameters
//    case calibParam:
//      Serial.println("CalibParam recognized");
//      //Nr of bytes in this response
//      //nrOfBytes = 5;
//      //response[nrOfBytes];
//      inState = parameter;                //Set the response state to "Parameters"
//      response[0] = inState;              //Add state to respsonse
//      
//      //Add the values
//      response[1] =  *((uint8_t*) & (maxPos.zRight) + 1); //high byte (0x12)
//      response[2] =  *((uint8_t*) & (maxPos.zRight) + 0); 
//
//      
//      Serial.println("Response: ");
//      
//      for (byte i = 0; i < nrOfBytes; i++)
//      {
//        Serial.println(response[i]);
//      }
// 
//      break;
//      default:
//      Serial.println("No recognized request command");
//  }
//
//  //--------------------------------------------------------------------------------------------------
//
//
//    
//   Wire.write(response, sizeof(response));
//   Wire.flush();
//  Serial.println("Done with request");
//}
//
//
//
//
//
//
//    /*      Register address | Device ID (High/Low) | Value     
//Betegnelse  Byte 1           | Byte 2               | Byte 3 | Function
//-S3.1      | 0x01            |  Bit nr 1            | 0 R
//*/

