#include "Display/MapIOIndicator.h"
#include "Display/Common.h"
#include "LogicProgram/MapIO.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

MapIOIndicator::MapIOIndicator(const Point &incoming_point,
                               const char *name,
                               uint8_t progress,
                               uint8_t separator_width)
    : DisplayItemBase() {
    this->incoming_point = incoming_point;
    this->name = name;
    this->progress = progress;
    this->separator_width = separator_width;
}

MapIOIndicator::~MapIOIndicator() {
}

void MapIOIndicator::Render(uint8_t *fb) {
    draw_horz_progress_bar(fb, incoming_point.x + margin, incoming_point.y, progress);

    draw_text_f5X7(fb,
                    incoming_point.x + margin,
                    incoming_point.y + margin + HORZ_PROGRESS_BAR_HEIGHT,
                    name);

}

uint8_t MapIOIndicator::GetWidth() {
    return margin + (text_width * name_size) + margin + margin;
}

uint8_t MapIOIndicator::GetHeight() {
    return HORZ_PROGRESS_BAR_HEIGHT + text_height;
}