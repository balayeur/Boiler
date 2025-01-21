#include "Function.h"


bool burnerStatePlanned = false;        // Планируемое состояние горелки


// Отправка температур на сервер
void sendTemperatureData() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level)
  sensors.requestTemperatures();   // Send the command to get temperatures

  String tempRetour =  readDSTemperatureC(TEMP_RETOUR);
  String tempAller =   readDSTemperatureC(TEMP_ALLER);
  String tempCuve =    readDSTemperatureC(TEMP_CUVE);

  // Отправляем температуру на сервер
  String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointTemp);
  url += "?tank=" + String(tempCuve) + "&direct=" + String(tempAller) + "&back=" + String(tempRetour);

  sendGetRequest(url);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  int currentHour = timeinfo->tm_hour;
  int currentMinute = timeinfo->tm_min;
  int currentSecond = timeinfo->tm_sec;
  Serial.printf("Current time: %02d:%02d:%02d\n", currentHour, currentMinute, currentSecond);
  Serial.printf("Current temperature: %s\n", tempCuve.c_str());
}


//  Проверка, нужно ли включить горелку
void checkBurnerState() {
  // Получение текущей температуры в котле
  float currTempCuve = sensors.getTempCByIndex(TEMP_CUVE);
  // Проверка, нужно ли включить горелку
  bool newBurnerStatePlanned = shouldTurnOnBurner(currTempCuve, burnerStatePlanned);
  // Управление реле горелки
  if (newBurnerStatePlanned != burnerStatePlanned) {
    burnerStatePlanned = newBurnerStatePlanned;
    if (burnerStatePlanned) {
      digitalWrite(BURNER_OUT_PIN, HIGH);  // Включаем реле
      Serial.println("Burner ON");
      // sendLogToServer("Burner ON");
    } else {
      digitalWrite(BURNER_OUT_PIN, LOW);   // Выключаем реле
      Serial.println("Burner OFF");
      // sendLogToServer("Burner OFF");
    }
  }
  // digitalWrite(BURNER_OUT_PIN, relayOn ? HIGH : LOW);
}


void synchronizeTime() {
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0;
  const int daylightOffset_sec = 3600;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // Устанавливаем время с NTP сервера 

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) { // Получаем время
    Serial.println("Failed to obtain time");
    return;
  }

  char timeStringBuff[50]; // Буфер для форматированной строки времени
  strftime(timeStringBuff, sizeof(timeStringBuff),
           "Time synchronized: %A, %B %d %Y %H:%M:%S",
           &timeinfo); // Форматируем время
  Serial.println(timeStringBuff); // Вывод форматированной строки времени
}
