#pragma once

#include "LogicProgram/Inputs/InputBase.h"
#include <stdint.h>
#include <unistd.h>

class InputNO : public InputBase {
  private:
    const static Bitmap bitmap_active;
    const static Bitmap bitmap_passive;

    const Bitmap *GetCurrentBitmap() override final;

  public:
    InputNO(const MapIO io_adr, InputBase &prev_item);
    ~InputNO();

    bool DoAction() override final;
};
