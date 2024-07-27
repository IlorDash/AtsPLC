#include "Display/DisplayItemBase.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DisplayItemBase::DisplayItemBase(const Point &incoming_point) {
    this->incoming_point = incoming_point;
}

DisplayItemBase::~DisplayItemBase() {
}

void DisplayItemBase::draw(uint8_t *fb, uint8_t x, uint8_t y, const Bitmap &bitmap) {
    for (int row = y; row < y + bitmap.size.height; row += 8) {
        if (row >= DISPLAY_HEIGHT) {
            continue;
        }
        for (int column = x; column < x + bitmap.size.width; column++) {
            if (column >= DISPLAY_WIDTH) {
                continue;
            }
            int src_id = (((row - y) / 8) * bitmap.size.width) + (column - x);
            int dst_id = ((row / 8) * DISPLAY_WIDTH) + column;

            uint8_t b = bitmap.data[src_id];
            fb[dst_id] |= b << (y % 8);
            if ((y % 8) > 0 && row + 1 < DISPLAY_HEIGHT) {
                fb[dst_id + DISPLAY_WIDTH] |= b >> (8 - (y % 8));
            }
        }
    }
}