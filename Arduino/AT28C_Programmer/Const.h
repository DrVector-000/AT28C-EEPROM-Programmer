/*  
  AT28C_Programmer.ino - Programmatore EEPROM AT28C
  Copyright (C) 2023 DrVector
  
  Dichiarazione costanti
*/

#include <Arduino.h>

//******************************************************************************************************************//
//* Pins pilota Shift Register 74HC595
//******************************************************************************************************************//
// Pin SRCLK del SN74HC595
// Shift Register Clock - INPUT
const int SN_SRCLK_PIN = 12;
// Pin SER del SN74HC595
// Serial Data In - INPUT
const int SN_SER_PIN = 10;
// Pin RCLK del SN74HC595
// Storage Register Clock - INPUT
const int SN_RCLK_PIN = 11;

//******************************************************************************************************************//
//* Pins pilota Parallel EEPROM AT28C
//******************************************************************************************************************//
// Pin WE della EEPROM
const int EEPROM_WE_PIN = A0;
// Pin OE della EEPROM
//const int EEPROM_OE_PIN = A3;
const int EEPROM_OE_PIN = A1;
// Pin CE della EEPROM
const int EEPROM_CE_PIN = A2;

const int dataPins[] = 
{
    2, 3, 4, 5, 6, 7, 8, 9
};