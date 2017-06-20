#include "Arduino.h"
#include <EEPROM.h>
#include "EEPROMStream.h"

EEPROMStream::EEPROMStream(int start, int size):
  _size(size-2),
  _start(start+2),
  _position(0){
}

void EEPROMStream::init(){
#ifdef ESP8266
  EEPROM.begin(this->_size + 2);
#endif
  _length = EEPROM.read(_start-2) | EEPROM.read(_start-1) << 8;
}

int EEPROMStream::available(){
    return _length - _position;
}


int EEPROMStream::read(){
  if (available()){
      return EEPROM.read(_start + _position++);
  } else {
    return -1;
  }
}

size_t EEPROMStream::write(uint8_t b){
  if (_position < _size){
    EEPROM.write(_start +_position++, b);
    return 1;
  } else {
    return 0;
  }
}

int EEPROMStream::peek(){
  if (available()){
    return EEPROM.read(_start + _position);
  } else {
    return -1;
  }
}

void EEPROMStream::flush(){
  EEPROM.write(_start-2, _position & 0xFF);
  EEPROM.write(_start-1, (_position >> 8) & 0xFF);
  EEPROM.commit();
  _length = _position;
  _position = 0;
}

void EEPROMStream::rewind(){
  _position = 0;
}

int EEPROMStream::position() {
    return _position;
}

void EEPROMStream::seek(int position) {
    if (position < _length) {
        _position = position;
    }
}

int EEPROMStream::length() {
    return _length;
}
