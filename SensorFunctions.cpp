#include "SensorFunctions.h"

String readDSTemperatureC(int index) {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  float tempC = sensors.getTempCByIndex(index);

  if (tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature [");
    Serial.print(index);
    Serial.print("]: ");
    Serial.println(tempC);
  }
  return String(tempC);
}

bool getBurnerState(float _current) {
  return _current > 0.2;
}

float getCurrent(int _sensorValue) {
  const float sensitivity = 185.0;  // Sensitivity of the sensor (in mV/A)
  const float offsetSensor = 785;
  
  float mVolt = (3.3 / 1024) * (_sensorValue - offsetSensor) * 1000;
  return mVolt / sensitivity;
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}