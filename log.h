#ifndef LOG_H
#define LOG_H

#include <Arduino.h>

void logData(const String& message);

#endif


// const char* logFileName = "/log.txt";
// File logFile;


// logFile = LittleFS.open(logFileName, "a"); // Открываем файл для добавления данных
// if (!logFile) {
//     Serial.println("Failed to open log file for writing");
// }

// if (httpResponseCode > 0) {
//     // Остальной код без изменений
// } else {
//     Serial.print("Error sending GET request! HTTP Error code: ");
//     Serial.println(httpResponseCode);

//     // Запись в файл лога
//     if (logFile) {
//         logFile.println("Error sending GET request! HTTP Error code: " + String(httpResponseCode));
//         logFile.flush(); // Сбрасываем буфер записи
//     }
// }

// logFile.close(); // Закрываем файл
