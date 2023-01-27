#include <Arduino.h>
#include "function.h"

/**
 * Setup af konfigurationer.
 * Starter Serial forbindelse med en baudrate på 9600.
 */
void setup(){
  Serial.begin(9600);
  clientSetup();
  setupDS1621();
}

/**
 * Det primære loop, der kontinuerligt kører på Arduino Mega 2560
*/
void loop()
{
  clientLoop(getTemperature());
  delay(1000);
}