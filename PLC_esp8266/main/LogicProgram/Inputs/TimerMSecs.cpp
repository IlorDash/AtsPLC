#include "LogicProgram/Inputs/TimerMSecs.h"
#include "Display/bitmaps/timer_msec_active.h"
#include "Display/bitmaps/timer_msec_passive.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static const char *TAG = "TimerMSecs";

TimerMSecs::TimerMSecs(uint32_t delay_time_ms, InputBase *incoming_item)
    : CommonTimer(incoming_item) {
    if (delay_time_ms < 1) {
        delay_time_ms = 1;
    }
    if (delay_time_ms > 99999) {
        delay_time_ms = 99999;
    }
    this->delay_time_us = delay_time_ms * 1000LL;
    str_size = sprintf(this->str_time, "%u", delay_time_ms);

    ESP_LOGD(TAG, "ctor, str_time:%s", this->str_time);
}

TimerMSecs::~TimerMSecs() {
}

bool TimerMSecs::DoAction() {
    state =
        state == LogicItemState::lisActive ? LogicItemState::lisPassive : LogicItemState::lisActive;
    return true;
}

const Bitmap *TimerMSecs::GetCurrentBitmap() {
    switch (state) {
        case LogicItemState::lisActive:
            return &TimerMSecs::bitmap_active;

        default:
            return &TimerMSecs::bitmap_passive;
    }
}

bool TimerMSecs::Render(uint8_t *fb) {
    if (!require_render) {
        return true;
    }
    bool res = CommonTimer::Render(fb);
    require_render = false;
    return res;
}