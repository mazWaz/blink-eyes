#include "s_fs.h"
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
        if (!SD.mkdir("/image")) {
            return false;
        }
    }

    if (SD.exists("/cache")) {
        if (!SD.mkdir("/cache")) {
            return false;
        }
    }

    return true;
}

bool SFS::deleteFile(const String& filename) {
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

    return false;
}

bool SFS::fileExists(String fileName) {
    if (!SD.exists("/image/" + fileName)) {
        return false;
    }
    if (!SD.exists("/cache/" + fileName)) {
        return false;
    }
    return true;
}

String SFS::listFiles() {
    String fileList = "";
    File dir = SD.open("/cache");

    while (File entry = dir.openNextFile()) {
        fileList += String(entry.name()) + "\n";
        entry.close();
    }
    dir.close();

    return fileList;
}