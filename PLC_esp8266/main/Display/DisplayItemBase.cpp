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