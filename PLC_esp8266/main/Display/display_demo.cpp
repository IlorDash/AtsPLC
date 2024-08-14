#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "Display/Common.h"
#include "LogicProgram/LogicProgram.h"
#include "LogicProgram/StatusBar.h"
#include "display.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *TAG_Demo = "DEMO";

void display_demo_0(bool active) {
    Controller controller;
    StatusBar statusBar(controller, 0);

    IncomeRail incomeRail0(controller, 0);
    InputNO input1(MapIO::DI, &incomeRail0);
    ComparatorGE comparator1(5, MapIO::AI, &input1);
    ComparatorEq comparator2(250, MapIO::V1, &comparator1);
    DirectOutput directOutput0(MapIO::O1, &comparator2);
    OutcomeRail outcomeRail0(0);

    if (active) {
        input1.DoAction();
        comparator1.DoAction();
        comparator2.DoAction();
        directOutput0.DoAction();
    }

    IncomeRail incomeRail1(controller, 1);
    TimerSecs timerSecs1(2, &incomeRail1);
    TimerSecs timerSecs2(10, &timerSecs1);
    TimerMSecs timerMSecs3(10000, &timerSecs2);
    OutcomeRail outcomeRail1(1);

    if (active) {
        timerSecs1.DoAction();
        timerSecs2.DoAction();
        timerMSecs3.DoAction();
    }

    uint8_t *fb = begin_render();
    statusBar.Render(fb);
    incomeRail0.Render(fb);
    input1.Render(fb);
    comparator1.Render(fb);
    comparator2.Render(fb);
    directOutput0.Render(fb);
    outcomeRail0.Render(fb);

    incomeRail1.Render(fb);
    timerSecs1.Render(fb);
    timerSecs2.Render(fb);
    timerMSecs3.Render(fb);
    outcomeRail1.Render(fb);
    end_render(fb);

    for (size_t i = 0; i < 150; i++) {
        vTaskDelay(100 / portTICK_PERIOD_MS);

        bool need_render = false;
        need_render |= timerSecs1.DoAction() | timerSecs1.ProgressHasChanges();
        need_render |= timerSecs2.DoAction() | timerSecs2.ProgressHasChanges();
        need_render |= timerMSecs3.DoAction();

        if (need_render) {
            ESP_LOGI(TAG_Demo, "cycle  i:%u, need_render:%u", i, need_render);
            begin_render();

            statusBar.Render(fb);
            incomeRail0.Render(fb);
            input1.Render(fb);
            comparator1.Render(fb);
            comparator2.Render(fb);
            directOutput0.Render(fb);
            outcomeRail0.Render(fb);

            incomeRail1.Render(fb);
            timerSecs1.Render(fb);
            timerSecs2.Render(fb);
            timerMSecs3.Render(fb);
            outcomeRail1.Render(fb);
            end_render(fb);
        }
    }
}
