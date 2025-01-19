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

  String json;
  serializeJson(doc, json);
  request->send(200, "application/json", json);
}

void handleUpdateSchedule(AsyncWebServerRequest *request) {
  if (!request->hasParam("body", true)) {
    request->send(400, "text/plain", "Bad Request");
    return;
  }

  String body = request->getParam("body", true)->value();
  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, body);

  if (error) {
    request->send(400, "text/plain", "Invalid JSON");
    return;
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

  if (saveSchedule()) {
    request->send(200, "text/plain", "Schedule updated");
  } else {
    request->send(500, "text/plain", "Failed to save schedule");
  }
}