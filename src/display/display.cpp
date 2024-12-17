#include "display.h"
LGFX Display::tft;

Display::Display(SFS &fs) : _fs(fs) {}
void Display::init() {
    tft.init();
    tft.setRotation(3);  // Set display orientation (adjust if needed)
    tft.setColorDepth(24);
    tft.fillScreen(TFT_BLACK);  // Clear the screen with black color
}

void Display::loop() {
    if (gif.open("/image/image11.GIF", _fs.GIFOpenFile, _fs.GIFCloseFile,
                 _fs.GIFReadFile, _fs.GIFSeekFile, GIFDraw)) {
        GIFINFO gi;
        Serial.printf("Successfully opened GIF; Canvas size = %d x% d\n ",
                      gif.getCanvasWidth(), gif.getCanvasHeight());
        if (gif.getInfo(&gi)) {
            Serial.printf("frame count: %d\n", gi.iFrameCount);
            Serial.printf("duration: %d ms\n", gi.iDuration);
            Serial.printf("max delay: %d ms\n", gi.iMaxDelay);
            Serial.printf("min delay: %d ms\n", gi.iMinDelay);
        }
        while (gif.playFrame(true, NULL)) {
        }
        gif.close();
    } else {
        Serial.printf("Error opening file = %d\n", gif.getLastError());
        while (1) {
        };
    }
}

void Display::GIFDraw(GIFDRAW *pDraw) {
    uint8_t *s;
    uint16_t *d, *usPalette, usTemp[320];
    int x, y, iWidth;

    iWidth = pDraw->iWidth;

    if (iWidth + pDraw->iX > 240) iWidth = 240 - pDraw->iX;
    usPalette = pDraw->pPalette;
    y = pDraw->iY + pDraw->y;  // current line
    if (y >= 240 || pDraw->iX >= 240 || iWidth < 1) return;
    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2)  // restore to background color
    {
        for (x = 0; x < iWidth; x++) {
            if (s[x] == pDraw->ucTransparent) s[x] = pDraw->ucBackground;
        }
        pDraw->ucHasTransparency = 0;
    }

    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency)  // if transparency used
    {
        uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
        int x, iCount;
        pEnd = s + iWidth;
        x = 0;
        iCount = 0;  // count non-transparent pixels
        while (x < iWidth) {
            c = ucTransparent - 1;
            d = usTemp;
            while (c != ucTransparent && s < pEnd) {
                c = *s++;
                if (c == ucTransparent)  // done, stop
                {
                    s--;  // back up to treat it like transparent
                } else    // opaque
                {
                    *d++ = usPalette[c];
                    iCount++;
                }
            }  // while looking for opaque pixels
            if (iCount)  // any opaque pixels?
            {
                tft.startWrite();
                tft.setAddrWindow(pDraw->iX + x, y, iCount, 1);
                tft.writePixels((lgfx::rgb565_t *)usTemp, iCount);
                tft.endWrite();
                x += iCount;
                iCount = 0;
            }
            // no, look for a run of transparent pixels
            c = ucTransparent;
            while (c == ucTransparent && s < pEnd) {
                c = *s++;
                if (c == ucTransparent)
                    iCount++;
                else
                    s--;
            }
            if (iCount) {
                x += iCount;  // skip these
                iCount = 0;
            }
        }
    } else {
        s = pDraw->pPixels;
        // Translate the 8-bit pixels through the RGB565 palette (already
        // byte reversed)
        for (x = 0; x < iWidth; x++) usTemp[x] = usPalette[*s++];
        tft.startWrite();
        tft.setAddrWindow(pDraw->iX, y, iWidth, 1);
        tft.writePixels((lgfx::rgb565_t *)usTemp, iWidth);
        tft.endWrite();
    }
}
