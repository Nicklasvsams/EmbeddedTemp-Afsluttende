#include <Arduino.h>

void clientSetup();
void clientLoop(String tempString);
void setupDS1621();
void setThreshold(int HTemp, int LTemp);
String getTemperature();