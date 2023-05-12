#include <SPI.h>

#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

const char* ssid = "Мой WIFI"; // Вводим сюда SSID
const char* password = "popopopa"; //Вводим пароль

WebServer server(80);

#define DHTPIN 4 // пин подключения DHT11
#define DHTTYPE DHT11 // тип датчика DHT11
DHT dht(DHTPIN, DHTTYPE);

Adafruit_BMP085 bmp;

String SendHTML(float temperature_dht, float temperature_bmp, float pressure, float humidity) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta charset=\"UTF-8\">";

  ptr += "<title>ESP32 Weather Report</title>\n";

  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";

  ptr +="<script>\n";
  ptr +="setInterval(loadDoc,200);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById(\"webpage\").innerHTML =this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";

  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>ESP32 Weather Report</h1>\n";
  ptr += "<p>DHT11 Temperature: ";
  ptr += (float)temperature_dht;
  ptr += "°C</p>";
  ptr += "<p>BMP180 Temperature: ";
  ptr += (float)temperature_bmp;
  ptr += "°C</p>";
  ptr += "<p>BMP180 Pressure: ";
  ptr += (float)pressure;
  ptr += "mmHg</p>";
  ptr += "<p>DHT11 Humidity: ";
  ptr += (float)humidity;
  ptr += "%</p>";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void handle_OnConnect() {
  float temperature_dht = dht.readTemperature(); // Получает значение температуры с DHT11
  float temperature_bmp = bmp.readTemperature(); // Получает значение температуры с BMP180
  float pressure = bmp.readPressure() / 133.3224F; // Получает значение давления и преобразует его в мм.рт.ст
  float humidity = dht.readHumidity();

  String html = SendHTML(temperature_dht, temperature_bmp, pressure, humidity);
  server.send(200, "text/html", html);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  delay(100);

  dht.begin();

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  pinMode(DHTPIN, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}



