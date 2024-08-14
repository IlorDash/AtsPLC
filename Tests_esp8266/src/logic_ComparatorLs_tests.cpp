#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main/LogicProgram/Inputs/ComparatorLs.cpp"
#include "main/LogicProgram/Inputs/ComparatorLs.h"
#include "main/LogicProgram/Inputs/IncomeRail.h"

static uint8_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8] = {};

TEST_GROUP(LogicComparatorLsTestsGroup){
    //
    TEST_SETUP(){ memset(frame_buffer, 0, sizeof(frame_buffer));
}

TEST_TEARDOWN() {
}
}
;

namespace {
    class TestableComparatorLs : public ComparatorLs {
      public:
        TestableComparatorLs(uint16_t reference, const MapIO io_adr, InputBase *incoming_item)
            : ComparatorLs(reference, io_adr, incoming_item) {
        }
        virtual ~TestableComparatorLs() {
        }

        LogicItemState *PublicMorozov_Get_state() {
            return &state;
        }
    };
} // namespace

TEST(LogicComparatorLsTestsGroup, Render) {

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorLs testable(42, MapIO::V1, &incomeRail);

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

TEST(LogicComparatorLsTestsGroup, DoAction_skip_when_incoming_passive) {
    mock().expectNoCall("adc_read");

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorLs prev_element(0, MapIO::V1, &incomeRail);
    *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisPassive;

    TestableComparatorLs testable(42, MapIO::AI, &prev_element);

    CHECK_FALSE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisPassive, testable.GetState());
}

TEST(LogicComparatorLsTestsGroup, DoAction_change_state_to_active) {
    volatile uint16_t adc = 42;
    mock()
        .expectOneCall("adc_read")
        .withOutputParameterReturning("adc", (const void *)&adc, sizeof(adc));

    Controller controller;
    IncomeRail incomeRail(controller, 0);
    TestableComparatorLs prev_element(0, MapIO::V1, &incomeRail);
    *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisActive;

    TestableComparatorLs testable(42 + 1, MapIO::AI, &prev_element);

    CHECK_TRUE(testable.DoAction());
    CHECK_EQUAL(LogicItemState::lisActive, testable.GetState());
}

// TEST(LogicComparatorLsTestsGroup, DoAction_change_state_to_passive) {
//     mock("0").expectOneCall("gpio_get_level").andReturnValue(0);

//     Controller controller;
//     IncomeRail incomeRail(controller, 0);
//     TestableComparatorLs prev_element(MapIO::V1, &incomeRail);
//     *(prev_element.PublicMorozov_Get_state()) = LogicItemState::lisActive;

//     TestableComparatorLs testable(MapIO::DI, &prev_element);
//     *(testable.PublicMorozov_Get_state()) = LogicItemState::lisActive;

//     CHECK_TRUE(testable.DoAction());
//     CHECK_EQUAL(LogicItemState::lisPassive, testable.GetState());
// }
