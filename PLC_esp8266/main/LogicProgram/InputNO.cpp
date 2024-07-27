#include "LogicProgram/InputNO.h"
#include "Display/bitmaps/input_open_active.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InputNO::InputNO(const MapIO io_adr, const Point &location)
    : LogicItemBase(), LabeledLogicItem(MapIONames[io_adr], location) {
    this->io_adr = io_adr;
}

InputNO::~InputNO() {
}

bool InputNO::DoAction() {
    return true;
}

void InputNO::Render(uint8_t *fb) {
    LabeledLogicItem::Render(fb);

    uint8_t x_pos = location.x + LabeledLogicItem::width;
    uint8_t y_pos = location.y + (LabeledLogicItem::height - InputNO::bitmap.size.height / 2) + 1;
    DisplayItemBase::draw(fb, x_pos, y_pos, InputNO::bitmap);

    x_pos = location.x;
    y_pos = location.y + LabeledLogicItem::height;
    draw_network(x_pos, y_pos, LabeledLogicItem::width);
}