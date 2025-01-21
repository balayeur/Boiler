#include "ScheduleManager.h"

const char* SCHEDULE_FILE = "/schedule.json";

Schedule schedule[10];  // До 10 временных интервалов
int scheduleCount = 0;

bool loadSchedule() {
  if (!LittleFS.exists(SCHEDULE_FILE)) {
    Serial.println("Файл расписания не найден");
    return false;
  }

  File file = LittleFS.open(SCHEDULE_FILE, "r");
  if (!file) {
    Serial.println("Ошибка открытия файла расписания");
    return false;
  }

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();

  if (error) {
    Serial.println("Ошибка чтения JSON файла");
    return false;
  }

  scheduleCount = 0;
  for (JsonObject item : doc.as<JsonArray>()) {
    schedule[scheduleCount].startHour = item["startHour"];
    schedule[scheduleCount].startMinute = item["startMinute"];
    schedule[scheduleCount].endHour = item["endHour"];
    schedule[scheduleCount].endMinute = item["endMinute"];
    schedule[scheduleCount].minTemp = item["minTemp"];
    schedule[scheduleCount].maxTemp = item["maxTemp"];
    scheduleCount++;
  }

  return true;
}

bool saveSchedule() {
  File file = LittleFS.open(SCHEDULE_FILE, "w");
  if (!file) {
    Serial.println("Ошибка открытия файла для записи");
    return false;
  } else {
    Serial.println("Файл открыт для записи");
  }

  StaticJsonDocument<1024> doc;
  JsonArray array = doc.to<JsonArray>();

  for (int i = 0; i < scheduleCount; i++) {
    JsonObject item = array.createNestedObject();
    item["startHour"] = schedule[i].startHour;
    item["startMinute"] = schedule[i].startMinute;
    item["endHour"] = schedule[i].endHour;
    item["endMinute"] = schedule[i].endMinute;
    item["minTemp"] = schedule[i].minTemp;
    item["maxTemp"] = schedule[i].maxTemp;
  }

  if (serializeJson(doc, file) == 0) {
    Serial.println("Ошибка записи JSON");
    file.close();
    return false;
  }

  file.close();
  Serial.println("Расписание сохранено");
  return true;
}

bool isInTimeRange(Schedule sched, int hour, int minute) {
  int start = sched.startHour * 60 + sched.startMinute;
  int end = sched.endHour * 60 + sched.endMinute;
  int current = hour * 60 + minute;
  return current >= start && current <= end;
}

bool shouldTurnOnBurner(float currentTemp, bool burnerState) {
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  int currentHour = timeinfo->tm_hour;
  int currentMinute = timeinfo->tm_min;

  for (int i = 0; i < scheduleCount; i++) {
    if (isInTimeRange(schedule[i], currentHour, currentMinute)) {
      Serial.printf("Текущее время %02d:%02d находится в интервале %02d:%02d - %02d:%02d\n",
                    currentHour, currentMinute,
                    schedule[i].startHour, schedule[i].startMinute,
                    schedule[i].endHour, schedule[i].endMinute);
      Serial.println("currentTemp = " + String(currentTemp)
                    + ", minTemp = " + String(schedule[i].minTemp) 
                    + ", maxTemp = " + String(schedule[i].maxTemp));
      if (currentTemp < schedule[i].minTemp && !burnerState) {
        return true; // Включить горелку
      } else if (currentTemp > schedule[i].maxTemp && burnerState) {
        return false; // Выключить горелку
      }
    }
  }
  return burnerState; // Сохранить текущее состояние горелки
}


void handleGetSchedule(AsyncWebServerRequest *request) {  
  StaticJsonDocument<1024> doc;
  JsonArray array = doc.to<JsonArray>();

  for (int i = 0; i < scheduleCount; i++) {
    JsonObject item = array.createNestedObject();
    item["startHour"] = schedule[i].startHour;
    item["startMinute"] = schedule[i].startMinute;
    item["endHour"] = schedule[i].endHour;
    item["endMinute"] = schedule[i].endMinute;
    item["minTemp"] = schedule[i].minTemp;
    item["maxTemp"] = schedule[i].maxTemp;
  }
  
  Serial.println("Sending schedule");
  String json;
  serializeJson(doc, json);
  request->send(200, "application/json", json);
}



void handleUpdateSchedule(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {

  static String jsonData;
  if (index == 0) {
    jsonData = "";
  }

  jsonData += String((char *)data).substring(0, len);
  if (index + len == total)
  {
    Serial.println("Received POST request on /scheduleSet"); // Отладочное сообщение
    Serial.println("Received data:");
    Serial.println(jsonData);

    // Создаем JSON-документ достаточного размера для данных
    StaticJsonDocument<1024> doc;
    // const size_t capacity = JSON_ARRAY_SIZE(10) + 10 * JSON_OBJECT_SIZE(6) + 500;
    // StaticJsonDocument<capacity> doc;

    // Парсинг входящего JSON
    DeserializationError error = deserializeJson(doc, jsonData);
    if (error)
    {
      Serial.print("JSON parse error: ");
      Serial.println(error.c_str());
      request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
      return;
    }

    // Очистка старого расписания
    scheduleCount = 0;

    // Чтение данных из JSON-массива
    JsonArray array = doc.as<JsonArray>();
    for (JsonObject obj : array)
    {
      if (scheduleCount >= 10)
        break; // Ограничение на размер массива

      schedule[scheduleCount].startHour = obj["startHour"];
      schedule[scheduleCount].startMinute = obj["startMinute"];
      schedule[scheduleCount].endHour = obj["endHour"];
      schedule[scheduleCount].endMinute = obj["endMinute"];
      schedule[scheduleCount].minTemp = obj["minTemp"];
      schedule[scheduleCount].maxTemp = obj["maxTemp"];

      scheduleCount++;
    }

    if (saveSchedule()) {
      // Подтверждение успешного получения данных
      request->send(200, "application/json", "{\"status\":\"Schedule saved\"}");
    } else {
      request->send(500, "text/plain", "Failed to save schedule");
    }

    // Отладка: вывод расписания в сериал
    Serial.println("Received schedule:");
    for (size_t i = 0; i < scheduleCount; i++) {
      Serial.printf("Entry %d: %02d:%02d - %02d:%02d, MinTemp: %.1f, MaxTemp: %.1f\n",
                    i + 1,
                    schedule[i].startHour, schedule[i].startMinute,
                    schedule[i].endHour, schedule[i].endMinute,
                    schedule[i].minTemp, schedule[i].maxTemp);
    }

    // Clear jsonData after processing
    jsonData = "";
  }
}


// void handleUpdateSchedule(AsyncWebServerRequest *request) {
//   // Проверяем наличие параметра body
//   if (!request->hasParam("body", true)) {
//     request->send(400, "text/plain", "Bad Request"); // 400 Bad Request
//     return;
//   }

//   String body = request->getParam("body", true)->value();
//   StaticJsonDocument<1024> doc;
//   DeserializationError error = deserializeJson(doc, body);

//   if (error) {
//     request->send(400, "text/plain", "Invalid JSON");
//     return;
//   }
// }

  // // Обработка POST-запроса
  // server.on("/scheduleSet", HTTP_POST,
  //           [](AsyncWebServerRequest *request) {}, 
  //           NULL, 
  //           [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  // {
  //   Serial.println("Received POST request on /scheduleSet"); // Отладочное сообщение

  //   // Создаем JSON-документ достаточного размера для данных
  //   const size_t capacity = JSON_ARRAY_SIZE(10) + 10 * JSON_OBJECT_SIZE(6) + 500;
  //   StaticJsonDocument<capacity> doc;

  //   // Парсинг входящего JSON
  //   DeserializationError error = deserializeJson(doc, data, len);
  //   if (error) {
  //     Serial.print("JSON parse error: ");
  //     Serial.println(error.c_str());
  //     request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
  //     // request->send(400, "text/plain", "Invalid JSON");
  //     return;
  //   }

  //   // Очистка старого расписания    
  //   scheduleCount = 0;

  //   scheduleCount = 0;
  //   for (JsonObject item : doc.as<JsonArray>()) {
  //     schedule[scheduleCount].startHour = item["startHour"];
  //     schedule[scheduleCount].startMinute = item["startMinute"];
  //     schedule[scheduleCount].endHour = item["endHour"];
  //     schedule[scheduleCount].endMinute = item["endMinute"];
  //     schedule[scheduleCount].minTemp = item["minTemp"];
  //     schedule[scheduleCount].maxTemp = item["maxTemp"];
  //     scheduleCount++;
  //   }

  //   if (saveSchedule()) {
  //     // Подтверждение успешного получения данных
  //     // request->send(200, "application/json", "{\"status\":\"Schedule saved\"}");
  //     request->send(200, "text/plain", "Schedule updated");
  //   } else {
  //     request->send(500, "text/plain", "Failed to save schedule");
  //   }

  //   // Отладка: вывод расписания в сериал
  //   Serial.println("Received schedule:");
  //   for (size_t i = 0; i < scheduleSize; i++) {
  //     Serial.printf("Entry %d: %02d:%02d - %02d:%02d, MinTemp: %.1f, MaxTemp: %.1f\n",
  //                   i + 1,
  //                   schedule[i].startHour, schedule[i].startMinute,
  //                   schedule[i].endHour, schedule[i].endMinute,
  //                   schedule[i].minTemp, schedule[i].maxTemp);
  //   } 
  // });

