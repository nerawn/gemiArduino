#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "I2Cdev.h"
#include "DHT.h"
#include <Wire.h>
#include <ADXL345.h>

ADXL345 adxl;

#define FIREBASE_HOST "nodemcugemi-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "3FPSg30vOAITxd7LlEViYqwG0a8I5p4It4V9l7nz"
#define WIFI_SSID "TurkTelekom_T091B"
#define WIFI_PASSWORD "03Q9tXav"

#define DHTPIN 3
#define DHTTYPE DHT11

DHT dht11(DHTPIN, DHTTYPE);

TinyGPSPlus gps;
SoftwareSerial ss(14, 12);
float latitude , longitude;
String lat_str , lng_str;

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Wire.begin();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Baglaniyor");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  adxl.powerOn();
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  while (ss.available() > 0) //while data is available
    if (gps.encode(ss.read())) //read gps data
    {
      if (gps.location.isValid()) //check whether gps location is valid
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6); // latitude location is stored in a string
        Firebase.setString("lat", lat_str);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6); //longitude location is stored in a string
        Firebase.setString("long", lng_str);
      }
    }

  float temp = dht11.readTemperature();
  Firebase.setFloat("temp", temp);
int x,y,z;
adxl.readAccel(&x); 
  Firebase.setInt("gyroY", x);
}
