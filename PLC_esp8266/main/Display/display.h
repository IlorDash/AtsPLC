#pragma once

#include <stdint.h>
#include <unistd.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#ifdef __cplusplus
extern "C" {
#endif

#include "Display/Common.h"

void display_init();

void display_demo_0(bool active);
void display_demo_1();
void display_demo_2();
void display_demo(int8_t x, int8_t y);

void draw_demo(int8_t x, int8_t y, const uint8_t *xbm_data, uint8_t xbm_width, uint8_t xbm_height);
uint8_t *get_display_buffer();
void begin_render();
void end_render();
void draw_text_f5X7(uint8_t x, uint8_t y, const char *text);
void draw_text_f6X12(uint8_t x, uint8_t y, const char *text);
void draw_active_network(int8_t x, int8_t y, uint8_t w);
void draw_passive_network(int8_t x, int8_t y, uint8_t w, bool inverse_dash);
void draw_bitmap(uint8_t *fb, uint8_t x, uint8_t y, const struct Bitmap *bitmap);

#ifdef __cplusplus
}
#endif