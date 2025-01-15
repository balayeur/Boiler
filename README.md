# Boiler
Temp, time..

Installing the ESP32 in Arduino IDE
To install the ESP32 board in your Arduino IDE, follow these next instructions:

1. In your Arduino IDE 2, go to File > Preferences.

Arduino IDE 2 File, Preferences

2. Copy and paste the following line to the Additional Boards Manager URLs field.

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Additional URLs Field ESP32 Arduino 2
Note: if you already have the ESP8266 boards URL, you can separate the URLs with a comma, as follows:

http://arduino.esp8266.com/stable/package_esp8266com_index.json, https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

3. Open the Boards Manager. You can go to Tools > Board > Boards Manager… or you can simply click the Boards Manager icon in the left-side corner.

Arduino IDE 2 - Boards Manager

4. Search for ESP32 and press the install button for esp32 by Espressif Systems version 3.X.

Install ESP32 Arduino IDE 2 board add-on version 3
That’s it. It should be installed after a few seconds.

After this, restart your Arduino IDE.

Then, go to Tools > Board and check that you have ESP32 boards available.

ESP32 Boards installed in Arduino IDE 2
Now, you’re ready to start programming your ESP32 using Arduino IDE.