//
///*  Will move the robot to given pos.
//    Inn parameter is new position x and y defined is stepps.
//    Returns true if it has moved to new pos sucsessfully
//    False if it did not reach the destination.
//*/
//boolean moveTo(int newX , int newY) {
//  int slope = 0;
//  float calcSteep = 0;
//  int deltaX = newX - currentPos.x; //distance to move
//  int deltaY = newY - currentPos.y;
//  if (debug) {
//    Serial.print(F("Delta X: "));
//    Serial.print(deltaX);
//    Serial.print(F("Delta Y: "));
//    Serial.print(deltaY);
//  }
//  enum moveState {equalXY, moreXThenY, moreYThenX, still, fail};
//  enum moveState moveState = still;
//
//  if (deltaX == 0 && deltaY == 0) {
//    moveState = still;
//  } else if (deltaX > deltaY) {
//    moveState = moreXThenY;
//  } else if (deltaX == deltaY) {
//    moveState = equalXY;
//  }
//
//
//
//  //Return bool to indicate if the steppers still need to move to reach position
//  boolean moving = true;
//
//  /**** CALCULATION ****/
//
//  switch (moveState) {
//
//    // Check of movement requierd, Else keep still.
//    case still:
//      if (debug) {
//        Serial.println(F("Moving: STILL"));
//        break;
//
//      // If X and Y should move the same length.
//      case equalXY:
//        //Enable motors
//        enableMotors();
//        if (debug) {
//          Serial.println(F("Moving: EQUAL X AND Y MOVEMENT"));
//        }
//        // MOVE X
//        for (int i = 0; ((i < deltaX) && moving); i++)
//        {
//          //Check if moving in wanted direction is possible
//          if (moveXPossible(dirX()))
//          {
//            moveXMotor(dirX()); //onestep and direction
//            if (debug) {
//              Serial.print("Current X : ");
//              Serial.println(currentPos.x);
//            }
//          } else {
//            moving = false;
//          }
//          if (moveYPossible(dirY()))
//          {
//            moveYMotor(dirY()); //onestep and direction
//            if (debug) {
//              Serial.print("Current Y : ");
//              Serial.println(currentPos.y);
//            }
//          } else {
//            moving = false;
//          }
//        }
//        break;
//
//
//      // If X should move more than Y
//      case moreXThenY:
//        //Enable motors
//        enableMotors();
//        if (debug) {
//          Serial.println(F("Moving: Move X more than Y "));
//        }
//        slope = deltaX / deltaY;
//        //Round up the step
//        slope = round(slope);
//        //MOVE X
//        for (int moves = 0; ((moves < deltaY) && moving); moves++)
//          for (int i = 0; ((i < abs(dirX()*slope)) && moving); i++)
//          {
//            //Check if moving in wanted direction is possible
//            if (moveXPossible(dirX()))
//            {
//              moveXMotor(dirX());
//              if (debug) {
//                Serial.print("Current X : ");
//                Serial.println(currentPos.x);
//              }
//            }
//            else
//            {
//              moving = false;
//            }
//          }
//        // MOVE Y
//        //Check if moving in wanted direction is possible
//        if (moveYPossible(dirY()))
//        {
//          moveYMotor(dirY()); //moves one step and increases currentpos
//          if (debug) {
//            Serial.print("Current Y : ");
//            Serial.println(currentPos.y);
//          }
//        }
//        else
//        {
//          moving = false;
//        }
//
//
//        break;
//
//
//
//      // If Y should move more than X
//      case moreYThenX:
//        //Enable motors
//        enableMotors();
//        if (debug) {
//          Serial.println(F("Moving: Move Y more than X "));
//        }
//        break;
//
//
//      // If if the robot did not reach the destination.
//      case fail:
//        if (debug) {
//          Serial.println(F("Moving: FAIL. did not reach the destination."));
//        }
//        break;
//
//      }
//  }
//}
//
//
//
//boolean moveXY(int newX, int xDir, int newY, int yDir) {
//  enableMotors();
//
//  if (!X_HOME_SWITCH && xDir == DEC_DIR)
//  {
//    for (int i = 0; ((i < newX) && !X_HOME_SWITCH); i++) {
//      moveXMotor(xDir);
//    }
//  }
//  if (!X_OUTER_SWITCH && xDir == INC_DIR)
//  {
//    for (int i = 0; ((i < newX) && !X_OUTER_SWITCH); i++) {
//      moveXMotor(xDir);
//    }
//  }
//  disableMotors();
//}
