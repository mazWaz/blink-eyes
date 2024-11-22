#define LGFX_USE_V1
#include <driver/i2c.h>

#include <LovyanGFX.hpp>
#include <lgfx/v1/platforms/esp32s3/Bus_RGB.hpp>
#include <lgfx/v1/platforms/esp32s3/Panel_RGB.hpp>

#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_GC9A01 _panel_instance;
    lgfx::Bus_SPI _bus_instance;
    lgfx::Light_PWM _light_instance;

   public:
    LGFX(void) {
        {
            auto cfg = _bus_instance.config();
            cfg.spi_host = VSPI_HOST;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            cfg.freq_read = 16000000;
            cfg.spi_3wire = true;
            cfg.use_lock = true;
            cfg.dma_channel = 1;

            cfg.pin_sclk = GPIO_NUM_18;  // SCL
            cfg.pin_miso = -1;
            cfg.pin_mosi = GPIO_NUM_23;  // SDA
            cfg.pin_dc = GPIO_NUM_2;     // DC
            _bus_instance.config(cfg);
            _panel_instance.setBus(&_bus_instance);
        }

        {
            auto cfg = _panel_instance.config();
            cfg.pin_cs = GPIO_NUM_15;  // CS
            cfg.pin_rst = GPIO_NUM_4;  // RST
            cfg.pin_busy = -1;
            cfg.panel_width = SCREEN_WIDTH;
            cfg.panel_height = SCREEN_HEIGHT;
            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            cfg.dummy_read_pixel = 8;
            cfg.dummy_read_bits = 1;
            cfg.readable = false;
            cfg.invert = true;
            cfg.rgb_order = false;
            cfg.dlen_16bit = false;
            cfg.bus_shared = true;
            _panel_instance.config(cfg);

            _panel_instance.config(cfg);
        }

        setPanel(&_panel_instance);
    }
};