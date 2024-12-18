#include "s_fs.h"

extern SemaphoreHandle_t mutex;

#define SD_CS_PIN 5 // Pin connected to SD card CS (modify as per your setup)
File SFS::gifFile;

SFS::SFS() : _chipSelectPin(5) {}

bool SFS::init(int chipSelectPin)
{
    _chipSelectPin = chipSelectPin;
    SPI.begin();

    if (!SD.begin(_chipSelectPin, SPI, 8000000U))
    {
        return false;
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE)
    {
        return false;
    }
    checkDir();

    Serial.println("Success Begin SD Card");
    listFile = listFiles();
    return true;
}

bool SFS::checkDir()
{
    if (SD.exists("/image"))
    {
        if (!SD.mkdir("/image/"))
        {
            return false;
        }
    }

    if (SD.exists("/cache"))
    {
        if (!SD.mkdir("/cache/"))
        {
            return false;
        }
    }

    return true;
}

bool SFS::deleteFile(const String &filename)
{
    if (xSemaphoreTake(mutex, portMAX_DELAY))
    {
        if (SD.exists("/image/" + filename))
        {
            if (!SD.remove("/image/" + filename))
            {
                return false;
            }
        }

        if (SD.exists("/cache/" + filename))
        {
            if (!SD.remove("/cache/" + filename))
            {
                return false;
            }
        }
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

bool SFS::fileExists(String fileName)
{
    if (xSemaphoreTake(mutex, portMAX_DELAY))
    {
        if (!SD.exists("/image/" + fileName))
        {
            xSemaphoreGive(mutex);

            return false;
        }
        if (!SD.exists("/cache/" + fileName))
        {
            xSemaphoreGive(mutex);
            return false;
        }
        xSemaphoreGive(mutex);
        return true;
    }
    return false;
}

String SFS::listFiles()
{

    String fileList = "";
    File dir = SD.open("/image");

    while (File entry = dir.openNextFile())
    {
        fileList += String(entry.name()) + "\n";
        entry.close();
    }
    if (fileList.length() > 0)
    {
        fileList.remove(fileList.length() -
                        1); // Remove last character (newline)
    }
    dir.close();
    xSemaphoreGive(mutex);
    return fileList;
}

void *SFS::GIFOpenFile(const char *fname, int32_t *pSize)
{
    if (xSemaphoreTake(mutex, portMAX_DELAY))
    {
        if (pSize == nullptr)
        {
            xSemaphoreGive(mutex);
            return nullptr;
        }
        gifFile = SD.open(fname);

        if (gifFile)
        {
            *pSize = gifFile.size();
            xSemaphoreGive(mutex);
            return &gifFile;
        }

        gifFile.close();
        xSemaphoreGive(mutex);
        return nullptr;
    }
}

void SFS::GIFCloseFile(void *pHandle)
{
    if (xSemaphoreTake(mutex, portMAX_DELAY))
    {
        File *f = static_cast<File *>(pHandle);
        if (f != NULL)
        {
            f->close();
            f = NULL;
            xSemaphoreGive(mutex);
        }
        xSemaphoreGive(mutex);
    }
}

int32_t SFS::GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen)
{

    // Take the mutex before accessing shared resources
    if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE)
    {
        int32_t iBytesRead = iLen;
        File *f = static_cast<File *>(pFile->fHandle);

        // Handle edge case when attempting to read past the EOF
        if ((pFile->iSize - pFile->iPos) < iLen)
            iBytesRead = pFile->iSize - pFile->iPos - 1;

        if (iBytesRead <= 0)
        {
            // Release the mutex before returning
            xSemaphoreGive(mutex);
            return 0;
        }

        iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
        pFile->iPos = f->position();

        // Release the mutex after the operation is done
        xSemaphoreGive(mutex);

        return iBytesRead;
    }
    else
    {
        // If unable to take the mutex, return 0
        Serial.println("Failed to acquire mutex.");
        return 0;
    }
}

int32_t SFS::GIFSeekFile(GIFFILE *pFile, int32_t iPosition)
{
    if (xSemaphoreTake(mutex, portMAX_DELAY))
    {
        int i = micros();
        File *f = static_cast<File *>(pFile->fHandle);
        f->seek(iPosition);
        pFile->iPos = (int32_t)f->position();
        i = micros() - i;
        xSemaphoreGive(mutex);
        return pFile->iPos;
    }
    return 0;
}
