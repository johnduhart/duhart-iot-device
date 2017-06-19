#ifndef _ENVIRONMENT_BME280_H
#define _ENVIRONMENT_BME280_H

#include "EnvSensor.hpp"
#include "Adafruit_BME280.h"

class Sensor_BME280 : public EnvSensor {
public:
    Sensor_BME280(void);

    envsensor_type_t getSensorType();
    void start();

    float readTemperature();
    float readHumidity();

private:
    Adafruit_BME280 *pBme;
};

#endif
