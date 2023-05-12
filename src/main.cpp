#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"
#include "LiquidCrystal_I2C.h"

#define DHTPIN 4
#define DHTTYPE DHT11

#define PIN_RED    23 // GIOP23
#define PIN_GREEN  19 // GIOP22
#define PIN_BLUE   18 // GIOP21

// Укажите ваш MAC-адрес прибора
#define DEVICE_MAC "84:cc:a8:2f:8e:c4"

// Инициализируем датчики
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setRed() {
  analogWrite(PIN_RED, 255);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE, 0);
}

void setGreen() {
  analogWrite(PIN_RED, 0);
  analogWrite(PIN_GREEN, 255);
  analogWrite(PIN_BLUE, 0);
}

void setBlue() {
  analogWrite(PIN_RED, 0);
  analogWrite(PIN_GREEN, 0);
  analogWrite(PIN_BLUE, 255);
}



void error(String str){
  lcd.clear();
  Serial.println(str);
  lcd.setCursor(0, 0);

  if (str == "Connect to Wifi") {
    setBlue();
    lcd.println(str);
    
  } 
  else if (str == "DHT11 error" || str == "BMP180 error") {
    setRed();
    lcd.println(str);
    delay(3000);
  } 
  else if (str == "-1") {
    setRed();
    str += " http error";
    lcd.println(str);
    delay(3000);
  }
  
  setGreen();


}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  setGreen();

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("meteo-home");

  // Инициализируем датчики
  dht.begin();

  if (isnan(dht.readTemperature()) || isnan(dht.readHumidity())) {
    error("DHT11 error");
    setup();

  }

  if (!bmp.begin()) {
    error("BMP180 error");
    setup();
  }

  const char *ssid = "Мой WIFI";
  const char *password = "popopopa";

  // Подключаемся к Wi-Fi
  WiFi.begin("Мой WIFI", "popopopa");
  while (WiFi.status() != WL_CONNECTED) {
    
    error("Connect to Wifi");
  }

  setGreen();
}

void loop() {
  float temperature_dht = dht.readTemperature(); // Получает значение температуры с DHT11
  float temperature_bmp = bmp.readTemperature(); // Получает значение температуры с BMP180
  float pressure = bmp.readPressure() / 133.3224F; // Получает значение давления и преобразует его в мм.рт.ст
  float humidity = dht.readHumidity();




  //Отправляем данные на сервер
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://narodmon.ru/post.php";
    String data = "ID=" + String(DEVICE_MAC) + "&temperature_bmp=" + String(temperature_bmp) + "&temperature_dht=" + String(temperature_dht) + "&humidity=" + String(humidity) + "&pressure=" + String(pressure);
    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(data);
    Serial.printf("HTTP response code: %d\n", httpCode);
    http.end();

    error(String(httpCode));
    Serial.print("SEND OK!\nT1: ");

  }



  Serial.print(temperature_bmp);
  Serial.print(", T2: ");
  Serial.print(temperature_dht);
  Serial.print(", PR:");
  Serial.print(pressure);
  Serial.print(", HM: ");
  Serial.println(humidity);

  lcd.setCursor(0, 0);
  lcd.print("T: "); lcd.print(temperature_bmp); lcd.setCursor(9, 0); lcd.print(temperature_dht); lcd.write(223); lcd.print("C"); 
  lcd.setCursor(0, 1);
  lcd.print(humidity); lcd.print("% "); lcd.print(pressure); lcd.print("mmHg"); 

  delay(5 * 60 * 1000 + 5000); // Отправляем данные каждые 5 минут

}
