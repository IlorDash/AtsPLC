#pragma once

#include "LogicProgram/ComparatorBase.h"
#include <stdint.h>
#include <unistd.h>

class ComparatorGE : public ComparatorBase {
  private:
    const static Bitmap bitmap_active;
    const static Bitmap bitmap_passive;

    const Bitmap *GetCurrentBitmap() override final;

  public:
    ComparatorGE(int32_t reference, const MapIO io_adr, const Point &incoming_point);
    ComparatorGE(int32_t reference, const MapIO io_adr, InputBase &prior_item);
    ~ComparatorGE();

    bool DoAction() override final;

};
