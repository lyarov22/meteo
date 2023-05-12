#include <WiFi.h>
#include <WebServer.h>
 
#include "DHT.h"
#include <Adafruit_BMP085.h>

 
 
// Раскомментируйте ту строку, которая соответствует вашему датчику
 
#define DHTTYPE DHT11   // DHT 11
 
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
 
 
 
const char* ssid = "Мой WIFI";  // Вводим сюда SSID
 
const char* password = "popopopa";  //Вводим пароль
 
 
 
WebServer server(80);
 
 
 
// датчик DHT
 
uint8_t DHTPin = 4;
 
 
 
// Инициализируем DHT
 
DHT dht(DHTPin, DHTTYPE);

Adafruit_BMP085 bmp;

 
 
 
float Temperature;
 
float Humidity;
 

String SendHTML(float Temperaturestat, float Humiditystat, float Pressurestat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 Weather Report</h1>\n";
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  ptr +="<p>Pressure: ";
  ptr +=(int)Pressurestat;
  ptr +="hPa</p>";
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
 
}

void handle_OnConnect() {
 
 
 
Temperature = dht.readTemperature(); // Получает значение температуры
 
Humidity = dht.readHumidity(); // Получаем показания влажности

float pressure = bmp.readPressure() / 100.0; // Получает значение давления и преобразует его в гектопаскали

server.send(200, "text/html", SendHTML(Temperature, Humidity, pressure));

 
}
 
 
 
void handle_NotFound(){
 
server.send(404, "text/plain", "Not found");
 
}
 
 
 
 
void setup() {
 
Serial.begin(115200);
 
delay(100);
 
 
 
pinMode(DHTPin, INPUT);
 
 
 
dht.begin();
 
 
 
Serial.println("Connecting to ");
 
Serial.println(ssid);
 
 
 
//подключаемся к WiFi
 
WiFi.begin(ssid, password);
 

 
//проверяем подключение
 
while (WiFi.status() != WL_CONNECTED) {
 
delay(1000);
 
Serial.print(".");
 
}
 
Serial.println("");
 
Serial.println("WiFi connected..!");
 
Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
 
 
 
server.on("/", handle_OnConnect);
 
server.onNotFound(handle_NotFound);
 
 
 
server.begin();
 
Serial.println("HTTP server started");
 
 
 
}
 
void loop() {
 
 
 
server.handleClient();
 
 
 
}
 
 
 

