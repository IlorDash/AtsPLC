#include "LogicProgram/Inputs/ComparatorEq.h"
#include "Display/bitmaps/cmp_equal_active.h"
#include "Display/bitmaps/cmp_equal_passive.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ComparatorEq::ComparatorEq(uint8_t ref_percent04) : CommonComparator(ref_percent04) {
}

ComparatorEq::~ComparatorEq() {
}

const Bitmap *ComparatorEq::GetCurrentBitmap() {
    switch (state) {
        case LogicItemState::lisActive:
            return &ComparatorEq::bitmap_active;

        default:
            return &ComparatorEq::bitmap_passive;
    }
}

bool ComparatorEq::CompareFunction() {
    return GetValue() == ref_percent04;
}

TvElementType ComparatorEq::GetElementType() {
    return TvElementType::et_ComparatorEq;
}