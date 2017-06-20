#ifndef _CONFIGURAION_H
#define _CONFIGURAION_H

#include "EEPROMStream.h"
#include "Arduino.h"

typedef enum : byte {
    CONFIG_SECTION_WIFI = 1
} config_section_t;

typedef struct config_wifi_network_t {
    String *ssid;
    String *password;
    config_wifi_network_t *next;
} config_wifi_network_t;

typedef struct {
    config_wifi_network_t *networks;
} config_wifi_t;

typedef enum {
    CONFIG_FIELD_WIFI_NETWORKS      = 128,
    CONFIG_FIELD_WIFI_SSID          = 129,
    CONFIG_FIELD_WIFI_PASSWORD      = 130,
} config_field_t;

class ConfigurationObject {
public:
    bool read();
    void write();

    config_wifi_t *getWifiConfig();

    config_wifi_t *pWifiConfig;
private:
    EEPROMStream *pStream;

    bool readSectionWifi();
};

#endif
