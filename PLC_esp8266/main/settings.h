#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#define DEVICE_SETTINGS_VERSION ((uint32_t)0x00000001)

typedef struct {
    size_t count;
    uint32_t state;
} device_settings;

extern device_settings settings;

void load_settings();
void store_settings();