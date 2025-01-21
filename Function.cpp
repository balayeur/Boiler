#include "Function.h"
#include "WebServerSetup.h"
#include "SensorFunctions.h"
#include "ScheduleManager.h"
#include <ESP8266WiFi.h>
#include <time.h>

extern String tempAller;
extern String tempRetour;
extern String tempCuve;
extern const char* serverAddress;
extern const int serverPort;
extern const char* endpointTemp;

extern bool burnerState;
extern const int relayPin;


// Отправка температур на сервер
void sendTemperatureData() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level)
  sensors.requestTemperatures();   // Send the command to get temperatures

  tempRetour = readDSTemperatureC(0);
  tempAller = readDSTemperatureC(1);
  tempCuve = readDSTemperatureC(2);

  // Отправляем температуру на сервер
  String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointTemp);
  url += "?tank=" + String(tempCuve) + "&direct=" + String(tempAller) + "&back=" + String(tempRetour);

  sendGetRequest(url);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  int currentHour = timeinfo->tm_hour;
  int currentMinute = timeinfo->tm_min;
  Serial.printf("Current time: %02d:%02d\n", currentHour, currentMinute);
  Serial.printf("Current temperature: %s\n", tempCuve.c_str());
}


//  Проверка, нужно ли включить горелку
void checkBurnerState() {
  // Получение текущей температуры в котле
  float currentTemp = sensors.getTempCByIndex(TEMP_CUVE);

  // Проверка, нужно ли включить горелку
  bool newBurnerState = shouldTurnOnBurner(currentTemp, burnerState);

  // Управление реле горелки
  if (newBurnerState != burnerState) {
    burnerState = newBurnerState;
    if (burnerState) {
      digitalWrite(relayPin, HIGH);  // Включаем реле
      Serial.println("Burner ON");
    //   sendLogToServer("Burner ON");
    } else {
      digitalWrite(relayPin, LOW);   // Выключаем реле
      Serial.println("Burner OFF");
    //   sendLogToServer("Burner OFF");
    }
  }
}
