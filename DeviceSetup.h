#ifndef DEVICE_SETUP_H
#define DEVICE_SETUP_H

#include "pin_wire.h"
#include <DallasTemperature.h>
#include "SensorFunctions.h"


// extern const int sensorPin;

extern int numberOfDevices;
extern DeviceAddress tempDeviceAddress;
extern String tempAller;
extern String tempRetour;
extern String tempCuve;

// extern float current;
// extern bool burnerState;

void setupDevices();

#endif
