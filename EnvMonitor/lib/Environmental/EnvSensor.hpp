#ifndef _ENVIRONMENT_ENVSENSOR_H
#define _ENVIRONMENT_ENVSENSOR_H

typedef enum {
    ENVSENSOR_TYPE_TEMPERATURE = 1 << 0,
    ENVSENSOR_TYPE_HUMIDITY = 1 << 1,
} envsensor_type_t;

inline envsensor_type_t operator | (envsensor_type_t a, envsensor_type_t b) {
    return envsensor_type_t(((int) a) | ((int) b));
}

class EnvSensor {
public:
    virtual envsensor_type_t getSensorType() = 0;
    virtual void start() = 0;

    virtual float readTemperature() { return 0.0f; };
    virtual float readHumidity() { return 0.0f; };
};

#endif
