

//Function to check if endstops has been reached
void checkEndstops()
{
  //Do checking, if one of sensors is true
  //if(sensor == true)
  //endStopReached = true;
  //else
  //endStopReached = false;
}



//Function to check if encoders are correct
void checkEncoderSync()
{
  //Do checking, on the encoders
  //if(sensor == true)
  //endStopReached = true;
}



void resizeArray(byte *oldArray)
{
  int i = 0;
  int cnt = 0;
  for(i = 0; cnt<sizeof(oldArray); ++i)
    if(oldArray[i] != -1)
    ++cnt;
}

