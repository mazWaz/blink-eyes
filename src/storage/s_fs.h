#ifndef S_FS_H
#define S_FS_H

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SFS {
   public:
    SFS();
    bool init(int chipSelectPin = 5);
    bool checkDir();

    bool deleteFile(const String& filename);
    bool fileExists(String fileName);
    String listFiles();

   private:
    int _chipSelectPin;
};

#endif