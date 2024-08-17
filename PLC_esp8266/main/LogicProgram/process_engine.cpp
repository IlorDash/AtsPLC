
#include "process_engine.h"
#include "LogicProgram/Controller.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

Controller *controller;

void start_process_engine(EventGroupHandle_t gpio_events) {
    controller = new Controller(gpio_events);
    controller->Start();
}

void stop_process_engine() {
    controller->Stop();
    delete controller;
}