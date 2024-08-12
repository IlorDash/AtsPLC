#include "LogicProgram/Inputs/InputNC.h"
#include "Display/bitmaps/input_close_active.h"
#include "Display/bitmaps/input_close_passive.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InputNC::InputNC(const MapIO io_adr, InputBase *incoming_item)
    : CommonInput(io_adr, incoming_item) {
}

InputNC::~InputNC() {
}

bool InputNC::DoAction() {
    state =
        state == LogicItemState::lisActive ? LogicItemState::lisPassive : LogicItemState::lisActive;
    return true;
}

const Bitmap *InputNC::GetCurrentBitmap() {
    switch (state) {
        case LogicItemState::lisActive:
            return &InputNC::bitmap_active;

        default:
            return &InputNC::bitmap_passive;
    }
}

bool InputNC::Render(uint8_t *fb) {
    if (!require_render) {
        return true;
    }
    bool res = CommonInput::Render(fb);
    require_render = false;
    return res;
}
