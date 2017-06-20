// Includes for PlatformIO
#include "Arduino.h"
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include "Adafruit_BME280.h"
#include <msgpck.h>

#include "Environmental.hpp"
#include "Configuration.h"
#include "EEPROMStream.h"

EnvSensor *pSensor;
ConfigurationObject Config;

void initialConfig() {
    ConfigurationObject c;

    config_wifi_network_t *network = new config_wifi_network_t;
    network->ssid = new String("__");
    network->password = new String("__");

    c.pWifiConfig = new config_wifi_t;
    c.pWifiConfig->networks = network;

    c.write();
}

void setup() {
    Serial.begin(115200);
    pSensor = new Sensor_BME280;
    pSensor->start();

    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println();

    //delay(2000);
    //writeConfig();
    //readConfig();
    //Serial.println("Writing initial configuration...");
    //initialConfig();

    Serial.println("Reading inital config.");
    Config.read();

    Serial.println(*Config.pWifiConfig->networks->ssid);
    Serial.println(*Config.pWifiConfig->networks->password);
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
