#ifndef SENSOR_FUNCTIONS_H
#define SENSOR_FUNCTIONS_H

// #include <Arduino.h>
#include <DallasTemperature.h>

extern DallasTemperature sensors;

String readDSTemperatureC(int index);
bool getBurnerState(float _current);
float getCurrent(int _sensorValue);
void printAddress(DeviceAddress deviceAddress);

#endif
