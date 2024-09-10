#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "button.h"
#include "buttons.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sys_gpio.h"
#include <vector>

static void buttons_task(void *arg);

static struct {
    std::vector<button> buttons{
        { button("button UP",
                 BUTTON_UP_IO_CLOSE,
                 BUTTON_UP_IO_OPEN,
                 BUTTON_UP_PRESSED,
                 BUTTON_UP_LONG_PRESSED) },
        { button("button DOWN",
                 BUTTON_DOWN_IO_CLOSE,
                 BUTTON_DOWN_IO_OPEN,
                 BUTTON_DOWN_PRESSED,
                 BUTTON_DOWN_LONG_PRESSED) },
        { button("button RIGHT",
                 BUTTON_RIGHT_IO_CLOSE,
                 BUTTON_RIGHT_IO_OPEN,
                 BUTTON_RIGHT_PRESSED,
                 BUTTON_RIGHT_LONG_PRESSED) },
        { button("button SELECT",
                 BUTTON_SELECT_IO_CLOSE,
                 BUTTON_SELECT_IO_OPEN,
                 BUTTON_SELECT_PRESSED,
                 BUTTON_SELECT_LONG_PRESSED) },
    };

    EventGroupHandle_t event;
} buttons;

EventGroupHandle_t buttons_init(EventGroupHandle_t gpio_events, bool is_hotstart) {
    buttons.event = xEventGroupCreate();

    if (is_hotstart) {
        xEventGroupSetBits(buttons.event, BUTTON_LEFT_PRESSED);
    }

    xTaskCreate(buttons_task, "buttons_task", 1024, (void *)gpio_events, 2, NULL);
    return buttons.event;
}

static void buttons_task(void *arg) {
    EventGroupHandle_t gpio_events = (EventGroupHandle_t)arg;
    while (true) {
        EventBits_t uxBits = xEventGroupWaitBits(
            gpio_events,
            BUTTON_UP_IO_CLOSE | BUTTON_UP_IO_OPEN | BUTTON_DOWN_IO_CLOSE | BUTTON_DOWN_IO_OPEN
                | BUTTON_RIGHT_IO_CLOSE | BUTTON_RIGHT_IO_OPEN | BUTTON_SELECT_IO_CLOSE
                | BUTTON_SELECT_IO_OPEN,
            true,
            false,
            portMAX_DELAY);

        for (auto &button : buttons.buttons) {
            switch (button.handle(uxBits)) {
                case button::state::btDown:
                    ESP_LOGD(button.TAG, "process, uxBits:0x%08X btDown", uxBits);
                    break;
                case button::state::btShortPressed:
                    ESP_LOGD(button.TAG, "process, uxBits:0x%08X btShortPressed", uxBits);
                    break;
                case button::state::btPressed:
                    ESP_LOGD(button.TAG, "process, uxBits:0x%08X btPressed", uxBits);
                    xEventGroupSetBits(buttons.event, button.pressed_bit);
                    break;
                case button::state::btLongPressed:
                    ESP_LOGD(button.TAG, "process, uxBits:0x%08X btLongPressed", uxBits);
                    xEventGroupSetBits(buttons.event, button.long_pressed_bit);
                    break;

                default:
                    break;
            }
        }
        // ESP_LOGI(TAG, "process, uxBits:0x%08X", uxBits);
    }
}
