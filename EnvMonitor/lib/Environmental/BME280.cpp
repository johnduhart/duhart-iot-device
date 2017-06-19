#include "BME280.h"
#include "Adafruit_BME280.h"

Sensor_BME280::Sensor_BME280() {
    this->pBme = new Adafruit_BME280;
}

envsensor_type_t Sensor_BME280::getSensorType() {
    return envsensor_type_t::ENVSENSOR_TYPE_HUMIDITY
            | envsensor_type_t::ENVSENSOR_TYPE_TEMPERATURE;
}

void Sensor_BME280::start() {
    this->pBme->begin();
}

float Sensor_BME280::readTemperature() {
    return this->pBme->readTemperature();
}

float Sensor_BME280::readHumidity() {
    return this->pBme->readHumidity();
}
