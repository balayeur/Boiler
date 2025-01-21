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

// extern bool burnerState;


void sendTemperatureData(); // Отправка температур на сервер
void checkBurnerState();    //  Проверка, нужно ли включить горелку
void synchronizeTime();     // Синхронизация времени

#endif // FUNCTION_H
