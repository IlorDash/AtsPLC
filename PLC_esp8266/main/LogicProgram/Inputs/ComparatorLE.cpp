#include "LogicProgram/Inputs/ComparatorLE.h"
#include "Display/bitmaps/cmp_less_or_equal_active.h"
#include "Display/bitmaps/cmp_less_or_equal_passive.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ComparatorLE::ComparatorLE(uint16_t reference, const MapIO io_adr, InputBase *incoming_item)
    : CommonComparator(reference, io_adr, incoming_item) {
}

ComparatorLE::~ComparatorLE() {
}

const Bitmap *ComparatorLE::GetCurrentBitmap() {
    switch (state) {
        case LogicItemState::lisActive:
            return &ComparatorLE::bitmap_active;

        default:
            return &ComparatorLE::bitmap_passive;
    }
}

bool ComparatorLE::CompareFunction() {
    return GetValue() <= reference;
}