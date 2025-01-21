#include "WebServerSetup.h"

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

size_t scheduleSize = 0;    // Количество записей в расписании

WiFiClient client;
HTTPClient http;

const char *serverName = "http://148.179.79.9/post-esp-data.php";
String apiKeyValue = "tPmAT5Ab3j7F9";

const char *openWeatherMapApiKey = "a099a62fc75955cabbeabceb3f046712";
const char *openWeatherMapCity = "tours";
const char *openWeatherMapUnits = "metric"; // Use "imperial" for Fahrenheit

void setupWebServer()
{
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html, processor); });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String response = tempCuve + "," + tempAller + "," + tempRetour;
    request->send(200, "text/plain", response.c_str()); });

  // server.on("/burnerState", HTTP_GET, [](AsyncWebServerRequest *request)
  //           {
  //   String response = String(burnerState);
  //   request->send(200, "text/plain", response.c_str()); });

  server.on("/setSchedule", HTTP_POST, [](AsyncWebServerRequest *request)
            {
    String startTime = request->getParam("startTime", true)->value();
    String endTime = request->getParam("endTime", true)->value();
    int minTemp = request->getParam("minTemp", true)->value().toInt();
    int maxTemp = request->getParam("maxTemp", true)->value().toInt();

    // Сохраните расписание и температуры в переменные или EEPROM

    request->send(200, "text/plain", "Schedule set"); });

  server.on("/scheduler", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/scheduler.html", "text/html"); });

  server.on("/schedule", HTTP_GET, handleGetSchedule);

  server.on("/scheduleSet", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, handleUpdateSchedule);


  // server.on("/scheduleSet", HTTP_POST, [](AsyncWebServerRequest *request) {

  //   Serial.println("Received POST request on /scheduleSet"); // Отладочное сообщение

  //   if (request->contentType() == "application/json") {
  //     String body = "";
  //     request->onRequestBody([&body](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
  //       body += String((char*)data).substring(0, len);
  //       if (index + len == total) {
  //         Serial.println("Request body: " + body); // Отладочное сообщение

  //         StaticJsonDocument<1024> doc;
  //         DeserializationError error = deserializeJson(doc, body);
  //         if (error) {
  //           request->send(400, "text/plain", "Invalid JSON");
  //           return;
  //         }

  //         scheduleCount = 0;
  //         for (JsonObject item : doc.as<JsonArray>()) {
  //           schedule[scheduleCount].startHour = item["startHour"];
  //           schedule[scheduleCount].startMinute = item["startMinute"];
  //           schedule[scheduleCount].endHour = item["endHour"];
  //           schedule[scheduleCount].endMinute = item["endMinute"];
  //           schedule[scheduleCount].minTemp = item["minTemp"];
  //           schedule[scheduleCount].maxTemp = item["maxTemp"];
  //           scheduleCount++;
  //         }

  //         if (saveSchedule()) {
  //           request->send(200, "text/plain", "Schedule updated");
  //         } else {
  //           request->send(500, "text/plain", "Failed to save schedule");
  //         }
  //       }
  //     });
  //   } else {
  //     request->send(400, "text/plain", "Unsupported content type");
  //   }
  // });




  // // Обработка POST-запроса
  // server.on("/scheduleSet", HTTP_POST,
  //           [](AsyncWebServerRequest *request) {}, 
  //           NULL, 
  //           [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
  // {
  //   static String jsonData;
  //   if (index == 0) {
  //     jsonData = "";
  //   }

  //   jsonData += String((char*)data).substring(0, len);
  //   if (index + len == total) {
  //     Serial.println("Received POST request on /scheduleSet"); // Отладочное сообщение
  //     Serial.println("Received data:");
  //     Serial.println(jsonData);

  //     // Создаем JSON-документ достаточного размера для данных
  //     StaticJsonDocument<1024> doc;

  //     // Парсинг входящего JSON
  //     DeserializationError error = deserializeJson(doc, jsonData);
  //     if (error) {
  //       Serial.print("JSON parse error: ");
  //       Serial.println(error.c_str());
  //       request->send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
  //       return;
  //     }

  //     // Очистка старого расписания
  //     scheduleSize = 0;

  //     // Чтение данных из JSON-массива
  //     JsonArray array = doc.as<JsonArray>();
  //     for (JsonObject obj : array) {
  //       if (scheduleSize >= 10) break; // Ограничение на размер массива

  //       schedule[scheduleSize].startHour = obj["startHour"];
  //       schedule[scheduleSize].startMinute = obj["startMinute"];
  //       schedule[scheduleSize].endHour = obj["endHour"];
  //       schedule[scheduleSize].endMinute = obj["endMinute"];
  //       schedule[scheduleSize].minTemp = obj["minTemp"];
  //       schedule[scheduleSize].maxTemp = obj["maxTemp"];

  //       scheduleSize++;
  //     }

  //     // Подтверждение успешного получения данных
  //     request->send(200, "application/json", "{\"status\":\"Schedule saved\"}");

  //     // Отладка: вывод расписания в сериал
  //     Serial.println("Received schedule:");
  //     for (size_t i = 0; i < scheduleSize; i++) {
  //       Serial.printf("Entry %d: %02d:%02d - %02d:%02d, MinTemp: %.1f, MaxTemp: %.1f\n",
  //                     i + 1,
  //                     schedule[i].startHour, schedule[i].startMinute,
  //                     schedule[i].endHour, schedule[i].endMinute,
  //                     schedule[i].minTemp, schedule[i].maxTemp);
  //     }

  //     // Clear jsonData after processing
  //     jsonData = "";
  //   }
  // });

  // Start server
  server.begin();
}

// Function to replace placeholder with variable content
String processor(const String &var)
{
  if (var == "TEMP_TANK")
  {
    return tempCuve;
  }
  else if (var == "TEMP_DIRECT")
  {
    return tempAller;
  }
  else if (var == "TEMP_BACK")
  {
    return tempRetour;
  }
  return String();
}

float getTempOut()
{
  float temperature = 0.0;

  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {

    // // Send HTTP GET request
    // HTTPClient http;

    String apiUrl = "http://api.openweathermap.org/data/2.5/weather?q=";
    apiUrl += String(openWeatherMapCity) + "&appid=" + String(openWeatherMapApiKey);
    apiUrl += "&units=" + String(openWeatherMapUnits);

    int httpResponseCode = http.begin(client, apiUrl);
    // http.begin(apiUrl);
    // int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      // If successful response, get the temperature from the JSON response
      String payload = http.getString();
      // Parse the JSON response to extract the temperature
      // Here you would need to write code to parse the JSON response and extract the temperature
      // For simplicity, I'm assuming the temperature is directly provided in the response
      // For example, if the response looks like: {"main":{"temp":26.5},"...}
      // You would parse it accordingly to get the temperature value
      // float temperature = 20.5; // Assuming the temperature is 26.5 Celsius

      // Размер буфера должен соответствовать размеру вашего JSON-ответа
      // const size_t bufferSize = 2*JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(7) + 190;
      // DynamicJsonDocument jsonBuffer(bufferSize);
      DynamicJsonDocument jsonBuffer(1024);

      // Парсим JSON
      DeserializationError error = deserializeJson(jsonBuffer, payload);

      // Проверяем на ошибки парсинга
      if (error)
      {
        Serial.print("Parsing failed: ");
        Serial.println(error.c_str());
        return -999;
      }

      // Получаем значения температуры и влажности
      float temperature = jsonBuffer["main"]["temp"];
      int humidity = jsonBuffer["main"]["humidity"];

      // Выводим полученные значения в консоль
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity: ");
      Serial.println(humidity);

      // // Append the temperature to the URL
      // url += "&temperature=" + String(temperature);
      // Serial.println("Temperature added to URL: " + url);
    }
    else
    {
      Serial.println("Error getting temperature data from OpenWeatherMap");
    }

    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }

  return temperature;

  //   apiUrl += String(openWeatherMapCity) + "&appid=" + String(openWeatherMapApiKey) + "&units=" + String(openWeatherMapUnits);

  //   http.begin(client, apiUrl);
  //   int httpCode = http.GET();

  //   if (httpCode > 0) {
  //     String payload = http.getString();
  //     Serial.println(payload);

  //     StaticJsonDocument<1024> doc;
  //     DeserializationError error = deserializeJson(doc, payload);

  //     if (!error) {
  //       temperature = doc["main"]["temp"];
  //     }
  //   }

  //   http.end();
  // }

  // return temperature;
}

// Обработка HTTP ответа
void handleHTTPResponse(int httpResponseCode)
{
  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.print(httpResponseCode);

    // Если нужно, можно обработать ответ сервера
    String payload = http.getString();
    Serial.print("; Server response: ");
    Serial.println(payload);
  }
  else
  {
    Serial.print("Error sending request! HTTP Error code: ");
    Serial.println(httpResponseCode);
    // Записываем сообщение об ошибке в журнал
    // logData("Failed to send request");
  }
}

// method GET
void sendGetRequest(String url)
{
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {

    Serial.println("url: " + url);

    // Отправляем HTTP GET запрос
    http.begin(client, url);
    int httpResponseCode = http.GET();

    handleHTTPResponse(httpResponseCode);

    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected");
  }
}

// method POST
void sendPostRequest(String _burner_state)
{
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED)
  {

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Prepare your HTTP POST request data
    String httpRequestData = "api_key=" + apiKeyValue + "&burner_state=" + _burner_state + "";
    //  + "&location=" + sensorLocation
    //  + "&value1=" + String(bme.readTemperature())
    //  + "&value2=" + String(bme.readHumidity())
    //  + "&value3=" + String(bme.readPressure() / 100.0F)

    // Serial.print("httpRequestData: ");
    // Serial.println(httpRequestData);
    printf("httpRequestData: %s\n", httpRequestData.c_str());

    // Отправляем HTTP POST запрос
    int httpResponseCode = http.POST(httpRequestData);

    handleHTTPResponse(httpResponseCode);

    // Free resources
    http.end();
  }
  else
  {
    Serial.println("WiFi Disconnected !!!");
  }
}
