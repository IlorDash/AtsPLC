#pragma once

#include "LogicProgram/Inputs/InputBase.h"
#include <stdint.h>
#include <unistd.h>

class CommonInput : public InputBase {
  protected:
    virtual const Bitmap *GetCurrentBitmap() = 0;

  public:
    const uint8_t LeftPadding = 2;
    const uint8_t RightPadding = 2;
    CommonInput(const MapIO io_adr, const InputBase &incoming_item);
    virtual ~CommonInput();

    bool Render(uint8_t *fb) override;
    Point OutcomingPoint() override final;
};
