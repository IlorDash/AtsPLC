#include "LogicProgram/Outputs/DecOutput.h"

const Bitmap DecOutput::bitmap_passive = { //
    { 24,                                  // width
      16 },                                // height
    { 0xB0, 0x04, 0x00, 0x01, 0xF0, 0x10, 0x10, 0xE0, 0x00, 0xF0, 0x10, 0x10,
      0x10, 0x00, 0xE0, 0x10, 0x10, 0x60, 0x01, 0x00, 0x04, 0xB0, 0x80, 0x80,
      0x0D, 0x20, 0x00, 0x80, 0x1F, 0x10, 0x10, 0x0F, 0x00, 0x1F, 0x11, 0x11,
      0x11, 0x00, 0x0F, 0x10, 0x10, 0x08, 0x80, 0x00, 0x20, 0x0D, 0x01, 0x01 }
};
