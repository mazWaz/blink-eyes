#include "s_fs.h"

extern SemaphoreHandle_t mutex;

#define SD_CS_PIN 5  // Pin connected to SD card CS (modify as per your setup)
SFS::SFS() : _chipSelectPin(5) {}

bool SFS::init(int chipSelectPin) {
    _chipSelectPin = chipSelectPin;

    if (!SD.begin(_chipSelectPin)) {
        return false;
    }

    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        return false;
    }
    checkDir();

    Serial.println("Success Begin SD Card");

    return true;
}

bool SFS::checkDir() {
    if (SD.exists("/image")) {
        if (!SD.mkdir("/image/")) {
            return false;
        }
    }

    if (SD.exists("/cache")) {
        if (!SD.mkdir("/cache/")) {
            return false;
        }
    }

    return true;
}

bool SFS::deleteFile(const String &filename) {
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
        if (SD.exists("/image/" + filename)) {
            if (!SD.remove("/image/" + filename)) {
                return false;
            }
        }

        if (SD.exists("/cache/" + filename)) {
            if (!SD.remove("/cache/" + filename)) {
                return false;
            }
        }
        xSemaphoreGive(mutex);
        return true;
    }
}

bool SFS::fileExists(String fileName) {
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
        if (!SD.exists("/image/" + fileName)) {
            return false;
            Serial.println("/Image file Not found");
        }
        if (!SD.exists("/cache/" + fileName)) {
            Serial.println("/cache file Not found");

            return false;
        }
        xSemaphoreGive(mutex);
        return true;
    }
}

String SFS::listFiles() {
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
        String fileList = "";
        File dir = SD.open("/cache");

        while (File entry = dir.openNextFile()) {
            fileList += String(entry.name()) + "\n";
            entry.close();
        }
        if (fileList.length() > 0) {
            fileList.remove(fileList.length() -
                            1);  // Remove last character (newline)
        }
        dir.close();
        xSemaphoreGive(mutex);
        return fileList;
    }
}

void *SFS::GIFOpenFile(const char *fname, int32_t *pSize) {
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
        if (pSize == nullptr) {
            return NULL;
        }
        File *f = new File(SD.open(fname));
        if (f && *f) {
            *pSize = f->size();
            xSemaphoreGive(mutex);
            return (void *)f;
        }

        delete f;
        return NULL;
    }
}

void SFS::GIFCloseFile(void *pHandle) {
    if (xSemaphoreTake(mutex, portMAX_DELAY)) {
        File *f = static_cast<File *>(pHandle);
        Serial.println(f->name());

        if (f != NULL) {
            f->close();
        }
    }
}

int32_t SFS::GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
    int32_t iBytesRead = iLen;
    File *f = static_cast<File *>(pFile->fHandle);
    if ((pFile->iSize - pFile->iPos) < iLen)
        iBytesRead =
            pFile->iSize - pFile->iPos - 1;  // Workaround for read past EOF
    if (iBytesRead <= 0) return 0;
    iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();
    return iBytesRead;
}

int32_t SFS::GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
    int i = micros();
    File *f = static_cast<File *>(pFile->fHandle);
    f->seek(iPosition);
    pFile->iPos = (int32_t)f->position();
    i = micros() - i;
    return pFile->iPos;
}
