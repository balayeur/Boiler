#include "Function.h"


bool burnerStatePlanned = false;        // Планируемое состояние горелки

void isBurnerStateChanged(volatile bool &signalPresent, 
                          unsigned long lastInterruptTime)
                         // , unsigned long current_time) 
{
  // Проверяем, прошло ли больше времени, чем timeout с последнего прерывания
  if (signalPresent && (millis() - lastInterruptTime > timeout)) {        
    signalPresent = false; // Сбрасываем флаг, если сигнал исчез
  }

  if (signalPresent != lastSignalPresent) 
  {
    digitalWrite(LED_02_PIN, LOW); // Turn the LED on
    lastSignalPresent = signalPresent;
    Serial.println("Burner state changed: " + String(lastSignalPresent) + " -> " + String(signalPresent));
    // printf("signalPresent != lastSignalPresent -> current_time = %ld, 
    //       Burner state changed -> %d\n", millis(), lastSignalPresent);

    String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointBurner);
    url += "?burner_state=" + String(lastSignalPresent);

    sendGetRequest(url);

    digitalWrite(LED_02_PIN, HIGH); // Turn the LED off
  }
}


// Отправка температур на сервер
void sendTemperatureData() {
  digitalWrite(LED_BUILTIN, LOW);  // Turn the LED on (Note that LOW is the voltage level)
  sensors.requestTemperatures();   // Send the command to get temperatures

  String tempRetour =  readDSTemperatureC(TEMP_RETOUR);
  String tempAller =   readDSTemperatureC(TEMP_ALLER);
  String tempCuve =    readDSTemperatureC(TEMP_CUVE);

  // Отправляем температуру на сервер
  String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointTemp);
  url += "?tank=" + String(tempCuve) + "&direct=" + String(tempAller) + "&back=" + String(tempRetour);

  sendGetRequest(url);
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH

  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  int currentHour = timeinfo->tm_hour;
  int currentMinute = timeinfo->tm_min;
  int currentSecond = timeinfo->tm_sec;
  Serial.printf("Current time: %02d:%02d:%02d\n", currentHour, currentMinute, currentSecond);
  Serial.printf("Current temperature: %s\n", tempCuve.c_str());
}


//  Проверка, нужно ли включить горелку
void checkBurnerState() {
  // Получение текущей температуры в котле
  float currTempCuve = sensors.getTempCByIndex(TEMP_CUVE);
  // Проверка, нужно ли включить горелку
  bool newBurnerStatePlanned = shouldTurnOnBurner(currTempCuve, burnerStatePlanned);
  // Управление реле горелки
  if (newBurnerStatePlanned != burnerStatePlanned) {
    burnerStatePlanned = newBurnerStatePlanned;
    if (burnerStatePlanned) {
      digitalWrite(BURNER_OUT_PIN, HIGH);  // Включаем реле
      Serial.println("Burner ON");
      // sendLogToServer("Burner ON");
    } else {
      digitalWrite(BURNER_OUT_PIN, LOW);   // Выключаем реле
      Serial.println("Burner OFF");
      // sendLogToServer("Burner OFF");
    }
  }
  // digitalWrite(BURNER_OUT_PIN, relayOn ? HIGH : LOW);
}


void synchronizeTime() {
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0;
  const int daylightOffset_sec = 3600;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer); // Устанавливаем время с NTP сервера 

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) { // Получаем время
    Serial.println("Failed to obtain time");
    return;
  }

  char timeStringBuff[50]; // Буфер для форматированной строки времени
  strftime(timeStringBuff, sizeof(timeStringBuff),
           "Time synchronized: %A, %B %d %Y %H:%M:%S",
           &timeinfo); // Форматируем время
  Serial.println(timeStringBuff); // Вывод форматированной строки времени
}


//  // Serial.println(" ****************** ");
// /* 
//     // int sensorValue = analogRead(sensorPin);  // Lecture de la valeur analogique
//     // current = getCurrent(sensorValue);
//     // bool curBurnerState = getBurnerState(current);

//     // if (cnt > maxCnt) maxCnt = cnt;

//     // if (curBurnerState != lastSignalPresent) {
//     //   lastSignalPresent = curBurnerState;
//     //   Serial.print("Burner lastSignalPresent changed -> ");
//     //   Serial.println(lastSignalPresent);
//     //   // sendGetRequestBurnerStateChange(lastSignalPresent);
//     //   String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointBurner);
//     //   url += "?burner=" + String(lastSignalPresent);


//     // // Читаем состояние пина
//     // int pwmState = digitalRead(BURNER_IN_PIN);
//     // // pwmInputPin
//     // // Проверяем состояние сигнала PWM
//     // if (pwmState == HIGH) {
//     //   Serial.println("1 - Сигнал PWM присутствует");
//     // } else {
//     //   Serial.println("1 - Сигнал PWM отсутствует");
//     // }

//     // if (signalPresent) {
//     //   Serial.println("PWM signal is present");
//     // } else {
//     //   Serial.println("PWM signal is absent");
//     // }
//  */

    
//     // Проверяем, прошло ли больше времени, чем timeout с последнего прерывания
//     //unsigned long diff_time = current_time - lastInterruptTime;
//     // if ((current_time - lastInterruptTime) > timeout) {
//     if (signalPresent && (millis() - lastInterruptTime > timeout)) {        
//       //printf("timeout -> current_time = %ld, diff = %ld, signalPresent = %d => false\n", current_time, diff_time, signalPresent);
//       //Serial.printf("timeout -> current_time = %ld, diff = %ld, signalPresent = %d => false\n", current_time, diff_time, signalPresent);
      
//       signalPresent = false; // Сбрасываем флаг, если сигнал исчез
//     }

//     if (signalPresent != lastSignalPresent) {
//       digitalWrite(LED_02_PIN, LOW); // Turn the LED on
//       lastSignalPresent = signalPresent;
//       // Serial.print("signalPresent != lastSignalPresent -> current_time = "), Serial.print(current_time), Serial.print(", Burner state changed -> "), Serial.println(lastSignalPresent);
//       printf("signalPresent != lastSignalPresent -> current_time = %ld, Burner state changed -> %d\n", current_time, lastSignalPresent);

//       // Serial.print("signalPresent != lastSignalPresent -> current_time = ");
//       // Serial.print(current_time);
//       // Serial.print(", Burner state changed -> ");
//       // Serial.println(lastSignalPresent);

//       String url = "http://" + String(serverAddress) + ":" + String(serverPort) + String(endpointBurner);
//       url += "?burner_state=" + String(lastSignalPresent);
//       url += "&esp_time=" + String(current_time);

//       // Serial.println(url);
//       sendGetRequest(url);
//       // sendPostRequest((lastSignalPresent) ? "1" : "0");
//       // signalPresent = false;
//       digitalWrite(LED_02_PIN, HIGH); // Turn the LED off
//     }


//     // unsigned long pulseDuration = 0;      //pulseIn(BURNER_IN_PIN, LOW);  // Измеряем длительность импульса
//     // unsigned long pulseLongDuration = 0;  //pulseInLong(BURNER_IN_PIN, LOW);  // Измеряем длительность импульса

//     // if ((pulseDuration > 0) || (pulseLongDuration > 0)) {
//     //   Serial.print("Сигнал PWM присутствует. Длительность импульса (мкс): ");
//     //   Serial.print(pulseDuration);
//     //   Serial.print(", long импульса (мкс): ");
//     //   Serial.println(pulseLongDuration);
//     //   cnt = 0;
//     // } else {
//     //   cnt++;
//     //   Serial.print("Сигнал PWM отсутствует, cnt = ");
//     //   Serial.print(cnt);
//     //   Serial.print(", maxCnt = ");
//     //   Serial.println(maxCnt);
//     // }



//     // Serial.println(" ----------------- ");