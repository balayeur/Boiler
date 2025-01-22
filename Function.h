#ifndef FUNCTION_H
#define FUNCTION_H

#include <ESP8266WiFi.h>
#include <time.h>

#include "pin_wire.h"
#include "WebServerSetup.h"
#include "SensorFunctions.h"
#include "ScheduleManager.h"

// extern String tempAller;
// extern String tempRetour;
// extern String tempCuve;
extern const char* serverAddress;
extern const int serverPort;
extern const char* endpointTemp;
extern const char* endpointBurner;

extern const unsigned long timeout; // Declare timeout variable
extern bool lastSignalPresent;      // Declare lastSignalPresent variable


// extern bool burnerState;

// void isBurnerStateChanged(bool signalPresent, unsigned long lastInterruptTime);
void isBurnerStateChanged(volatile bool &signalPresent, unsigned long lastInterruptTime); //, unsigned long current_time);

void sendTemperatureData(); // Отправка температур на сервер
void checkBurnerState();    //  Проверка, нужно ли включить горелку
void synchronizeTime();     // Синхронизация времени

#endif // FUNCTION_H
