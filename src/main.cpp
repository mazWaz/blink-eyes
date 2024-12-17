#include <Arduino.h>
#include <communication/communication.h>
#include <display/display.h>
#include <storage/s_fs.h>
#include <storage/s_spiffs.h>
#include <webService/webservice.h>

SSPIFFS spiffs;
Communication com;
SFS sd;
WebService web(sd);
Display display(sd);

void setup() {
    Serial.begin(115200);
    // spiffs.init();
    // com.init();
    sd.init();
    // web.init();
    display.init();
}

void loop() {
    display.loop();
    // com.loop();
    // web.loop();

    delay(5);
}
// #include <AnimatedGIF.h>
// #include <FS.h>
// #include <SD.h>

// #include "lovyanGfxSetup.h"
// LGFX lcd;
// #define SD_CS 5  // Chip Select pin for SD card

// // Define GIF file path
// const char *gifFilePath =
//     "/anim.gif";  // Ensure this file exists on the SD card
// AnimatedGIF gif;
// File f;
// uint8_t ucFrameBuffer[(128 * 64) + ((128 * 64) / 8)];
// void *GIFOpenFile(const char *fname, int32_t *pSize) {
//     f = SD.open(fname);
//     if (f) {
//         *pSize = f.size();
//         return (void *)&f;
//     }
//     return NULL;
// } /* GIFOpenFile() */

// void GIFCloseFile(void *pHandle) {
//     File *f = static_cast<File *>(pHandle);
//     if (f != NULL) f->close();
// } /* GIFCloseFile() */
// int32_t GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
//     int32_t iBytesRead;
//     iBytesRead = iLen;
//     File *f = static_cast<File *>(pFile->fHandle);
//     // Note: If you read a file all the way to the last byte, seek() stops
//     // working
//     if ((pFile->iSize - pFile->iPos) < iLen)
//         iBytesRead = pFile->iSize - pFile->iPos - 1;  // <-- ugly work-around
//     if (iBytesRead <= 0) return 0;
//     iBytesRead = (int32_t)f->read(pBuf, iBytesRead);
//     pFile->iPos = f->position();
//     return iBytesRead;
// } /* GIFReadFile() */

// int32_t GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
//     int i = micros();
//     File *f = static_cast<File *>(pFile->fHandle);
//     f->seek(iPosition);
//     pFile->iPos = (int32_t)f->position();
//     i = micros() - i;
//     //  Serial.printf("Seek time = %d us\n", i);
//     return pFile->iPos;
// } /* GIFSeekFile() */
// void GIFDraw(GIFDRAW *pDraw) {
//     uint8_t *s;
//     uint16_t *d, *usPalette, usTemp[320];
//     int x, y, iWidth;

//     iWidth = pDraw->iWidth;

//     if (iWidth + pDraw->iX > SCREEN_WIDTH) iWidth = SCREEN_HEIGHT -
//     pDraw->iX; usPalette = pDraw->pPalette; y = pDraw->iY + pDraw->y;  //
//     current line if (y >= SCREEN_HEIGHT || pDraw->iX >= SCREEN_WIDTH ||
//     iWidth < 1) return; s = pDraw->pPixels; if (pDraw->ucDisposalMethod == 2)
//     // restore to background color
//     {
//         for (x = 0; x < iWidth; x++) {
//             if (s[x] == pDraw->ucTransparent) s[x] = pDraw->ucBackground;
//         }
//         pDraw->ucHasTransparency = 0;
//     }

//     // Apply the new pixels to the main image
//     if (pDraw->ucHasTransparency)  // if transparency used
//     {
//         uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
//         int x, iCount;
//         pEnd = s + iWidth;
//         x = 0;
//         iCount = 0;  // count non-transparent pixels
//         while (x < iWidth) {
//             c = ucTransparent - 1;
//             d = usTemp;
//             while (c != ucTransparent && s < pEnd) {
//                 c = *s++;
//                 if (c == ucTransparent)  // done, stop
//                 {
//                     s--;  // back up to treat it like transparent
//                 } else    // opaque
//                 {
//                     *d++ = usPalette[c];
//                     iCount++;
//                 }
//             }  // while looking for opaque pixels
//             if (iCount)  // any opaque pixels?
//             {
//                 lcd.startWrite();
//                 lcd.setAddrWindow(pDraw->iX + x, y, iCount, 1);
//                 lcd.writePixels(usTemp, iCount, false);
//                 lcd.endWrite();
//                 x += iCount;
//                 iCount = 0;
//             }
//             // no, look for a run of transparent pixels
//             c = ucTransparent;
//             while (c == ucTransparent && s < pEnd) {
//                 c = *s++;
//                 if (c == ucTransparent)
//                     iCount++;
//                 else
//                     s--;
//             }
//             if (iCount) {
//                 x += iCount;  // skip these
//                 iCount = 0;
//             }
//         }
//     } else {
//         s = pDraw->pPixels;
//         // Translate the 8-bit pixels through the RGB565 palette (already
//         byte
//         // reversed)
//         for (x = 0; x < iWidth; x++) usTemp[x] = usPalette[*s++];
//         lcd.startWrite();
//         lcd.setAddrWindow(pDraw->iX, y, iWidth, 1);
//         lcd.writePixels(usTemp, iWidth, false);
//         lcd.endWrite();
//     }
// } /* GIFDraw() */
// void setup() {
//     // Start the serial communication for debugging
//     Serial.begin(115200);
//     if (!SD.begin(5)) {
//         Serial.println("SD Card mount failed!");
//         return;
//     } else {
//         Serial.println("SD Card mount succeeded!");
//     }
//     // Initialize the display
//     lcd.init();
//     lcd.setRotation(3);  // Set display orientation (adjust if needed)
//     lcd.setColorDepth(24);
//     lcd.fillScreen(TFT_BLACK);  // Clear the screen with black color
//     delay(2000);
//     lcd.fillScreen(TFT_RED);  // Clear the screen with black color
//     delay(2000);
//     lcd.fillScreen(TFT_GREEN);  // Clear the screen with black color
//     delay(2000);
//     lcd.fillScreen(TFT_BLUE);  // Clear the screen with black color
//     delay(2000);
// }

// void loop() {
//     if (gif.open("/image/image11.GIF", GIFOpenFile, GIFCloseFile,
//     GIFReadFile,
//                  GIFSeekFile, GIFDraw)) {
//         GIFINFO gi;
//         Serial.printf("Successfully opened GIF; Canvas size = %d x %d\n",
//                       gif.getCanvasWidth(), gif.getCanvasHeight());
//         if (gif.getInfo(&gi)) {
//             Serial.printf("frame count: %d\n", gi.iFrameCount);
//             Serial.printf("duration: %d ms\n", gi.iDuration);
//             Serial.printf("max delay: %d ms\n", gi.iMaxDelay);
//             Serial.printf("min delay: %d ms\n", gi.iMinDelay);
//         }
//         while (gif.playFrame(true, NULL)) {
//         }
//         gif.close();
//     } else {
//         Serial.printf("Error opening file = %d\n", gif.getLastError());
//         while (1) {
//         };
//     }
// }