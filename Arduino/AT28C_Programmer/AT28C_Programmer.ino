/*  
  AT28C_Programmer.ino - Programmatore EEPROM AT28C
  Copyright (C) 2023 DrVector
  Utilizza 74HC595 shift registers
*/

#include <Arduino.h>
#include "Const.h"
#include "SRHelper.h"
#include "AT28C.h"

//******************************************************************************************************************//
//* Variabili globali
//******************************************************************************************************************//
bool serialEcho = false;

void setup() {
  // Inizializza Ouput Pins per SN74HC595
  pinMode(SN_SRCLK_PIN, OUTPUT);
  pinMode(SN_SER_PIN, OUTPUT);
  pinMode(SN_RCLK_PIN, OUTPUT);

  // Inizializza Ouput Pins della EEPROM
  pinMode(EEPROM_WE_PIN, OUTPUT);
  pinMode(EEPROM_OE_PIN, OUTPUT);
  pinMode(EEPROM_CE_PIN, OUTPUT); 

  // Inizializza EEPROM
  digitalWrite(EEPROM_WE_PIN, HIGH); 
  digitalWrite(EEPROM_OE_PIN, HIGH);
  digitalWrite(EEPROM_CE_PIN, LOW);

  // Imposta pin D2/D9 in INPUT
  for (int i = 2; i <= 9; i++) {
    pinMode(i, INPUT);
  }

  // Azzera lo shift register
  //addressWrite(0x0000);

  Serial.begin(115200);
  Serial.println("AT28C EEPROM PROGRAMMER V.1.01");
  Serial.println("");
}

void loop() {
  // Lettura porta seriale
  String s = ReadSerialComand();

  // Parsing dei comandi
  ParseComands(s);
}

//******************************************************************************************************************//
//* Comandi
//******************************************************************************************************************//
// Parsing dei comandi
void ParseComands(String s) {
  String params[10];

  s.toUpperCase();
  if (s != "") {
    if (serialEcho) {
      Serial.println(s);
    }

    String comand = GetComand(s);
    // Serial.println("COMAND: " + comand);
    //**********************************************
    // ECHO
    //**********************************************
    if (comand == "ECHO") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "1") {
        serialEcho = true;
      }
      else if (params[0] == "0") {
        serialEcho = false;
      }
      else if (params[0] == "?") {
        Serial.println("+ECHO=" + String(serialEcho));
      }
    }
    //**********************************************
    // VERSION
    //**********************************************
    if (comand == "VERSION") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "?") {
        Serial.println("+VERSION=0.002b");
      }
    }
    //**********************************************
    // READBYTE
    //**********************************************
    if (comand == "READBYTE") {
      GetComandParams(s, params);
      //Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        byte b = readByte(params[0].toInt());
        Serial.println("+READBYTE=" + (String)b);
      }
    }
    //**********************************************
    // WRITEBYTE
    //**********************************************
    if (comand == "WRITEBYTE") {
      GetComandParams(s, params);
      //Serial.println("PARAM: " + params[0] + "," + params[1]);
      if (params[0] != "") {
        byte b = writeByte(params[0].toInt(), params[1].toInt());
        byte wb = waitAndCheckWrite(b);
        Serial.println("+WRITEBYTE=" + (String)wb);
      }
    }
    //**********************************************
    // READEEPROM
    //**********************************************
    if (comand == "READEEPROM") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        readEEPROM(params[0].toInt());
        //Serial.println("+++");
      }
    }
    //**********************************************
    // WRITEEEPROM
    //**********************************************
    if (comand == "WRITEEEPROM") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] != "") {
        if (params[1] != "") {
          writePagedEEPROM(params[0].toInt(), params[1].toInt());
        } else {
          writeEEPROM(params[0].toInt());
        }
        //Serial.println("+++");
      }
    }
    //**********************************************
    // ENABLESDP
    //**********************************************
    if (comand == "ENABLESDP") {
      GetComandParams(s, params);
      // Serial.println("PARAM: " + params[0]);
      if (params[0] == "1") {
        enableSDP();
        Serial.println("+ENABLESDP=1");
      }
      else if (params[0] == "0") {
        disableSDP();
        Serial.println("+ENABLESDP=0");
      }
    }
  }
}

// Ritorna la stringa di comando
String GetComand(String s) {
  String comand = "";

  int i = s.indexOf('=');
  return s.substring(0, i);
}

// Ritorna lista parametri
void GetComandParams(String s, String(&params)[10]) {
  int index = s.indexOf('=');
  String par = s;
  par.remove(0, index + 1);
  
  for (int i = 0; i < 10; i++) {
    index = par.indexOf(',');
    if (index == -1) {
      int l = par.length();
      if (l > 0) {
        params[i] = par;
      }
      return;
    }
    else {
      params[i] = par.substring(0, index);
      par.remove(0, index + 1);
    }
  }
}

//******************************************************************************************************************//
//* Lettura comandi su porta seriale
//******************************************************************************************************************//
// Variabili globali
char receivedChars[64] = "";
int rcIndex = 0;

String ReadSerialComand(){
  while (Serial.available()) {
    if (rcIndex > 63) rcIndex = 0;
    
    // Legge carattere dalla seriale
    char rc = Serial.read();
    // Carattere di fine comando
    if (rc == '\n' or rc == '\r') {
      receivedChars[rcIndex] = '\0';
      rcIndex = 0;
      return receivedChars;
    }
    else {
      // Nuovo carattere
      receivedChars[rcIndex] = rc;
      rcIndex++;
    }
  }
  
  return "";
}
