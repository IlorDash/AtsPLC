#include "LogicElement.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

LogicElement::LogicElement() {
    this->state = LogicItemState::lisPassive;
}

LogicElement::~LogicElement() {
}
