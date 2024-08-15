#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main/LogicProgram/Inputs/ComparatorLE.cpp"
#include "main/LogicProgram/Inputs/ComparatorLE.h"
#include "main/LogicProgram/Inputs/IncomeRail.h"

static uint8_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8] = {};

TEST_GROUP(LogicComparatorLETestsGroup){
    //
    TEST_SETUP(){ memset(frame_buffer, 0, sizeof(frame_buffer));
}

TEST_TEARDOWN() {
}
}
;

namespace {
    class TestableComparatorLE : public ComparatorLE {
      public:
        TestableComparatorLE(uint8_t ref_percent04, const MapIO io_adr, InputBase *incoming_item)
            : ComparatorLE(ref_percent04, io_adr, incoming_item) {
        }
        virtual ~TestableComparatorLE() {
        }

        LogicItemState *PublicMorozov_Get_state() {
            return &state;
        }
    };
} // namespace

TEST(LogicComparatorLETestsGroup, Render) {

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorLE testable(42, MapIO::V1, &incomeRail);

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

TEST(LogicComparatorLETestsGroup, DoAction_skip_when_incoming_passive) {
    mock().expectNoCall("adc_read");

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorLE prev_element(0, MapIO::V1, &incomeRail);
    *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisPassive;

    TestableComparatorLE testable(42, MapIO::AI, &prev_element);

    CHECK_FALSE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisPassive, testable.GetState());
}

TEST(LogicComparatorLETestsGroup, DoAction_change_state_to_active) {
    volatile uint16_t adc = 51 / 0.1;
    mock()
        .expectNCalls(3, "adc_read")
        .withOutputParameterReturning("adc", (const void *)&adc, sizeof(adc));

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorLE prev_element(0, MapIO::V1, &incomeRail);
    *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisActive;

    TestableComparatorLE testable(50 / 0.4, MapIO::AI, &prev_element);

    CHECK_FALSE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisPassive, testable.GetState());

    adc = 50 / 0.1;
    CHECK_TRUE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisActive, testable.GetState());

    adc = 49 / 0.1;
    CHECK_FALSE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisActive, testable.GetState());
}

TEST(LogicComparatorLETestsGroup, DoAction_change_state_to_passive) {
    volatile uint16_t adc = 49 / 0.1;
    mock()
        .expectNCalls(2, "adc_read")
        .withOutputParameterReturning("adc", (const void *)&adc, sizeof(adc));

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorLE prev_element(0, MapIO::V1, &incomeRail);
    *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisActive;

    TestableComparatorLE testable(50 / 0.4, MapIO::AI, &prev_element);
    CHECK_TRUE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisActive, testable.GetState());

    adc = 51 / 0.1;
    CHECK_TRUE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisPassive, testable.GetState());
}