#ifndef EEPROMStream_h
#define EEPROMStream_h

#include <Stream.h>

class EEPROMStream : public Stream {
public:
  EEPROMStream(int start, int size);

  void init();
  int available();
  int read();
  size_t write(uint8_t b);
  int peek();
  void flush();
  void rewind();
  int position();
  void seek(int position);
  int length();

private:
    int _start;
    int _size;
    int _position;
    int _length;
};
#endif
