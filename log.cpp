#include "log.h"
// #include <FS.h>
#include <LittleFS.h>

void logData(const String& message) {
  File logFile = LittleFS.open("/log.txt", "a");
  if (logFile) {
    logFile.println(message);
    logFile.close();
  } else {
    Serial.println("Failed to open log file for writing");
  }
}
