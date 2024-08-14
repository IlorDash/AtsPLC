#include "LogicProgram/Inputs/ComparatorGE.h"
#include "Display/bitmaps/cmp_greate_or_equal_active.h"
#include "Display/bitmaps/cmp_greate_or_equal_passive.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ComparatorGE::ComparatorGE(uint16_t reference, const MapIO io_adr, InputBase *incoming_item)
    : CommonComparator(reference, io_adr, incoming_item) {
}

ComparatorGE::~ComparatorGE() {
}

const Bitmap *ComparatorGE::GetCurrentBitmap() {
    switch (state) {
        case LogicItemState::lisActive:
            return &ComparatorGE::bitmap_active;

        default:
            return &ComparatorGE::bitmap_passive;
    }
}

bool ComparatorGE::CompareFunction() {
    return GetValue() >= reference;
}