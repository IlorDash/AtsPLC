#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main/LogicProgram/Inputs/ComparatorGr.cpp"
#include "main/LogicProgram/Inputs/ComparatorGr.h"
#include "main/LogicProgram/Inputs/IncomeRail.h"

static uint8_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8] = {};

TEST_GROUP(LogicComparatorGrTestsGroup){
    //
    TEST_SETUP(){ memset(frame_buffer, 0, sizeof(frame_buffer));
}

TEST_TEARDOWN() {
}
}
;

namespace {
    class TestableComparatorGr : public ComparatorGr {
      public:
        TestableComparatorGr(uint8_t ref_percent04, const MapIO io_adr, InputBase *incoming_item)
            : ComparatorGr(ref_percent04, io_adr, incoming_item) {
        }
        virtual ~TestableComparatorGr() {
        }

        LogicItemState *PublicMorozov_Get_state() {
            return &state;
        }
    };
} // namespace

TEST(LogicComparatorGrTestsGroup, Render) {

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorGr testable(42, MapIO::V1, &incomeRail);

    CHECK_TRUE(testable.Render(frame_buffer));

    bool any_pixel_coloring = false;
    for (size_t i = 0; i < sizeof(frame_buffer); i++) {
        if (frame_buffer[i] != 0) {
            any_pixel_coloring = true;
            break;
        }
    }
    CHECK_TRUE(any_pixel_coloring);
}

TEST(LogicComparatorGrTestsGroup, DoAction_skip_when_incoming_passive) {
    mock().expectNoCall("adc_read");

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorGr prev_element(0, MapIO::V1, &incomeRail);
    *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisPassive;

    TestableComparatorGr testable(42, MapIO::AI, &prev_element);

    CHECK_FALSE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisPassive, testable.GetState());
}

TEST(LogicComparatorGrTestsGroup, DoAction_change_state_to_active) {
    volatile uint16_t adc = 51 / 0.1;
    mock()
        .expectNCalls(2, "adc_read")
        .withOutputParameterReturning("adc", (const void *)&adc, sizeof(adc));

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorGr prev_element(0, MapIO::V1, &incomeRail);
    *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisActive;

    TestableComparatorGr testable(51 / 0.4, MapIO::AI, &prev_element);

    CHECK_FALSE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisPassive, testable.GetState());

    adc = 52 / 0.1;
    CHECK_TRUE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisActive, testable.GetState());
}

TEST(LogicComparatorGrTestsGroup, DoAction_change_state_to_passive) {
    volatile uint16_t adc = 49 / 0.1;
    mock()
        .expectNCalls(2, "adc_read")
        .withOutputParameterReturning("adc", (const void *)&adc, sizeof(adc));

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorGr prev_element(0, MapIO::V1, &incomeRail);
    *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisActive;

    TestableComparatorGr testable(48 / 0.4, MapIO::AI, &prev_element);
    CHECK_TRUE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisActive, testable.GetState());

    adc = 47 / 0.1;
    CHECK_TRUE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisPassive, testable.GetState());
}