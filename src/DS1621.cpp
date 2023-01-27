#include <wire.h>
#include <Arduino.h>
#include "function.h"

#define DS1621_Address 0x48 /** DS1621 Konfigurations register */
#define ACCESS_TL 0xA2 /** Lav temperatur termostat register */ 
#define ACCESS_TH 0xA1 /** Høj Temperature termostat register */


/**
 * Laver standard opsætning af DS1621 Digital Temperatur og Termostat.
 * Ved opstart er konfigurationen sat til at kontinuerligt tjekke temperatur, 
 * og sætter en øvre grænse for TOut termostaten til 26 grader og 24 grader på den nedre grænse.
 */
void setupDS1621(){
  Wire.begin();                           /** Forbinder til I2C bus */
  setThreshold(26, 24);                      /** Sætter temperatur max og min grænse */         
  Wire.beginTransmission(DS1621_Address); /** Forbind til DS1621 */
  Wire.write(0xAC);                       /** Send konfiguration register addressen (Access Config) */
  Wire.write(0);                         
  Wire.beginTransmission(DS1621_Address); 
  Wire.write(0xEE);                       /** Sørger for at der bliver sendt kontinuerlig opdatering af temperatur. */
  Wire.endTransmission();                 /** Lukker forbindelsen. */
}

/**
 * Funktion der læser den nuværende temperatur på DS1621, og derefter konverterer
 * værdien fra bytes til et menneskeligt læsbart format. 
 * Til sidst konverteres dette char array til en String som den returnerer.
 */
String getTemperature(){
  Wire.beginTransmission(DS1621_Address);
  Wire.write(0xAA);
  Wire.endTransmission(false);

  // Anmod om 2 bytes fra DS162 and frigiv I2C bus
  Wire.requestFrom(DS1621_Address, 2); 
  uint8_t t_msb = Wire.read();
  uint8_t t_lsb = Wire.read();

  // Udregner temperatur (Raw value)
  int16_t raw_t = (int8_t)t_msb << 1 | t_lsb >> 7;

  // Konverter temperatur til °C
  int16_t c_temp = raw_t * 10 / 2;

  char c_buffer[8];

  // Formatterer værdien til et Char array
  if (c_temp < 0)
  {                      
    c_temp = abs(c_temp);
    sprintf(c_buffer, "-%02u.%1uC", c_temp / 10, c_temp % 10);
  }
  else
  {
    if (c_temp >= 1000)
      sprintf(c_buffer, "%03u.%1uC", c_temp / 10, c_temp % 10);
    else
      sprintf(c_buffer, " %02u.%1uC", c_temp / 10, c_temp % 10);
  }
  
  // Caster til String og returnerer værdien.
  return (String)c_buffer;
}

/**
 * Funktion der sætter øvre eller nedre grænse for termostaten.
 *  @param HTemp Den temperatur den øvre grænse skal sættes til
 *  @param LTemp Den temperatur den nedre grænse skal sættes til
 */
void setThreshold(int HTemp, int LTemp)
{
    Wire.beginTransmission(DS1621_Address);
    
    // Adgang til konfiguration af øvre termostat grænse (0xA1)
    Wire.write(ACCESS_TH);

    // Sender byte værdien af temperatur parameteren
    Wire.write(byte(HTemp));
    Wire.write(0);
    Wire.endTransmission();
    delay(15);

    Wire.beginTransmission(DS1621_Address);
    
    // Adgang til konfiguration af nedre termostat grænse (0xA2)
    Wire.write(ACCESS_TL);

    // Sender byte værdien af temperatur parameteren
    Wire.write(byte(LTemp));
    Wire.write(0);
    Wire.endTransmission();
    delay(15);
}