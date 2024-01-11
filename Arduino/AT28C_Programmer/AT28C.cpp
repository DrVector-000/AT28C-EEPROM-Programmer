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

  // Imposta il bus di dati in output
  setDataBusMode(OUTPUT);

  // Imposta indirizzo
  addressWrite(address);

  // Scrittura pins D2/D9 (Bus Dati)
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(dataPins[i], bitRead(value, i));
  }
  delayMicroseconds(1);

  digitalWrite(EEPROM_CE_PIN, LOW);
  delayMicroseconds(1);

  digitalWrite(EEPROM_WE_PIN, LOW);
  delayMicroseconds(1);

  digitalWrite(EEPROM_WE_PIN, HIGH);
  delayMicroseconds(1);

  digitalWrite(EEPROM_CE_PIN, HIGH);
  delayMicroseconds(1);

  return value;
}

//******************************************************************************************************************//
//* Scrittura di una pagina di 64 byte all'indirizzo selezionato
//******************************************************************************************************************//
byte writePage(unsigned int address, byte* page, unsigned int size)
{
  digitalWrite(EEPROM_CE_PIN, HIGH);
  digitalWrite(EEPROM_OE_PIN, HIGH);
  digitalWrite(EEPROM_WE_PIN, HIGH);

  // Imposta il bus di dati in output
  setDataBusMode(OUTPUT);

  byte value; // valore da scrivere, ultimo valore scritto
  for (unsigned int idx = 0; idx < size; idx++) {
    value = writeByte(address + idx, page[idx]);
  }

  return value;
}

//******************************************************************************************************************//
//* Attende il termine della scrittura di un byte e ne verifica la corretta valorizzazione
//******************************************************************************************************************//
byte waitAndCheckWrite(byte value)
{
  // Imposta il bus dati in input
  setDataBusMode(INPUT);

  digitalWrite(EEPROM_CE_PIN, LOW);
  delayMicroseconds(1);

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
  unsigned int addr = 0;

  for (int i = 0; i < size; i++) {
    byte bval = readByte(addr + i);
    Serial.write(&bval, 1);
  }
}

//******************************************************************************************************************//
//* Scrittura della EEPROM
//******************************************************************************************************************//
void writeEEPROM(unsigned int size)
{
  unsigned int address = 0;  

  while (address < size)
  {
    // single byte write    
    byte val = 0;
    if (Serial.available() > 0) {
      val = Serial.read();
      writeByte(address, val);
      byte wval = waitAndCheckWrite(val);
      Serial.write(&wval, 1);
      address++;
    }
  }
}

//******************************************************************************************************************//
//* Scrittura della EEPROM in modo paginato
//******************************************************************************************************************//
void writePagedEEPROM(unsigned int size, unsigned int pagesize)
{
  unsigned int address = 0;  

  while (address < size)
  {
    // page write
    unsigned int idx = 0;
    #define PAGE_SIZE 64
    byte page[PAGE_SIZE];
    while (idx < PAGE_SIZE) {
      if (Serial.available() > 0) {
        page[idx++] = Serial.read();
      }
    }
    byte val = writePage(address, page, PAGE_SIZE);
    byte wval = waitAndCheckWrite(val);
    // feedback al programmatore dei bytes letti da EPROM
    idx = 0;
    while (idx < PAGE_SIZE) {
      byte val = readByte(address + idx++);
      Serial.write(&val, 1);
    }
    address += PAGE_SIZE;
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
