
#include "LogicProgram/InputNO.h"
#include <stdint.h>
#include <unistd.h>

const uint8_t InputNO::width = 16;
const uint8_t InputNO::height = 16;
const uint8_t InputNO::bitmap[] = { 0xFF, 0x00, 0x00, 0x0A, 0x0A, 0x0A, 0x0A, 0x00,
                                    0x0A, 0x0A, 0x0A, 0x0A, 0x00, 0x00, 0xFF, 0x80,
                                    0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x01 };