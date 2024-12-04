#ifndef S_FS_H
#define S_FS_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SFS {
   public:
    SFS();
    bool init(int chipSelectPin = 5);
    bool saveFile(const String& filename, const uint8_t* data, size_t size);
    bool deleteFile(const String& filename);
    bool exists(const String& filename);
    String listFiles();

   private:
    int _chipSelectPin;
};

#endif  // S_FS_H