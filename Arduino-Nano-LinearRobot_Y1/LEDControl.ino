
void ledDiscoShow()
{
  //
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  uint8_t colorIndex = startIndex;
  // Pre generated show for led strips.
  CRGBPalette16 currentPalette = RainbowColors_p;

  // Brightness for the ledds.
  uint8_t brightness = 30;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, LINEARBLEND);
    colorIndex += 3;
  }
}

// Chnage the color off all leds to the given spectre
void setLedColor(short red, short green, short blue) {
  for (int i = 0; i <= NUM_LEDS; i++) {
    leds[i] = CRGB (red, green, blue);
  }
  if (debug) {
    Serial.println("Led color changed to");
    Serial.print("Red:  ");
    Serial.println(red);
    Serial.print("Green:  ");
    Serial.println(green);
    Serial.print("Blue:  ");
    Serial.println(blue);
  }
  FastLED.show();
}




