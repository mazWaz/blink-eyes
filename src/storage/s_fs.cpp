#include "s_fs.h"
#define SD_CS_PIN 5  // Pin connected to SD card CS (modify as per your setup)
SFS::SFS() : _chipSelectPin(5) {}

bool SFS::init(int chipSelectPin) {
    _chipSelectPin = chipSelectPin;
    return SD.begin(_chipSelectPin);
}

bool SFS::saveFile(const String& filename, const uint8_t* data, size_t size) {
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open file for writing");
        return false;
    }
    file.write(data, size);
    file.close();
    return true;
}

bool SFS::deleteFile(const String& filename) {
    if (SD.exists(filename)) {
        SD.remove(filename);
        return true;
    }
    return false;
}

bool SFS::exists(const String& filename) { return SD.exists(filename); }

String SFS::listFiles() {
    String fileList = "";
    File dir = SD.open("/");
    while (File entry = dir.openNextFile()) {
        fileList += String(entry.name()) + "\n";
        entry.close();
    }
    return fileList;
}