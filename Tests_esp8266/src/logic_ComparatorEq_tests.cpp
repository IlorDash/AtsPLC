#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main/LogicProgram/Inputs/ComparatorEq.cpp"
#include "main/LogicProgram/Inputs/ComparatorEq.h"
#include "main/LogicProgram/Inputs/IncomeRail.h"

static uint8_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8] = {};

TEST_GROUP(LogicComparatorEqTestsGroup){
    //
    TEST_SETUP(){ memset(frame_buffer, 0, sizeof(frame_buffer));
}

TEST_TEARDOWN() {
}
}
;

namespace {
    class TestableComparatorEq : public ComparatorEq {
      public:
        TestableComparatorEq(uint8_t ref_percent04, const MapIO io_adr, InputBase *incoming_item)
            : ComparatorEq(ref_percent04, io_adr, incoming_item) {
        }
        virtual ~TestableComparatorEq() {
        }

        LogicItemState *PublicMorozov_Get_state() {
            return &state;
        }
    };
} // namespace

TEST(LogicComparatorEqTestsGroup, Render) {

    Controller controller(NULL);
    IncomeRail incomeRail(&controller, 0, LogicItemState::lisActive);
    TestableComparatorEq testable(42, MapIO::V1, &incomeRail);

    CHECK_TRUE(testable.Render(frame_buffer, LogicItemState::lisActive));

    bool any_pixel_coloring = false;
    for (size_t i = 0; i < sizeof(frame_buffer); i++) {
        if (frame_buffer[i] != 0) {
            any_pixel_coloring = true;
            break;
        }
    }
    CHECK_TRUE(any_pixel_coloring);
}

TEST(LogicComparatorEqTestsGroup, DoAction_skip_when_incoming_passive) {
    mock().expectNoCall("adc_read");

    Controller controller(NULL);
    IncomeRail incomeRail(&controller, 0, LogicItemState::lisPassive);

    TestableComparatorEq testable(42, MapIO::AI, &incomeRail);

    CHECK_FALSE(testable.DoAction(false, LogicItemState::lisActive));
    CHECK_EQUAL(LogicItemState::lisPassive, *testable.PublicMorozov_Get_state());
}

TEST(LogicComparatorEqTestsGroup, DoAction_change_state_to_active) {
    volatile uint16_t adc = 49 / 0.1;
    mock()
        .expectNCalls(3, "adc_read")
        .withOutputParameterReturning("adc", (const void *)&adc, sizeof(adc));

    Controller controller(NULL);
    IncomeRail incomeRail(&controller, 0, LogicItemState::lisActive);

    TestableComparatorEq testable(50 / 0.4, MapIO::AI, &incomeRail);

    CHECK_FALSE(testable.DoAction(false, LogicItemState::lisActive));
    CHECK_EQUAL(LogicItemState::lisPassive, *testable.PublicMorozov_Get_state());

    adc = 51 / 0.1;
    CHECK_FALSE(testable.DoAction(false, LogicItemState::lisActive));
    CHECK_EQUAL(LogicItemState::lisPassive, *testable.PublicMorozov_Get_state());

    adc = 50 / 0.1;
    CHECK_TRUE(testable.DoAction(false, LogicItemState::lisActive));
    CHECK_EQUAL(LogicItemState::lisActive, *testable.PublicMorozov_Get_state());
}

TEST(LogicComparatorEqTestsGroup, DoAction_change_state_to_passive) {
    volatile uint16_t adc = 50 / 0.1;
    mock()
        .expectNCalls(2, "adc_read")
        .withOutputParameterReturning("adc", (const void *)&adc, sizeof(adc));

    Controller controller(NULL);
    IncomeRail incomeRail(&controller, 0, LogicItemState::lisActive);

    TestableComparatorEq testable(50 / 0.4, MapIO::AI, &incomeRail);
    CHECK_TRUE(testable.DoAction(false, LogicItemState::lisActive));
    CHECK_EQUAL(LogicItemState::lisActive, *testable.PublicMorozov_Get_state());

    adc = 49 / 0.1;
    CHECK_TRUE(testable.DoAction(false, LogicItemState::lisActive));
    CHECK_EQUAL(LogicItemState::lisPassive, *testable.PublicMorozov_Get_state());
}
