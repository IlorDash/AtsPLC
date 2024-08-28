#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main/LogicProgram/Inputs/IncomeRail.h"
#include "main/LogicProgram/Inputs/TimerSecs.cpp"
#include "main/LogicProgram/Inputs/TimerSecs.h"

static uint8_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8] = {};

TEST_GROUP(LogicTimerSecsTestsGroup){ //
                                      TEST_SETUP(){ memset(frame_buffer, 0, sizeof(frame_buffer));
}

TEST_TEARDOWN() {
}
}
;

namespace {
    class TestableTimerSecs : public TimerSecs {
      public:
        TestableTimerSecs(uint32_t delay_time_s, InputBase *incoming_item)
            : TimerSecs(delay_time_s, incoming_item) {
        }
        virtual ~TestableTimerSecs() {
        }

        uint64_t PublicMorozov_GetDelayTimeUs() {
            return delay_time_us;
        }
        uint8_t PublicMorozov_GetProgress(LogicItemState prev_elem_state) {
            return GetProgress(prev_elem_state);
        }
        uint8_t PublicMorozov_ProgressHasChanges(LogicItemState prev_elem_state) {
            return ProgressHasChanges(prev_elem_state);
        }
    };
} // namespace

TEST(LogicTimerSecsTestsGroup, Reference_in_limit_1_to_99999) {
    mock().expectNCalls(4, "esp_timer_get_time").ignoreOtherParameters();

    Controller controller(NULL);
    IncomeRail incomeRail0(&controller, 0, LogicItemState::lisActive);
    TestableTimerSecs testable_0(0, &incomeRail0);
    CHECK_EQUAL(1 * 1000000LL, testable_0.PublicMorozov_GetDelayTimeUs());

    TestableTimerSecs testable_99998(99998, &incomeRail0);
    CHECK_EQUAL(99998 * 1000000LL, testable_99998.PublicMorozov_GetDelayTimeUs());

    TestableTimerSecs testable_99999(99999, &incomeRail0);
    CHECK_EQUAL(99999 * 1000000LL, testable_99999.PublicMorozov_GetDelayTimeUs());

    TestableTimerSecs testable_100000(100000, &incomeRail0);
    CHECK_EQUAL(99999 * 1000000LL, testable_100000.PublicMorozov_GetDelayTimeUs());
}

TEST(LogicTimerSecsTestsGroup, ProgressHasChanges_true_every_one_sec) {
    volatile uint64_t os_us = 0;
    mock()
        .expectNCalls(11, "esp_timer_get_time")
        .withOutputParameterReturning("os_us", (const void *)&os_us, sizeof(os_us));

    Controller controller(NULL);
    IncomeRail incomeRail0(&controller, 0, LogicItemState::lisActive);
    TestableTimerSecs testable(10, &incomeRail0);
    testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive);

    CHECK_FALSE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));

    os_us = 500000;
    CHECK_FALSE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));

    os_us = 1000000;
    CHECK_TRUE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));

    os_us = 1200000;
    CHECK_FALSE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));

    os_us = 2000000;
    CHECK_TRUE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));

    os_us = 2500000;
    CHECK_FALSE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));

    os_us = UINT64_MAX - 900000;
    CHECK_TRUE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));

    os_us = 100000 - 2;
    CHECK_FALSE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));

    os_us = 100000 - 1;
    CHECK_TRUE(testable.PublicMorozov_ProgressHasChanges(LogicItemState::lisActive));
}

TEST(LogicTimerSecsTestsGroup, success_render_with_zero_progress) {
    volatile uint64_t os_us = 0;
    mock()
        .expectNCalls(3, "esp_timer_get_time")
        .withOutputParameterReturning("os_us", (const void *)&os_us, sizeof(os_us));

    Controller controller(NULL);
    IncomeRail incomeRail0(&controller, 0, LogicItemState::lisActive);
    TestableTimerSecs testable(10, &incomeRail0);

    uint8_t percent04 = testable.PublicMorozov_GetProgress(LogicItemState::lisActive);
    CHECK_EQUAL(0, percent04);
    CHECK_TRUE(testable.Render(frame_buffer, LogicItemState::lisActive));
}