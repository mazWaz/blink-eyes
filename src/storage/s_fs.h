#ifndef S_FS_H
#define S_FS_H
#include <AnimatedGIF.h>
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

class SFS {
   public:
    SFS();
    bool init(int chipSelectPin = 5);
    bool checkDir();

    bool deleteFile(const String &filename);
    bool fileExists(String fileName);
    String listFiles();

    File openFile(const char *fname);
    void closeFile(File *f);

    static void *GIFOpenFile(const char *fname, int32_t *pSize);
    static void GIFCloseFile(void *pHandle);
    static int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen);
    static int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition);

   private:
    static const char *szFilename;
    int _chipSelectPin;
};

#endif