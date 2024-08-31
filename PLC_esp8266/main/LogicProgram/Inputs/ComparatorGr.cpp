#include "LogicProgram/Inputs/ComparatorGr.h"
#include "Display/bitmaps/cmp_greate_active.h"
#include "Display/bitmaps/cmp_greate_passive.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ComparatorGr::ComparatorGr(uint8_t ref_percent04, const MapIO io_adr)
    : CommonComparator(ref_percent04, io_adr) {
}

ComparatorGr::~ComparatorGr() {
}

const Bitmap *ComparatorGr::GetCurrentBitmap() {
    switch (state) {
        case LogicItemState::lisActive:
            return &ComparatorGr::bitmap_active;

        default:
            return &ComparatorGr::bitmap_passive;
    }
}

bool ComparatorGr::CompareFunction() {
    return GetValue() > ref_percent04;
}

size_t ComparatorGr::Serialize(uint8_t *buffer, size_t buffer_size) {
    size_t writed = 0;

    return writed;
}

size_t ComparatorGr::Deserialize(uint8_t *buffer, size_t buffer_size) {
    size_t readed = 0;

    return readed;
}