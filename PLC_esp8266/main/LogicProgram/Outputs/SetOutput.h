#pragma once

#include "LogicProgram/Outputs/CommonOutput.h"
#include <stdint.h>
#include <unistd.h>

class SetOutput : public CommonOutput {
  private:
    const static Bitmap bitmap_active;
    const static Bitmap bitmap_passive;

    const Bitmap *GetCurrentBitmap() override final;

  protected:

  public:
    explicit SetOutput();
    SetOutput(const MapIO io_adr);
    ~SetOutput();

    bool DoAction(bool prev_elem_changed, LogicItemState prev_elem_state) override final;
    TvElementType GetElementType() override final;
};
