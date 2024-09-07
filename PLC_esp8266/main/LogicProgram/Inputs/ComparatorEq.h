#pragma once

#include "LogicProgram/Inputs/CommonComparator.h"
#include <stdint.h>
#include <unistd.h>

class ComparatorEq : public CommonComparator {
  private:
    const static Bitmap bitmap_active;
    const static Bitmap bitmap_passive;

    const Bitmap *GetCurrentBitmap() override final;
    bool CompareFunction() override final;

  public:
    ComparatorEq();
    ComparatorEq(uint8_t ref_percent04, const MapIO io_adr);
    ~ComparatorEq();
    TvElementType GetElementType() override final;
};
