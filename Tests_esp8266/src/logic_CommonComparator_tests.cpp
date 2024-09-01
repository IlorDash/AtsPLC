#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "main/LogicProgram/Inputs/CommonComparator.h"

static uint8_t frame_buffer[DISPLAY_WIDTH * DISPLAY_HEIGHT / 8] = {};
TEST_GROUP(LogicCommonComparatorTestsGroup){
    //
    TEST_SETUP(){ memset(frame_buffer, 0, sizeof(frame_buffer));
}

TEST_TEARDOWN() {
}
}
;

namespace {
    static const Bitmap bitmap = { //
        { 16,                      // width
          16 },                    // height
        { 0xFF, 0x00, 0x00, 0x0A, 0x0A, 0x0A, 0x0A, 0x00, 0x0A, 0x0A, 0x0A,
          0x0A, 0x00, 0x00, 0xFF, 0x80, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00,
          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x01 }
    };

    class TestableCommonComparator : public CommonComparator {
      public:
        TvElementType elementType;

        TestableCommonComparator(uint8_t ref_percent04,
                                 const MapIO io_adr,
                                 TvElementType elementType = TvElementType::et_ComparatorLs)
            : CommonComparator(ref_percent04, io_adr) {
            this->elementType = elementType;
        }
        virtual ~TestableCommonComparator() {
        }

        const Bitmap *GetCurrentBitmap() {
            return &bitmap;
        }

        bool CompareFunction() override {
            return true;
        }
        MapIO PublicMorozov_Get_io_adr() {
            return io_adr;
        }
        uint8_t PublicMorozov_GetReference() {
            return ref_percent04;
        }
        TvElementType GetElementType() override final {
            return elementType;
        }
    };
} // namespace

TEST(LogicCommonComparatorTestsGroup, Reference_in_limit_0_to_250) {
    TestableCommonComparator testable_0(0, MapIO::DI);
    CHECK_EQUAL(0, testable_0.PublicMorozov_GetReference());

    TestableCommonComparator testable_100(100, MapIO::DI);
    CHECK_EQUAL(100, testable_100.PublicMorozov_GetReference());

    TestableCommonComparator testable_250(250, MapIO::DI);
    CHECK_EQUAL(250, testable_250.PublicMorozov_GetReference());

    TestableCommonComparator testable_251(251, MapIO::DI);
    CHECK_EQUAL(250, testable_251.PublicMorozov_GetReference());
}

TEST(LogicCommonComparatorTestsGroup, Render) {
    TestableCommonComparator testable(0, MapIO::DI);

    Point start_point = { 0, INCOME_RAIL_TOP };
    CHECK_TRUE(testable.Render(frame_buffer, LogicItemState::lisActive, &start_point));

    bool any_pixel_coloring = false;
    for (size_t i = 0; i < sizeof(frame_buffer); i++) {
        if (frame_buffer[i] != 0) {
            any_pixel_coloring = true;
            break;
        }
    }
    CHECK_TRUE(any_pixel_coloring);
    CHECK_EQUAL(32, start_point.x);
}

TEST(LogicCommonComparatorTestsGroup, Serialize) {
    uint8_t buffer[256] = {};
    TestableCommonComparator testable(42, MapIO::V2);

    size_t writed = testable.Serialize(buffer, sizeof(buffer));
    CHECK_EQUAL(3, writed);

    CHECK_EQUAL(TvElementType::et_ComparatorLs, *((TvElementType *)&buffer[0]));
    CHECK_EQUAL(42, *((uint8_t *)&buffer[1]));
    CHECK_EQUAL(MapIO::V2, *((MapIO *)&buffer[2]));
}

TEST(LogicCommonComparatorTestsGroup, Serialize_just_for_obtain_size) {
    TestableCommonComparator testable(50 / 0.4, MapIO::V2);

    size_t writed = testable.Serialize(NULL, SIZE_MAX);
    CHECK_EQUAL(3, writed);
}

TEST(LogicCommonComparatorTestsGroup, Serialize_to_small_buffer_return_zero) {
    uint8_t buffer[1] = {};
    TestableCommonComparator testable(50 / 0.4, MapIO::V2);

    size_t writed = testable.Serialize(buffer, sizeof(buffer));
    CHECK_EQUAL(0, writed);
}

TEST(LogicCommonComparatorTestsGroup, Deserialize) {
    uint8_t buffer[256] = {};
    *((TvElementType *)&buffer[0]) = TvElementType::et_ComparatorLs;
    *((uint8_t *)&buffer[1]) = 42;
    *((MapIO *)&buffer[2]) = MapIO::V3;

    TestableCommonComparator testable(19, MapIO::DI);

    size_t readed = testable.Deserialize(&buffer[1], sizeof(buffer) - 1);
    CHECK_EQUAL(2, readed);

    CHECK_EQUAL(42, testable.PublicMorozov_GetReference());
    CHECK_EQUAL(MapIO::V3, testable.PublicMorozov_Get_io_adr());
}

TEST(LogicCommonComparatorTestsGroup, Deserialize_with_small_buffer_return_zero) {
    uint8_t buffer[0] = {};
    *((TvElementType *)&buffer[0]) = TvElementType::et_ComparatorLs;

    TestableCommonComparator testable(50 / 0.4, MapIO::V2);

    size_t readed = testable.Deserialize(buffer, sizeof(buffer));
    CHECK_EQUAL(0, readed);
}

TEST(LogicCommonComparatorTestsGroup, Deserialize_with_wrong_io_adr_return_zero) {
    uint8_t buffer[256] = {};
    *((TvElementType *)&buffer[0]) = TvElementType::et_ComparatorLs;
    *((uint8_t *)&buffer[1]) = 42;

    TestableCommonComparator testable(50 / 0.4, MapIO::V1);

    *((MapIO *)&buffer[2]) = (MapIO)(MapIO::DI - 1);
    size_t readed = testable.Deserialize(&buffer[1], sizeof(buffer) - 1);
    CHECK_EQUAL(0, readed);

    *((MapIO *)&buffer[2]) = (MapIO)(MapIO::V4 + 1);
    readed = testable.Deserialize(&buffer[1], sizeof(buffer) - 1);
    CHECK_EQUAL(0, readed);

    *((MapIO *)&buffer[2]) = MapIO::DI;
    readed = testable.Deserialize(&buffer[1], sizeof(buffer) - 1);
    CHECK_EQUAL(2, readed);
}

TEST(LogicCommonComparatorTestsGroup, Deserialize_with_large_reference_return_zero) {
    uint8_t buffer[256] = {};
    *((TvElementType *)&buffer[0]) = TvElementType::et_ComparatorLs;
    *((MapIO *)&buffer[2]) = MapIO::V3;

    TestableCommonComparator testable(0, MapIO::V1);

    *((uint8_t *)&buffer[1]) = LogicElement::MaxValue + 1;
    size_t readed = testable.Deserialize(&buffer[1], sizeof(buffer) - 1);
    CHECK_EQUAL(0, readed);

    *((uint8_t *)&buffer[1]) = LogicElement::MaxValue;
    readed = testable.Deserialize(&buffer[1], sizeof(buffer) - 1);
    CHECK_EQUAL(2, readed);
}