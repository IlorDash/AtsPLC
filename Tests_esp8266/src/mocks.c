#include "CppUTestExt/MockSupport_c.h"
#include "driver/adc.h"
#include "driver/gpio.h"
#include "esp_spiffs.h"
#include "freertos/event_groups.h"
#include <stdlib.h>

const char *esp_err_to_name(esp_err_t code) {
    (void)code;
    return "UNIT_TESTS_WRAPPER";
}

esp_err_t gpio_config(const gpio_config_t *config) {
    return mock_c()
        ->actualCall("gpio_config")
        ->withConstPointerParameters("config", config)
        ->returnIntValueOrDefault(ESP_OK);
}

esp_err_t adc_init(adc_config_t *config) {
    return mock_c()
        ->actualCall("adc_init")
        ->withConstPointerParameters("config", config)
        ->returnIntValueOrDefault(ESP_OK);
}

int gpio_get_level(gpio_num_t gpio_num) {
    return mock_c()
        ->actualCall("gpio_get_level")
        ->withIntParameters("gpio_num", gpio_num)
        ->returnIntValueOrDefault(-1);
}

esp_err_t gpio_set_level(gpio_num_t gpio_num, uint32_t level) {
    switch (gpio_num) {
        case GPIO_NUM_2:
            return mock_c()
                ->actualCall("gpio_set_level_0")
                ->withIntParameters("gpio_num", gpio_num)
                ->withUnsignedIntParameters("level", level)
                ->returnIntValueOrDefault(ESP_OK);

        case GPIO_NUM_15:
            return mock_c()
                ->actualCall("gpio_set_level_1")
                ->withIntParameters("gpio_num", gpio_num)
                ->withUnsignedIntParameters("level", level)
                ->returnIntValueOrDefault(ESP_OK);
        default:
            return mock_c()
                ->actualCall("gpio_set_level")
                ->withIntParameters("gpio_num", gpio_num)
                ->withUnsignedIntParameters("level", level)
                ->returnIntValueOrDefault(ESP_OK);
    }
}

esp_err_t adc_read(uint16_t *data) {
    return mock_c()
        ->actualCall("adc_read")
        ->withPointerParameters("data", data)
        ->returnIntValueOrDefault(ESP_OK);
}

/*

*/
esp_err_t esp_vfs_spiffs_register(const esp_vfs_spiffs_conf_t *conf) {
    return mock_scope_c(conf->partition_label)
        ->actualCall("esp_vfs_spiffs_register")
        ->withConstPointerParameters("conf", conf)
        ->returnIntValueOrDefault(ESP_OK);
}

esp_err_t esp_vfs_spiffs_unregister(const char *partition_label) {
    return mock_scope_c(partition_label)
        ->actualCall("esp_vfs_spiffs_unregister")
        ->returnIntValueOrDefault(ESP_OK);
}

esp_err_t gpio_install_isr_service(int no_use) {
    (void)no_use;
    return mock_c()->actualCall("gpio_install_isr_service")->returnIntValueOrDefault(ESP_OK);
}

esp_err_t gpio_isr_handler_add(gpio_num_t gpio_num, gpio_isr_t isr_handler, void *args) {
    (void)args;
    char buffer[32];
    sprintf(buffer, "%d", gpio_num);

    return mock_scope_c(buffer)
        ->actualCall("gpio_isr_handler_add")
        ->withPointerParameters("gpio_isr_t", isr_handler)
        ->returnIntValueOrDefault(ESP_OK);
}

EventGroupHandle_t xEventGroupCreate(void) {
    return mock_c()->actualCall("xEventGroupCreate")->returnPointerValueOrDefault(NULL);
}

BaseType_t xEventGroupSetBitsFromISR(EventGroupHandle_t xEventGroup,
                                     const EventBits_t uxBitsToSet,
                                     BaseType_t *pxHigherPriorityTaskWoken) {
    (void)pxHigherPriorityTaskWoken;
    char buffer[32];
    sprintf(buffer, "0x%08X", uxBitsToSet);

    return mock_scope_c(buffer)
        ->actualCall("xEventGroupSetBitsFromISR")
        ->withPointerParameters("xEventGroup", xEventGroup)
        ->returnIntValueOrDefault(pdTRUE);
}