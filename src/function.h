#include <Arduino.h>

void clientSetup();
void clientLoop(String tempString);
void setupDS1621();
void setThresh(byte reg, int tC);
String getTemperature();