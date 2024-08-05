#include "LogicProgram/Inputs/TimerMSecs.h"

const Bitmap TimerMSecs::bitmap_active = { //
    { 24,                                  // width
      16 },                                // height
    { 0xFF, 0x00, 0x01, 0x01, 0x3F, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x3F, 0x02, 0x04, 0x02, 0x3F, 0x00, 0x12, 0x25, 0x29, 0x12, 0x00, 0xFF,
      0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF }
};
