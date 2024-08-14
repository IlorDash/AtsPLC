#pragma once

#include "LogicProgram/Inputs/CommonComparator.h"
#include <stdint.h>
#include <unistd.h>

class ComparatorLE : public CommonComparator {
  private:
    const static Bitmap bitmap_active;
    const static Bitmap bitmap_passive;

    const Bitmap *GetCurrentBitmap() override final;
    bool CompareFunction() override final;

  public:
    ComparatorLE(uint8_t ref_percent04, const MapIO io_adr, InputBase *incoming_item);
    ~ComparatorLE();
};
