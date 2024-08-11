#pragma once

#include "Display/Common.h"
#include "Display/DisplayChainItem.h"
#include "LogicProgram/LogicElement.h"
#include "LogicProgram/MapIO.h"
#include <stdint.h>
#include <unistd.h>

class InputBase : public LogicElement, public DisplayChainItem{
  protected:
    InputBase(const Controller &controller,
              const Point &incoming_point,
              LogicItemState incoming_item_state);

  public:
    InputBase(const InputBase &prev_item);
    virtual ~InputBase();
};
