#include "display.h"
#include "Display/DisplayItemBase.h"
#include "Display/Location.h"
#include "LogicProgram/InputNO.h"
#include "LogicProgram/LogicItemBase.h"
#include "LogicProgram/MapIO.h"
#include "demo.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#include <driver/i2c.h>
#include <esp_err.h>
extern "C" {
#include <ssd1306/ssd1306.h>
}

#define SCL_PIN GPIO_NUM_4
#define SDA_PIN GPIO_NUM_5
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

static const char *TAG = "display";

static struct {
    ssd1306_t dev = { .i2c_port = I2C_NUM_0,
                      .i2c_addr = SSD1306_I2C_ADDR_0,
                      .screen = SSD1306_SCREEN,
                      .width = DISPLAY_WIDTH,
                      .height = DISPLAY_HEIGHT };
    uint8_t buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8] = {};

    const font_info_t *font_4X7 = font_builtin_fonts[FONT_FACE_BITOCRA_4X7];
    const font_info_t *font_6X12 = font_builtin_fonts[FONT_FACE_TERMINUS_6X12_ISO8859_1];
    const font_info_t *font_8X14 = font_builtin_fonts[FONT_FACE_TERMINUS_8X14_ISO8859_1];
} display;

// const font_info_t *font = font_builtin_fonts[FONT_FACE_GLCD5x7];

void ladder_diagram(int8_t x, int8_t y);
void ladder_diagram_acsii(int8_t x, int8_t y);
static void draw_xbm(const ssd1306_t *dev,
                     uint8_t *fb,
                     int8_t x,
                     int8_t y,
                     const uint8_t *xbm_data,
                     int8_t xbm_width,
                     int8_t xbm_height);

void display_init() {
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = SDA_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = SCL_PIN;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.clk_stretch_tick = 300;
    ESP_ERROR_CHECK(i2c_driver_install(display.dev.i2c_port, conf.mode));
    ESP_ERROR_CHECK(i2c_param_config(display.dev.i2c_port, &conf));

    ESP_ERROR_CHECK(ssd1306_init(&display.dev) != 0 ? ESP_FAIL : ESP_OK);
    ESP_ERROR_CHECK(ssd1306_load_frame_buffer(&display.dev, NULL) != 0 ? ESP_FAIL : ESP_OK);
    ESP_ERROR_CHECK(ssd1306_set_whole_display_lighting(&display.dev, false) != 0 ? ESP_FAIL
                                                                                 : ESP_OK);

    ssd1306_load_frame_buffer(&display.dev, bitmap_demo_3);

    // ssd1306_draw_string(&display.dev,
    //                     display.buffer,
    //                     display.font_4X7,
    //                     0,
    //                     0,
    //                     "001.IN0 ? OUT0<1 : OUT1<1",
    //                     OLED_COLOR_WHITE,
    //                     OLED_COLOR_BLACK);

    // ladder_diagram(16, 12);
    // ladder_diagram(16, 29);
    // ladder_diagram(16, 46);
    // // ladder_diagram_acsii(14, 32);

    // ESP_ERROR_CHECK(ssd1306_load_frame_buffer(&display.dev, display.buffer) != 0 ? ESP_FAIL
    //                                                                              : ESP_OK);
    ESP_LOGI(TAG, "init succesfully");
}

InputNO input1(MapIO::DI, { 60, 32 });

void display_demo_0() {
    memset(display.buffer, 0, sizeof(display.buffer));

    draw_xbm(&display.dev,
             display.buffer,
             input1.GetLocation().x,
             input1.GetLocation().y,
             input1.GetBitmap(),
             input1.GetSize().width,
             input1.GetSize().height);
    ssd1306_load_frame_buffer(&display.dev, display.buffer);
}
void display_demo_1() {
    memset(display.buffer, 0, sizeof(display.buffer));
    draw_xbm(&display.dev,
             display.buffer,
             0,
             1,
             cmp_equal_active,
             cmp_equal_active_height,
             cmp_equal_active_width);
    ssd1306_load_frame_buffer(&display.dev, display.buffer);
}

void display_demo_2() {
    memset(display.buffer, 0, sizeof(display.buffer));
    draw_xbm(&display.dev,
             display.buffer,
             0,
             2,
             cmp_equal_active,
             cmp_equal_active_height,
             cmp_equal_active_width);
    ssd1306_load_frame_buffer(&display.dev, display.buffer);

    // ssd1306_load_frame_buffer(&display.dev, bitmap_demo_2);
}

void display_demo(int8_t x, int8_t y) {
    memset(display.buffer, 0, sizeof(display.buffer));
    draw_xbm(&display.dev,
             display.buffer,
             x,
             y,
             cmp_equal_active,
             cmp_equal_active_height,
             cmp_equal_active_width);
    ssd1306_load_frame_buffer(&display.dev, display.buffer);

    // ssd1306_load_frame_buffer(&display.dev, bitmap_demo_2);
}

void ladder_diagram(int8_t x, int8_t y) {

    ssd1306_draw_vline(&display.dev, display.buffer, x, y, 17, OLED_COLOR_WHITE);
    ssd1306_draw_hline(&display.dev, display.buffer, x + 1, y + 11 + 1, 18, OLED_COLOR_WHITE);

    ssd1306_draw_char(&display.dev,
                      display.buffer,
                      display.font_6X12,
                      x + 12,
                      y + 0,
                      '0',
                      OLED_COLOR_WHITE,
                      OLED_COLOR_BLACK);

    ssd1306_draw_vline(&display.dev, display.buffer, x + 19, y + 7 + 1, 9, OLED_COLOR_WHITE);
    ssd1306_draw_vline(&display.dev, display.buffer, x + 24, y + 7 + 1, 9, OLED_COLOR_WHITE);

    ssd1306_draw_hline(&display.dev, display.buffer, x + 25, y + 11 + 1, 11, OLED_COLOR_WHITE);

    ssd1306_draw_char(&display.dev,
                      display.buffer,
                      display.font_6X12,
                      x + 29,
                      y + 0,
                      '1',
                      OLED_COLOR_WHITE,
                      OLED_COLOR_BLACK);

    ssd1306_draw_vline(&display.dev, display.buffer, x + 36, y + 7 + 1, 9, OLED_COLOR_WHITE);
    ssd1306_draw_line(&display.dev,
                      display.buffer,
                      x + 36,
                      y + 15 + 1,
                      x + 41,
                      y + 7 + 1,
                      OLED_COLOR_WHITE);
    ssd1306_draw_vline(&display.dev, display.buffer, x + 41, y + 7 + 1, 9, OLED_COLOR_WHITE);

    ssd1306_draw_char(&display.dev,
                      display.buffer,
                      display.font_6X12,
                      x + 46 + 5,
                      y + 0,
                      'A',
                      OLED_COLOR_WHITE,
                      OLED_COLOR_BLACK);

    ssd1306_draw_hline(&display.dev, display.buffer, x + 42, y + 11 + 1, 11 + 5, OLED_COLOR_WHITE);
    ssd1306_draw_circle(&display.dev, display.buffer, x + 57 + 5, y + 11 + 1, 5, OLED_COLOR_WHITE);
    ssd1306_draw_hline(&display.dev, display.buffer, x + 62 + 5, y + 11 + 1, 7, OLED_COLOR_WHITE);
    ssd1306_draw_vline(&display.dev, display.buffer, x + 74, y, 17, OLED_COLOR_WHITE);
}

void ladder_diagram_acsii(int8_t x, int8_t y) {

    ssd1306_draw_char(&display.dev,
                      display.buffer,
                      display.font_6X12,
                      x + 21,
                      y + 0,
                      '0',
                      OLED_COLOR_WHITE,
                      OLED_COLOR_BLACK);

    ssd1306_draw_char(&display.dev,
                      display.buffer,
                      display.font_6X12,
                      x + 49,
                      y + 0,
                      '1',
                      OLED_COLOR_WHITE,
                      OLED_COLOR_BLACK);

    ssd1306_draw_char(&display.dev,
                      display.buffer,
                      display.font_6X12,
                      x + 77,
                      y + 0,
                      'A',
                      OLED_COLOR_WHITE,
                      OLED_COLOR_BLACK);

    ssd1306_draw_string(&display.dev,
                        display.buffer,
                        display.font_8X14,
                        x,
                        y + 12,
                        "\x15\x12\x16\x15\x12\x16/\x15\x12()\x12\x16",
                        OLED_COLOR_WHITE,
                        OLED_COLOR_BLACK);
}

static void draw_xbm(const ssd1306_t *dev,
                     uint8_t *fb,
                     int8_t x,
                     int8_t y,
                     const uint8_t *xbm_data,
                     int8_t xbm_width,
                     int8_t xbm_height) {

    ESP_LOGI(TAG, "draw_xbm x:%d, y:%d, width:%d, width:%d", x, y, xbm_width, xbm_height);

    for (int row = y; row < y + xbm_height; row += 8) {
        if (row >= dev->height) {
            continue;
        }
        for (int column = x; column < x + xbm_width; column++) {
            if (column >= dev->width) {
                continue;
            }
            int src_id = (((row - y) / 8) * xbm_width) + (column - x);
            int dst_id = ((row / 8) * dev->width) + column;

            uint8_t b = xbm_data[src_id];
            fb[dst_id] |= b << (y % 8);
            if ((y % 8) > 0 && row + 1 < dev->height) {
                fb[dst_id + dev->width] |= b >> (8 - (y % 8));
            }
        }
    }
}