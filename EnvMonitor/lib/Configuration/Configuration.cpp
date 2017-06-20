#include "Configuration.h"
#include <msgpck.h>

bool msgpck_read_string(Stream * s, String ** pString);

bool ConfigurationObject::read() {
    bool result = true;

    // Read configuration
    this->pStream = new EEPROMStream(0, 512);
    this->pStream->init();
    Serial.println(this->pStream->length());

    // Magic number
    result &= msgpck_integer_next(this->pStream);
    if (!result) goto exit;
    byte magicNumber;
    result &= msgpck_read_integer(this->pStream, &magicNumber, 0);
    if (!result || magicNumber != 42) goto exit;

    // Sections
    uint32_t numberOfSections;
    msgpck_read_map_size(this->pStream, &numberOfSections);

    for (int i = 0; i < numberOfSections; i++) {
        // Read the section key
        byte section;
        msgpck_read_integer(this->pStream, &section, 0);

        switch (section) {
            case config_section_t::CONFIG_SECTION_WIFI:
                this->readSectionWifi();
                break;
        }
    }

exit:
    delete this->pStream;

    return result;
}

bool ConfigurationObject::readSectionWifi() {
    if (msgpck_nil_next(this->pStream)) {
        msgpck_read_nil(this->pStream);
        return true;
    }

    this->pWifiConfig = new config_wifi_t;
    uint32_t numberOfFields;
    msgpck_read_map_size(this->pStream, &numberOfFields);

    for (uint32_t i = 0; i < numberOfFields; i++) {
        uint8_t field;
        msgpck_read_integer(this->pStream, &field, 1);

        switch (field) {
            case config_field_t::CONFIG_FIELD_WIFI_NETWORKS:
                uint32_t numberOfNetworks;
                msgpck_read_array_size(this->pStream, &numberOfNetworks);

                config_wifi_network_t *pPrevNetwork;
                for (size_t j = 0; j < numberOfNetworks; j++) {
                    config_wifi_network_t *pNetwork = new config_wifi_network_t;

                    uint32_t numberOfNetworkFields;
                    msgpck_read_map_size(this->pStream, &numberOfNetworkFields);
                    for (size_t k = 0; k < numberOfNetworkFields; k++) {
                        uint8_t networkField;
                        msgpck_read_integer(this->pStream, &networkField, 1);

                        switch (networkField) {
                            case config_field_t::CONFIG_FIELD_WIFI_SSID:
                                msgpck_read_string(this->pStream, &pNetwork->ssid);
                                break;
                            case config_field_t::CONFIG_FIELD_WIFI_PASSWORD:
                                msgpck_read_string(this->pStream, &pNetwork->password);
                                break;
                        }
                    }

                    if (this->pWifiConfig->networks == NULL) {
                        this->pWifiConfig->networks = pNetwork;
                    }
                    if (pPrevNetwork != NULL) {
                        pPrevNetwork->next = pNetwork;
                    }
                    pPrevNetwork = pNetwork;
                }
                break;
        }
    }

    return true;
}

void ConfigurationObject::write() {
    this->pStream = new EEPROMStream(0, 512);
    this->pStream->init();

    // Write the magic number
    msgpck_write_integer(this->pStream, 42);

    // Write the number of sections in the configuration
    msgpck_write_map_header(this->pStream, 1);

    // Write the WiFi section header
    msgpck_write_integer(this->pStream, config_section_t::CONFIG_SECTION_WIFI);
    if (this->pWifiConfig == NULL) {
        msgpck_write_nil(this->pStream);
    } else {
        // config_wifi_t
        msgpck_write_map_header(this->pStream, 1);

        // networks
        msgpck_write_integer(this->pStream, config_field_t::CONFIG_FIELD_WIFI_NETWORKS);

        int numberOfNetworks = 0;
        config_wifi_network_t *pNetwork = this->pWifiConfig->networks;
        while (pNetwork != NULL) {
            numberOfNetworks++;
            if (pNetwork->next == NULL) {
                break;
            }
            pNetwork = pNetwork->next;
        }
        msgpck_write_array_header(this->pStream, numberOfNetworks);

        pNetwork = this->pWifiConfig->networks;
        while (pNetwork != NULL) {
            msgpck_write_map_header(this->pStream, 2);

            msgpck_write_integer(this->pStream, config_field_t::CONFIG_FIELD_WIFI_SSID);
            msgpck_write_string(this->pStream, *pNetwork->ssid);

            msgpck_write_integer(this->pStream, config_field_t::CONFIG_FIELD_WIFI_PASSWORD);
            msgpck_write_string(this->pStream, *pNetwork->password);

            pNetwork = pNetwork->next;
        }
    }

    this->pStream->flush();
    delete this->pStream;
}

bool msgpck_read_string(Stream * s, String ** pString) {
  uint8_t fb;
  bool b = true;
  uint32_t read_size = 0;
  uint8_t * p = (uint8_t *) &read_size;
  if(s->readBytes(&fb,1) == 1) {
    if((fb >> 5) == 5) {
      read_size = fb & 0x1f;
    } else if(fb == 0xd9) {
      b &= s->readBytes(&p[0],1) == 1;
    } else if(fb == 0xda) {
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else if(fb == 0xdb) {
      b &= s->readBytes(&p[3],1) == 1;
      b &= s->readBytes(&p[2],1) == 1;
      b &= s->readBytes(&p[1],1) == 1;
      b &= s->readBytes(&p[0],1) == 1;
    } else {
      return false;
    }

    char str[read_size + 1];
    str[read_size] = '\0';

    uint32_t i;
    for(i = 0; i < read_size; i++) {
      b &= s->readBytes(&str[i],1) == 1;
    }

    if (b)
        *pString = new String(str);

    return b;
  }
  return false;
}
