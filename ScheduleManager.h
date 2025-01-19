#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

// Файл с расписанием
// const char *SCHEDULE_FILE = "/schedule.json";
extern const char* SCHEDULE_FILE;

// Структура расписания
struct Schedule {
  int startHour;
  int startMinute;
  int endHour;
  int endMinute;
  float minTemp;
  float maxTemp;
};

extern Schedule schedule[10];  // До 10 временных интервалов
extern int scheduleCount;

// Загрузка расписания из файла
bool loadSchedule();

// Сохранение расписания в файл
bool saveSchedule();

// Проверка, находится ли текущее время в интервале
bool isInTimeRange(Schedule sched, int hour, int minute);

// Обработка API получения расписания
void handleGetSchedule(AsyncWebServerRequest *request);

// Обработка API обновления расписания
void handleUpdateSchedule(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

#endif  // SCHEDULE_MANAGER_H