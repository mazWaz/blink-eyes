#include <AnimatedGIF.h>
#include <FS.h>
#include <SD.h>

#include "lovyanGfxSetup.h"
#include "storage/s_fs.h"

#define TFT_HOR_RES SCREEN_WIDTH
#define TFT_VER_RES SCREEN_HEIGHT
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 20 * (LV_COLOR_DEPTH / 8))

class Display {
   private:
    static LGFX tft;
    AnimatedGIF gif;
    SFS &_fs;

    static void GIFDraw(GIFDRAW *pDraw);  // Change return type to void

   public:
    Display(SFS &fs);

    void init();
    void loop();
};
