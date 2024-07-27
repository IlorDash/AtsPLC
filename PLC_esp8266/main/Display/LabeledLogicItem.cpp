#include "Display/LabeledLogicItem.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LabeledLogicItem::LabeledLogicItem(const char *label, const Point &location)
    : DisplayItemBase(location) {
    this->label = label;
    this->width = 6 * strlen(label);
    this->height = 12;
}

LabeledLogicItem::~LabeledLogicItem() {
}

void LabeledLogicItem::Render(uint8_t *fb) {
    draw_text_f6X12(location.x, location.y, label);
}