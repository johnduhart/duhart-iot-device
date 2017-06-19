#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include "Adafruit_BME280.h"
#include "Environmental.hpp"

EnvSensor *pSensor;

void setup() {
    Serial.begin(115200);
    pSensor = new Sensor_BME280;
    pSensor->start();

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println();
}

void loop() {
    Serial.println("Hi!");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);

    Serial.print("Temperature = ");
    Serial.print(pSensor->readTemperature());
    Serial.println(" *C");

    Serial.print("Humidity = ");
    Serial.print(pSensor->readHumidity());
    Serial.println(" %");

    Serial.println();

    delay(500);
}
