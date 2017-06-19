// Includes for PlatformIO
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "Adafruit_BME280.h"
#include <msgpck.h>

#include "Environmental.hpp"
#include "EEPROMStream.h"

EnvSensor *pSensor;

void writeConfig() {
    EEPROMStream stream(0, 512);
    stream.init();

    msgpck_write_bool(&stream, true);
    msgpck_write_integer(&stream, 54);
    msgpck_write_string(&stream, "Hello There!");

    stream.flush();
}

void readConfig() {
    EEPROMStream stream(0, 512);
    stream.init();

    bool a;
    uint8_t b;
    char buf[16];
    uint bufSize;

    msgpck_read_bool(&stream, &a);
    msgpck_read_integer(&stream, &b, 1);
    msgpck_read_string(&stream, buf, 16, &bufSize);

    // Copy the contents of the array
    char c[bufSize + 1];
    memcpy(c, buf, sizeof(c[0]) * bufSize);
    c[bufSize] = '\0';

    Serial.println(a);
    Serial.println(b);
    Serial.println(c);

    Serial.println();
    Serial.println(c[0]);
    Serial.println(c[1]);
    Serial.println(c[11]);
    Serial.println();
}

void setup() {
    Serial.begin(115200);
    pSensor = new Sensor_BME280;
    pSensor->start();

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println();

    //delay(2000);
    //writeConfig();
    readConfig();
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
