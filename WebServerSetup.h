// #include <AsyncPrinter.h>
// #include <DebugPrintMacros.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncTCPbuffer.h>
// #include <SyncClient.h>
// #include <async_config.h>
// #include <tcp_axtls.h>

#ifndef WEB_SERVER_SETUP_H
#define WEB_SERVER_SETUP_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <ESP8266HTTPClient.h>
#include "WebPage.h"
#include "ScheduleManager.h"
// #include "log.h"
// #include <ESPAsyncTCP.h>

// WebServer.cpp
extern AsyncWebServer server;
extern WiFiClient client;
extern HTTPClient http;

// main.cpp
extern String tempAller;
extern String tempRetour;
extern String tempCuve;
extern bool burnerState;

//local



String processor(const String& var);

void setupWebServer();

float getTempOut();

void sendGetRequest(String url);
void sendPostRequest(String url);


#endif
