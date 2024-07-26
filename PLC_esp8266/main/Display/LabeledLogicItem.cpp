#include "Display/LabeledLogicItem.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LabeledLogicItem::LabeledLogicItem(const char *label, const Point &location)
    : DisplayItemBase(location) {
    this->label = label;
}

LabeledLogicItem::~LabeledLogicItem() {
}
