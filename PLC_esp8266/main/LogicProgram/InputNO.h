#pragma once

#include "Display/Common.h"
#include "Display/LabeledLogicItem.h"
#include "LogicProgram/LogicItemBase.h"
#include "LogicProgram/MapIO.h"
#include <stdint.h>
#include <unistd.h>

class InputNO : public LogicItemBase, public LabeledLogicItem {
  private:
    MapIO io_adr;

  public:
    const uint8_t LeftPadding = 2;
    const uint8_t RightPadding = 2;
    InputNO(const MapIO io_adr, const Point &location);
    ~InputNO();

    bool DoAction() override final;
    void Render(uint8_t *fb) override final;
    Point OutcomingPoint() override final;
};
