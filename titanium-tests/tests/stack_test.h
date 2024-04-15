
#include <unity.h>
#include <util/stack/stack.h>

Stack stack;

void test_stack_init()
{
    stack_init(&stack);

    TEST_ASSERT_EQUAL(&stack.data[0], stack.sp);

    stack_free(&stack);
}

void test_stack_free()
{
    stack_init(&stack);
    stack_free(&stack);

    TEST_ASSERT_NULL(stack.sp);
}

void test_stack_push_and_pop_uint8()
{
    stack_init(&stack);

    stack_push_uint8(&stack, 0x12);
    stack_push_uint8(&stack, 0x34);

    TEST_ASSERT_EQUAL_MESSAGE(&stack.data[2], stack.sp, "[stack_push_uint8 (SP check)]");

    uint8_t val1 = stack_pop_uint8(&stack);
    uint8_t val2 = stack_pop_uint8(&stack);

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x34, val1, "[stack_pop_uint8]");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x12, val2, "[stack_pop_uint8]");

    stack_free(&stack);
}

void test_stack_push_and_pop_uint16()
{
    stack_init(&stack);

    stack_push_uint16(&stack, 0x1234);
    stack_push_uint16(&stack, 0x5678);

    TEST_ASSERT_EQUAL_MESSAGE(&stack.data[4], stack.sp, "[stack_push_uint16 (SP check)]");

    uint16_t val1 = stack_pop_uint16(&stack);
    uint16_t val2 = stack_pop_uint16(&stack);

    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x5678, val1, "[stack_pop_uint16]");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x1234, val2, "[stack_pop_uint16]");

    stack_free(&stack);
}

void test_stack_push_and_pop_uint32()
{
    stack_init(&stack);

    stack_push_uint32(&stack, 0x12345768);
    stack_push_uint32(&stack, 0x87654321);

    TEST_ASSERT_EQUAL_MESSAGE(&stack.data[8], stack.sp, "[stack_push_uint32 (SP check)]");

    uint32_t val1 = stack_pop_uint32(&stack);
    uint32_t val2 = stack_pop_uint32(&stack);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x87654321, val1, "[stack_pop_uint32]");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x12345768, val2, "[stack_pop_uint32]");

    stack_free(&stack);
}

void test_stack_push_and_pop_uint64()
{
    stack_init(&stack);

    stack_push_uint64(&stack, 0x1234576887654321);
    stack_push_uint64(&stack, 0x8765432112345678);

    TEST_ASSERT_EQUAL_MESSAGE(&stack.data[16], stack.sp, "[stack_push_uint64 (SP check)]");

    uint64_t val1 = stack_pop_uint64(&stack);
    uint64_t val2 = stack_pop_uint64(&stack);

    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0x8765432112345678, val1, "[stack_pop_uint64]");
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0x1234576887654321, val2, "[stack_pop_uint64]");

    stack_free(&stack);
}

void stack_tests_main()
{
    RUN_TEST(test_stack_init);
    RUN_TEST(test_stack_free);

    RUN_TEST(test_stack_push_and_pop_uint8);
    RUN_TEST(test_stack_push_and_pop_uint16);
    RUN_TEST(test_stack_push_and_pop_uint32);
    RUN_TEST(test_stack_push_and_pop_uint64);
}
