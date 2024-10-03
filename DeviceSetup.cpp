#include "DeviceSetup.h"
// #include <OneWire.h>
// #include "WebPage.h"

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setupDevices()
{

  // pinMode(BURNER_IN_PIN, INPUT_PULLUP);
  // pinMode(BURNER_IN_PIN, INPUT);

  // analogWriteFreq(Fan_PWM_FRQ);

  // Start up the DS18B20 library
  sensors.begin();
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  // locate devices on the bus
  printf("Locating devices...Found %d devices.\n", numberOfDevices);

  // Loop through each device, print out address
  for (int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i))
    {
      printf("Found device %d with address: ", i);
      printAddress(tempDeviceAddress);
      printf("\n");
    }
    else
    {
      printf("Found ghost device at %d but could not detect address. Check power and cabling", i);
    }
  }

  tempRetour = readDSTemperatureC(0);
  tempAller = readDSTemperatureC(1);
  tempCuve = readDSTemperatureC(2);

  //   int sensorValue = analogRead(sensorPin);  // Lecture de la valeur analogique
  //   current = getCurrent(sensorValue);
  //   burnerState = getBurnerState(current);
  //   Serial.print("sensorValue = ");
  //   Serial.print(sensorValue);
  //   Serial.print(", current = ");
  //   Serial.print(current);
  //   Serial.print(", state = ");
  //   Serial.println(burnerState);
}
