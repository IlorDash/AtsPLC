#include "LogicProgram/Inputs/CommonComparator.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CommonComparator::CommonComparator(uint8_t ref_percent04,
                                   const MapIO io_adr,
                                   InputBase *incoming_item)
    : CommonInput(io_adr, incoming_item) {
    if (ref_percent04 > StatefulElement::MaxValue) {
        ref_percent04 = StatefulElement::MaxValue;
    }
    this->ref_percent04 = ref_percent04;
    str_size = sprintf(this->str_reference, "%d", ref_percent04);
}

CommonComparator::~CommonComparator() {
}

bool CommonComparator::DoAction() {
    bool any_changes = false;
    LogicItemState prev_state = state;

    if (incoming_item->GetState() == LogicItemState::lisActive //
        && this->CompareFunction()) {
        state = LogicItemState::lisActive;
    } else {
        state = LogicItemState::lisPassive;
    }

    if (state != prev_state) {
        any_changes = true;
    }
    return any_changes;
}

bool CommonComparator::Render(uint8_t *fb) {
    bool res;
    res = CommonInput::Render(fb);

    uint8_t x_pos = incoming_point.x + LeftPadding + LabeledLogicItem::width + 2;
    switch (str_size) {
        case 1:
            res &= draw_text_f5X7(fb, x_pos + 3, incoming_point.y + 2, str_reference);
            break;
        case 2:
            res &= draw_text_f5X7(fb, x_pos + 0, incoming_point.y + 2, str_reference);
            break;
        default:
            res &= draw_text_f4X7(fb, x_pos, incoming_point.y + 3, str_reference);
            break;
    }
    return res;
}