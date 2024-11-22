#include "display.h"

Display::Display() {
    draw_buf = (uint8_t *)malloc(DRAW_BUF_SIZE);
    if (!draw_buf) {
        Serial.println("Buffer allocation failed.");
    }
}

Display::~Display() {
    if (draw_buf) {
        free(draw_buf);
        draw_buf = nullptr;
    }
}

void Display::init() {
    tft.begin();
    tft.setRotation(2);
    tft.setBrightness(255);
    tft.fillScreen(TFT_BLACK);

    lv_init();
    lv_tick_set_cb((lv_tick_get_cb_t)millis);

#if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print);
#endif

    lv_display_t *disp = lv_display_create(tftHorRes, tftVerRes);
    if (!disp) {
        return;
    }

    lv_display_set_flush_cb(disp, disp_flush);
    lv_display_set_user_data(disp, this);

    lv_display_set_buffers(disp, draw_buf, NULL, DRAW_BUF_SIZE,
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t *indev = lv_indev_create();
    if (!indev) {
        return;
    }
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);

    ui_init();
}

void Display::loop() {
    lv_timer_handler();
    delay(5);
}

void Display::disp_flush(lv_display_t *disp, const lv_area_t *area,
                         uint8_t *px_map) {
    Display *display = static_cast<Display *>(lv_display_get_user_data(disp));
    LGFX *tft = &display->tft;

    uint32_t w = lv_area_get_width(area);
    uint32_t h = lv_area_get_height(area);

    tft->startWrite();
    tft->setAddrWindow(area->x1, area->y1, w, h);
    tft->writePixels((lgfx::rgb565_t *)px_map, w * h);
    tft->endWrite();

    lv_disp_flush_ready(disp);
}
