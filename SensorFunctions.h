#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

// #include <Arduino.h>
#include <DallasTemperature.h>

extern DallasTemperature sensors;

#define TEMP_ALLER  0
#define TEMP_RETOUR 1
#define TEMP_CUVE   2
// #define TEMP_EAU_CHAUDE 3
// #define TEMP_EXTERIEUR 4


String readDSTemperatureC(int index);
bool getBurnerState(float _current);
float getCurrent(int _sensorValue);
void printAddress(DeviceAddress deviceAddress);

#endif
