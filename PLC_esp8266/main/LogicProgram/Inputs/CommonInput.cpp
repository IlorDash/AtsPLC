#include "LogicProgram/Inputs/CommonInput.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CommonInput::CommonInput(const MapIO io_adr,
                         const Controller *controller,
                         const Point &incoming_point)
    : InputBase(controller, incoming_point), InputElement(io_adr),
      LabeledLogicItem(MapIONames[io_adr]) {
}

CommonInput::~CommonInput() {
}

bool CommonInput::Render(uint8_t *fb, LogicItemState prev_elem_state, const Point &start_point) {
    bool res = true;
    auto bitmap = GetCurrentBitmap();

    if (prev_elem_state == LogicItemState::lisActive) {
        res &= draw_active_network(fb,
                                   incoming_point.x,
                                   incoming_point.y,
                                   LabeledLogicItem::width + LeftPadding);
    } else {
        res &= draw_passive_network(fb,
                                    incoming_point.x,
                                    incoming_point.y,
                                    LabeledLogicItem::width + LeftPadding,
                                    false);
    }

    uint8_t x_pos = incoming_point.x + LeftPadding;
    res &= draw_text_f6X12(fb, x_pos, incoming_point.y - LabeledLogicItem::height, label);

    x_pos += LabeledLogicItem::width;
    draw_bitmap(fb, x_pos, incoming_point.y - (bitmap->size.height / 2) + 1, bitmap);

    x_pos += bitmap->size.width;
    if (state == LogicItemState::lisActive) {
        res &= draw_active_network(fb, x_pos, incoming_point.y, RightPadding);
    } else {
        res &= draw_passive_network(fb, x_pos, incoming_point.y, RightPadding, true);
    }
    return res;
}

Point CommonInput::OutcomingPoint() {
    auto bitmap = GetCurrentBitmap();
    uint8_t x_pos = LeftPadding + incoming_point.x + LabeledLogicItem::width + bitmap->size.width
                  + RightPadding;
    uint8_t y_pos = incoming_point.y;
    return { x_pos, y_pos };
}