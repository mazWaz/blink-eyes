#include <display/ui/ui.h>
#include <lvgl.h>

#include <vector>

#include "lovyanGfxSetup.h"

#define TFT_HOR_RES SCREEN_WIDTH
#define TFT_VER_RES SCREEN_HEIGHT
#define DRAW_BUF_SIZE (TFT_HOR_RES * TFT_VER_RES / 20 * (LV_COLOR_DEPTH / 8))

class Display {
   private:
    LGFX tft;

    int32_t tftHorRes = SCREEN_WIDTH;
    int32_t tftVerRes = SCREEN_HEIGHT;
    uint8_t *draw_buf = nullptr;

    static void disp_flush(lv_display_t *disp, const lv_area_t *area,
                           uint8_t *px_map);

#if LV_USE_LOG != 0
    static void my_print(lv_log_level_t level, const char *buf) {
        LV_UNUSED(level);
        Serial.println(buf);
        Serial.flush();
    }
#endif

   public:
    Display();
    ~Display();
    void init();
    void loop();
};
