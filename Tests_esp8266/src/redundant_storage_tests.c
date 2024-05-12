#include "CppUTest/TestHarness_c.h"
#include "CppUTestExt/MockSupport_c.h"

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "redundant_storage.c"
#include "storage.c"

#define storage_0_partition "storage_0"
#define storage_1_partition "storage_1"
#define storage_0_path "/tmp/storage_0"
#define storage_1_path "/tmp/storage_1"
#define storage_name "settings"

static void create_storage_0() {
    mkdir(storage_0_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
static void create_storage_1() {
    mkdir(storage_1_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}
static void remove_storage_0() {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "rm -rf %s", storage_0_path);
    system(cmd);
}
static void remove_storage_1() {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "rm -rf %s", storage_1_path);
    system(cmd);
}
static bool storage_0_exists() {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/%s", storage_0_path, storage_name);
    struct stat st;
    return stat(filename, &st) == 0;
}
static bool storage_1_exists() {
    char filename[256];
    snprintf(filename, sizeof(filename), "%s/%s", storage_1_path, storage_name);
    struct stat st;
    return stat(filename, &st) == 0;
}

TEST_GROUP_C_SETUP(RedundantStorageTestsGroup) {
    create_storage_0();
    create_storage_1();
}
TEST_GROUP_C_TEARDOWN(RedundantStorageTestsGroup) {
    remove_storage_0();
    remove_storage_1();
}

TEST_C(RedundantStorageTestsGroup, load_if_clear_storage_return_NULL) {
    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();

    redundant_storage storage = redundant_storage_load(storage_0_partition,
                                                       storage_0_path,
                                                       storage_1_partition,
                                                       storage_1_path,
                                                       storage_name);
    CHECK_EQUAL_C_POINTER(NULL, storage.data);
    CHECK_EQUAL_C_UINT(0, storage.size);
}

TEST_C(RedundantStorageTestsGroup, store) {
    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();

    uint8_t data[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                       0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                       0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };

    redundant_storage storage;
    storage.data = data;
    storage.size = sizeof(data);

    redundant_storage_store(storage_0_partition,
                            storage_0_path,
                            storage_1_partition,
                            storage_1_path,
                            storage_name,
                            storage);
}

TEST_C(RedundantStorageTestsGroup, load) {
    uint8_t data[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a,
                       0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                       0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };

    redundant_storage storage;
    storage.data = data;
    storage.size = sizeof(data);

    mock_c()->disable();
    redundant_storage_store(storage_0_partition,
                            storage_0_path,
                            storage_1_partition,
                            storage_1_path,
                            storage_name,
                            storage);
    mock_c()->enable();

    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();

    storage = redundant_storage_load(storage_0_partition,
                                     storage_0_path,
                                     storage_1_partition,
                                     storage_1_path,
                                     storage_name);
    CHECK_EQUAL_C_MEMCMP(data, storage.data, sizeof(data));
    CHECK_EQUAL_C_UINT(sizeof(data), storage.size);
    free(storage.data);
}

TEST_C(RedundantStorageTestsGroup, second_storage_restored_when_load) {
    uint8_t data[] = { 0xff, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a };

    redundant_storage storage;
    storage.data = data;
    storage.size = sizeof(data);

    mock_c()->disable();
    redundant_storage_store(storage_0_partition,
                            storage_0_path,
                            storage_1_partition,
                            storage_1_path,
                            storage_name,
                            storage);
    mock_c()->enable();

    remove_storage_1();
    create_storage_1();
    CHECK_EQUAL_C_BOOL(true, storage_0_exists());
    CHECK_EQUAL_C_BOOL(false, storage_1_exists());

    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();

    storage = redundant_storage_load(storage_0_partition,
                                     storage_0_path,
                                     storage_1_partition,
                                     storage_1_path,
                                     storage_name);
    CHECK_EQUAL_C_MEMCMP(data, storage.data, sizeof(data));
    CHECK_EQUAL_C_UINT(sizeof(data), storage.size);
    free(storage.data);

    CHECK_EQUAL_C_BOOL(true, storage_0_exists());
    CHECK_EQUAL_C_BOOL(true, storage_1_exists());
}

TEST_C(RedundantStorageTestsGroup, first_storage_restored_when_load) {
    uint8_t data[] = { 0xee, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a };

    redundant_storage storage;
    storage.data = data;
    storage.size = sizeof(data);

    mock_c()->disable();
    redundant_storage_store(storage_0_partition,
                            storage_0_path,
                            storage_1_partition,
                            storage_1_path,
                            storage_name,
                            storage);
    mock_c()->enable();

    remove_storage_0();
    create_storage_0();
    CHECK_EQUAL_C_BOOL(false, storage_0_exists());
    CHECK_EQUAL_C_BOOL(true, storage_1_exists());

    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_register")->ignoreOtherParameters();
    mock_scope_c("storage_0")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();
    mock_scope_c("storage_1")->expectOneCall("esp_vfs_spiffs_unregister")->ignoreOtherParameters();

    storage = redundant_storage_load(storage_0_partition,
                                     storage_0_path,
                                     storage_1_partition,
                                     storage_1_path,
                                     storage_name);
    CHECK_EQUAL_C_MEMCMP(data, storage.data, sizeof(data));
    CHECK_EQUAL_C_UINT(sizeof(data), storage.size);
    free(storage.data);

    CHECK_EQUAL_C_BOOL(true, storage_0_exists());
    CHECK_EQUAL_C_BOOL(true, storage_1_exists());
}