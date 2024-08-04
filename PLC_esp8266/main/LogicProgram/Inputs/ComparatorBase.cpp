#include "LogicProgram/Inputs/ComparatorBase.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ComparatorBase::ComparatorBase(uint16_t reference, const MapIO io_adr, InputBase &prior_item)
    : InputBase(io_adr, prior_item) {
    if (reference > 999) {
        reference = 999;
    }
    this->reference = reference;
    str_size = sprintf(this->str_reference, "%d", reference);
}

ComparatorBase::~ComparatorBase() {
}

void ComparatorBase::Render(uint8_t *fb) {
    InputBase::Render(fb);

    uint8_t x_pos = incoming_point.x + LeftPadding + LabeledLogicItem::width + 2;
    switch (str_size) {
        case 1:
            draw_text_f5X7(x_pos + 3, incoming_point.y + 2, str_reference);
            break;
        case 2:
            draw_text_f5X7(x_pos + 0, incoming_point.y + 2, str_reference);
            break;
        default:
            draw_text_f4X7(x_pos, incoming_point.y + 3, str_reference);
            break;
    }
}