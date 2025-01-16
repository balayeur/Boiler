#ifdef ESP32
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#else
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif

#include "pin_wire.h"
#include "DeviceSetup.h"
#include "SensorFunctions.h"
#include "WebServerSetup.h"



// Определяем текущую среду
#define TEST_ENV // Закомментируйте эту строку для рабочей среды



// Number of temperature devices found
int numberOfDevices;

// We'll use this variable to store a found device address
DeviceAddress tempDeviceAddress;

// Variables to store temperature values
String tempAller = "";
String tempRetour = "";
String tempCuve = "";

// const int sensorPin = A0;         // Broche analogique utilisée pour lire le capteur
// const float sensitivity = 185.0;  // Sensibilité du capteur (en mV/A)
// const float offsetSensor = 785;
// float current = 0.0;
// bool burnerState = false;

// Timer variables
unsigned long lastTime_temperature = 0;
unsigned long timer_temperature = 1000 * 60;
// ADC timer
unsigned long lastTime_burnerState = 0;
unsigned long timer_burnerState = 1000;

// Replace with your network credentials
// const char* ssid = "AAS";
// const char* password = "140613ap";

const char* ssid = "SFR_5FB8";
const char* password = "88itss8iwz93gqsk4hjt";

// const char* ssid = "Xiaomi 11T";
// const char* password = "fghjfghj";

// Create AsyncWebServer object on port 80
// AsyncWebServer server(80);

// // REPLACE with your Domain name and URL path or IP address with path
// const char* serverName = "http://192.168.224.39/post-esp-data.php";
// Keep this API Key value to be compatible with the PHP code provided in the project page.
// If you change the apiKeyValue value, the PHP file /post-esp-data.php also needs to have the same key
// String apiKeyValue = "tPmAT5Ab3j7F9";

// const char* serverAddress = "148.179.79.129";  // Адрес вашего сервера
// const char* serverAddress = "148.179.79.9";  // Адрес вашего сервера
const char* serverAddress = "192.168.1.99";       // Home raspberry pi

// Основные endpoint
const char* endpointTempMain = "/get-esp-boiler-temperature-data.php";
const char* endpointBurnerMain = "/get-esp-boiler-burner-data.php";
// Тестовые endpoint
const char* endpointTempTest = "/Boiler/test-get-esp-boiler-temperature-data.php";
const char* endpointBurnerTest = "/Boiler/test-get-esp-boiler-burner-data.php";
// Переменные для выбранных endpoint
const char* endpointTemp;
const char* endpointBurner;

const int serverPort = 80;                                            // Порт HTTP сервера
// const char* endpointTemp =   "/get-esp-boiler-temperature-data.php";  // API endpoint на сервере
// const char* endpointBurner = "/get-esp-boiler-burner-data.php";       // API endpoint на сервере


// int cnt = 0;
// int maxCnt = 0;
bool burnerState = false;
volatile bool signalPresent = false;           // Флаг для отслеживания присутствия сигнала
volatile unsigned long lastInterruptTime = 0;  // Переменная для хранения времени последнего прерывания
const unsigned long timeout = 1000;            // Время в миллисекундах, после которого считаем, что сигнал исчез

const int relayPin = 5;  // Пин для управления реле


void ICACHE_RAM_ATTR handleInterrupt() {  
  signalPresent = true;          // Устанавливаем флаг, что сигнал присутствует
  lastInterruptTime = millis();  // Сохраняем время последнего прерывания
  
  // Serial.print("handleInterrupt() - ");
  // Serial.print(lastInterruptTime);
  // Serial.println(", signalPresent = true");


  // unsigned long currentTime = micros();
  // if (currentTime - lastInterruptTime > 20000) { // Отбрасываем помехи
  //   signalPresent = true;
  // }
  // lastInterruptTime = currentTime;

  // int pwmValue = digitalRead(BURNER_IN_PIN);  // Считываем значение с пина

  // if (pwmValue == HIGH) {
  //   Serial.println("PWM signal is present");
  // } else {
  //   Serial.println("PWM signal is absent");
  // }
}


// SETUP ----------------------------------------------------------
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // Initialize the LED_BUILTIN pin as an output, led ON
  pinMode(LED_02_PIN, OUTPUT);  // led ON

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Выключаем реле по умолчанию

  // pinMode(A0, INPUT);
  // pinMode(ADC_PIN, INPUT);
  // pinMode(sensorPin, INPUT);

  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();


#ifdef TEST_ENV
    Serial.println("Работа в тестовой среде.");
    endpointTemp = endpointTempTest;
    endpointBurner = endpointBurnerTest;
#else
    Serial.println("Работа в рабочей среде.");
    endpointTemp = endpointTempMain;
    endpointBurner = endpointBurnerMain;
#endif

    // Выводим выбранные endpoint
    Serial.print("Используется endpointTemp: ");
    Serial.println(endpointTemp);
    Serial.print("Используется endpointBurner: ");
    Serial.println(endpointBurner);


  setupDevices();

  pinMode(BURNER_IN_PIN, INPUT);
  // Настраиваем вход с подтягивающим резистором
  // pinMode(BURNER_IN_PIN, INPUT_PULLUP);                                            
  // Настраиваем прерывания на фронт сигнала
  // attachInterrupt(digitalPinToInterrupt(BURNER_IN_PIN), handleInterrupt, RISING);
  attachInterrupt(digitalPinToInterrupt(BURNER_IN_PIN), handleInterrupt, HIGH);
  // Настраиваем прерывания на изменение состояния пина
  // attachInterrupt(digitalPinToInterrupt(BURNER_IN_PIN), handleInterrupt, CHANGE);  


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  printf("Connecting to WiFi - %s\n", ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  // Print ESP Local IP Address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // printf("IP address: %s\n", WiFi.localIP().toString().c_str());


  setupWebServer();

  digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off 
  digitalWrite(LED_02_PIN, HIGH); // Turn the LED off 

}

void controlBurner(bool state) {
  if (state) {
    digitalWrite(relayPin, HIGH);  // Включаем реле
  } else {
    digitalWrite(relayPin, LOW);   // Выключаем реле
  }
}


void loop() {

  unsigned long current_time = millis();

  if ((current_time - lastTime_burnerState) > timer_burnerState) {
    // Serial.println(" ****************** ");

    // int sensorValue = analogRead(sensorPin);  // Lecture de la valeur analogique
    // current = getCurrent(sensorValue);
    // bool curBurnerState = getBurnerState(current);

    // Serial.print("sensorValue = ");
    // Serial.print(sensorValue);
    // Serial.print(", current = ");
    // Serial.print(current);
    // Serial.print(", burnerState = ");
    // Serial.println(burnerState);

    // if (cnt > maxCnt) maxCnt = cnt;

    // if (curBurnerState != burnerState) {
    //   burnerState = curBurnerState;
    //   Serial.print("Burner burnerState changed -> ");
    //   Serial.println(burnerState);
    //   // sendGetRequestBurnerStateChange(burnerState);
    //   String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointBurner);
    //   url += "?burner=" + String(burnerState);
    //   sendGetRequest(url);


    // // Читаем состояние пина
    // int pwmState = digitalRead(BURNER_IN_PIN);
    // // pwmInputPin
    // // Проверяем состояние сигнала PWM
    // if (pwmState == HIGH) {
    //   Serial.println("1 - Сигнал PWM присутствует");
    // } else {
    //   Serial.println("1 - Сигнал PWM отсутствует");
    // }

    // if (signalPresent) {
    //   Serial.println("PWM signal is present");
    // } else {
    //   Serial.println("PWM signal is absent");
    // }


    
    // Проверяем, прошло ли время с последнего прерывания
    unsigned long diff_time = current_time - lastInterruptTime;
    if (diff_time > timeout) {
      printf("timeout -> current_time = %ld, diff = %ld, signalPresent = %d => false\n", current_time, diff_time, signalPresent);
      signalPresent = false;  // Сбрасываем флаг, если сигнал исчез
    }

    if (signalPresent != burnerState) {
      digitalWrite(LED_02_PIN, LOW);
      burnerState = signalPresent;
      // Serial.print("signalPresent != burnerState -> current_time = "), Serial.print(current_time), Serial.print(", Burner state changed -> "), Serial.println(burnerState);
      printf("signalPresent != burnerState -> current_time = %ld, Burner state changed -> %d\n", current_time, burnerState);

      // Serial.print("signalPresent != burnerState -> current_time = ");
      // Serial.print(current_time);
      // Serial.print(", Burner state changed -> ");
      // Serial.println(burnerState);

      String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointBurner);
      url += "?burner_state=" + String(burnerState);
      url += "&esp_time=" + String(current_time);

      // Serial.println(url);
      sendGetRequest(url);
      // sendPostRequest((burnerState) ? "1" : "0");
      // signalPresent = false;
      digitalWrite(LED_02_PIN, HIGH);
    }


    // unsigned long pulseDuration = 0;      //pulseIn(BURNER_IN_PIN, LOW);  // Измеряем длительность импульса
    // unsigned long pulseLongDuration = 0;  //pulseInLong(BURNER_IN_PIN, LOW);  // Измеряем длительность импульса

    // if ((pulseDuration > 0) || (pulseLongDuration > 0)) {
    //   Serial.print("Сигнал PWM присутствует. Длительность импульса (мкс): ");
    //   Serial.print(pulseDuration);
    //   Serial.print(", long импульса (мкс): ");
    //   Serial.println(pulseLongDuration);
    //   cnt = 0;
    // } else {
    //   cnt++;
    //   Serial.print("Сигнал PWM отсутствует, cnt = ");
    //   Serial.print(cnt);
    //   Serial.print(", maxCnt = ");
    //   Serial.println(maxCnt);
    // }



    // Serial.println(" ----------------- ");

    lastTime_burnerState = millis();
  }



  if ((millis() - lastTime_temperature) > timer_temperature) {
    digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level)
    sensors.requestTemperatures();   // Send the command to get temperatures

    tempRetour = readDSTemperatureC(0);
    tempAller = readDSTemperatureC(1);
    tempCuve = readDSTemperatureC(2);

    // Отправляем температуру на сервер
    // sendGetRequestTemperature();
    String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointTemp);
    url += "?tank=" + String(tempCuve) + "&direct=" + String(tempAller) + "&back=" + String(tempRetour);
    // url += "&burner=" + String(burnerState);

    // float tempOut = getTempOut();
    // url += "&temp_out=" + String(tempOut);
    
    sendGetRequest(url);

    digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

    lastTime_temperature = millis();
  }
}