#include <Arduino.h>
#include <wire.h>
#define DS1621_Address 0x48

#define ACCESS_TL 0xA2 // access low temperature register
#define ACCESS_TH 0xA1 // access high temperature register
void setThresh(byte reg, int tC);

void setup()
{
  Wire.begin(); // join i2c bus
  setThresh(ACCESS_TH, 26);               // high temp threshold = 80F
  setThresh(ACCESS_TL, 24);               // low temp threshold = 75F                   
  Wire.beginTransmission(DS1621_Address); // connect to DS1621 (send DS1621 address)
  Wire.write(0xAC);                       // send configuration register address (Access Config)
  Wire.write(0);                          // perform continuous conversion
  Wire.beginTransmission(DS1621_Address); // send repeated start condition                // Temp
  Wire.write(0xEE);                       // send start temperature conversion command    // Temp
  Wire.endTransmission();
  Serial.begin(9600);
  Serial.println("Starting up :)");
}

char c_buffer[8];

void loop()
{
  delay(1000); // wait a second

  Wire.beginTransmission(DS1621_Address); // connect to DS1621 (send DS1621 address)
  // Wire.write(0xEE); // TEMP
  Wire.write(0xAA); // read temperature command
  // Wire.write(0x22); // TEMP
  Wire.endTransmission(false);         // send repeated start condition
  Wire.requestFrom(DS1621_Address, 2); // request 2 bytes from DS1621 and release I2C bus at end of reading
  uint8_t t_msb = Wire.read();         // read temperature MSB register
  uint8_t t_lsb = Wire.read();         // read temperature LSB register

  // calculate full temperature (raw value)
  int16_t raw_t = (int8_t)t_msb << 1 | t_lsb >> 7;
  // convert raw temperature value to tenths °C
  raw_t = raw_t * 10 / 2;

  // get temperature in tenths °C
  int16_t c_temp = raw_t;
  if (c_temp < 0)
  {                       // if temperature < 0 °C
    c_temp = abs(c_temp); // absolute value
    sprintf(c_buffer, "-%02u.%1uC", c_temp / 10, c_temp % 10);
  }
  else
  {
    if (c_temp >= 1000) // if temperature >= 100.0 °C
      sprintf(c_buffer, "%03u.%1uC", c_temp / 10, c_temp % 10);
    else
      sprintf(c_buffer, " %02u.%1uC", c_temp / 10, c_temp % 10);
  }
  Serial.println(c_buffer);
}

void setThresh(byte reg, int tC)
{
  if (reg == ACCESS_TL || reg == ACCESS_TH)
  {
    Wire.beginTransmission(DS1621_Address);
    Wire.write(reg);      // select temperature reg
    Wire.write(byte(tC)); // set threshold
    Wire.write(0);        // clear fractional bit
    Wire.endTransmission();
    delay(15);
  } 
}