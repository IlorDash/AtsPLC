#pragma once

#include "Display/Common.h"
#include "Display/LabeledLogicItem.h"
#include "Display/NetworkedLogicItem.h"
#include "LogicProgram/Inputs/InputBase.h"
#include "LogicProgram/LogicItemBase.h"
#include "LogicProgram/MapIO.h"
#include <stdint.h>
#include <unistd.h>

class OutputBase : public LogicItemBase, public NetworkedLogicItem, public LabeledLogicItem {
  protected:
    MapIO io_adr;
    InputBase *prior_item;

    OutputBase(const MapIO io_adr, const Point &incoming_point);
    virtual const Bitmap *GetCurrentBitmap() = 0;

  public:
    const uint8_t LeftPadding = 7;
    const uint8_t RightPadding = 0;
    OutputBase(const MapIO io_adr, InputBase &prior_item);
    ~OutputBase();

    void SetOrigin() override final;
    void Render(uint8_t *fb) override;
    Point OutcomingPoint() override;
};