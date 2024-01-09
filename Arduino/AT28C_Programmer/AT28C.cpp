/*  
  AT28C_Programmer.ino - Programmatore EEPROM AT28C
  Copyright (C) 2023 DrVector
  
  Gestione EEPROM AT28C
*/

#include <Arduino.h>
#include "Const.h"
#include "SRHelper.h"

//******************************************************************************************************************//
//* Imposta il bus dati in INPUT o OUTPUT
//******************************************************************************************************************//
void setDataBusMode(int mode)
{
	if (mode == INPUT || mode == OUTPUT) {
		for (int i = 0; i < 8; i++) {
			pinMode(dataPins[i], mode);
		}
	}
}

//******************************************************************************************************************//
//* Lettura di un byte all'indirizzo selezionato
//******************************************************************************************************************//
byte readByte(unsigned int address) {

  // Imposta il bus dati in input
  setDataBusMode(INPUT);
  
  digitalWrite(EEPROM_CE_PIN, HIGH);
  digitalWrite(EEPROM_OE_PIN, HIGH);
  digitalWrite(EEPROM_WE_PIN, HIGH);
  delayMicroseconds(1);

  // Imposta indirizzo
  addressWrite(address);
  //Serial.println("address=" + (String)addr);

  digitalWrite(EEPROM_CE_PIN, LOW);
  delayMicroseconds(1);

  digitalWrite(EEPROM_OE_PIN, LOW);
  delayMicroseconds(1);
  //delayMicroseconds(100);

  // Lettura pins D2/D9 (Bus Dati)
  byte bval = 0;
  for (int y = 0; y < 8; y++) {
    bitWrite(bval, y, digitalRead(dataPins[y]));
  }

  digitalWrite(EEPROM_OE_PIN, HIGH);
  delayMicroseconds(1);

  digitalWrite(EEPROM_CE_PIN, HIGH);
  delayMicroseconds(1);

  return bval;
}

//******************************************************************************************************************//
//* Scrittura di un byte all'indirizzo selezionato
//******************************************************************************************************************//
byte writeByte(unsigned int address, byte value)
{
  digitalWrite(EEPROM_CE_PIN, HIGH);
  digitalWrite(EEPROM_OE_PIN, HIGH);
  digitalWrite(EEPROM_WE_PIN, HIGH);
  delayMicroseconds(1);

  // Imposta il bus di dati in output
	setDataBusMode(OUTPUT);

  // Imposta indirizzo
  addressWrite(address);

  // Scrittura pins D2/D9 (Bus Dati)
	for (int i = 0; i < 8; i++)
	{
		//int a = (value & (1 << i)) > 0;
		digitalWrite(dataPins[i], bitRead(value, i));
	}
  delayMicroseconds(1);

  digitalWrite(EEPROM_CE_PIN, LOW);
  delayMicroseconds(1);

  digitalWrite(EEPROM_WE_PIN, LOW);
  delayMicroseconds(1);

  digitalWrite(EEPROM_WE_PIN, HIGH);
  delayMicroseconds(1);

  // Imposta il bus dati in input
  setDataBusMode(INPUT);

  digitalWrite(EEPROM_OE_PIN, LOW);
  delayMicroseconds(1);

  // Attende il termine della scrittura del byte, verificando
  // che il bit 7 corrisponda a quando scritto
  // durante la scrittura il bit 7 è il complemento di quanto inviato
  while (bitRead(value, 7) != digitalRead(dataPins[7])) {
    digitalWrite(EEPROM_OE_PIN, HIGH);
    delayMicroseconds(1);
    delay(1);
    digitalWrite(EEPROM_OE_PIN, LOW);
    delayMicroseconds(1);
  }

  // Lettura pins D2/D9 (Bus Dati)
  byte bval = 0;
  for (int y = 0; y < 8; y++) {
    bitWrite(bval, y, digitalRead(dataPins[y]));
  }

  digitalWrite(EEPROM_OE_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(EEPROM_CE_PIN, HIGH);
  delayMicroseconds(1);

  return bval;
}

//******************************************************************************************************************//
//* Lettura della EEPROM
//******************************************************************************************************************//
void readEEPROM(unsigned int size) {
  //Serial.println("START DUMP (" + (String)size + " byte)");

  unsigned int addr = 0;

  // Azzera lo shift register
  //addressWrite(0x0000);

//  byte buffer[256];
//  int index = 0;
  for (int i = 0; i < size; i++) {
    byte bval = readByte(addr + i);
//    buffer[index] = bval;
//    index ++;
    //Serial.println(bval, DEC);
    
//    if (index == 256) {
//      Serial.write(buffer, 1);
//      index = 0;
//    }
      Serial.write(&bval, 1);
  }
    
  //Serial.println("END DUMP");
}

//******************************************************************************************************************//
//* Scrittura della EEPROM
//******************************************************************************************************************//
void writeEEPROM(unsigned int size)
{
  unsigned int address = 0;  
  //for (unsigned int i = 1; i <= size; i++)
  while (address < size)
  {
    byte val = 0;
    if (Serial.available() > 0) {
      val = Serial.read();
      byte wval = writeByte(address, val);
      Serial.write(&wval, 1);
      address++;
    }    
  }
}

//******************************************************************************************************************//
//* Disabilita Software Data Protection
//******************************************************************************************************************//
void disableSDP()
{
  writeByte(0x5555, 0xaa);
  writeByte(0x2aaa, 0x55);
  writeByte(0x5555, 0x80);
  writeByte(0x5555, 0xaa);
  writeByte(0x2aaa, 0x55);
  writeByte(0x5555, 0x20);
}

//******************************************************************************************************************//
//* Abilita Software Data Protection
//******************************************************************************************************************//
void enableSDP()
{
  writeByte(0x5555, 0xaa);
  writeByte(0x2aaa, 0x55);
  writeByte(0x5555, 0xa0);
}