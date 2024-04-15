
#include <unity.h>
#include <titan/datapool/datapool.h>

DataPool pool;

void test_datapool_init()
{
    datapool_init(&pool);

    TEST_ASSERT_EQUAL_MESSAGE(0, pool.capacity, "[datapool capacity]");
    TEST_ASSERT_EQUAL_MESSAGE(0, pool.size, "[datapool size]");
    TEST_ASSERT_NULL_MESSAGE(pool.values, "[datapool values]");

    datapool_free(&pool);
}

void test_datapool_free()
{
    datapool_init(&pool);
    datapool_free(&pool);

    TEST_ASSERT_EQUAL_MESSAGE(0, pool.capacity, "[datapool capacity]");
    TEST_ASSERT_EQUAL_MESSAGE(0, pool.size, "[datapool size]");
    TEST_ASSERT_NULL_MESSAGE(pool.values, "[datapool values]");
}

void test_datapool_write()
{
    datapool_init(&pool);

    datapool_write_pool(&pool, 0x69);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x69, pool.values[0], "[datapool_write_pool]");
    TEST_ASSERT_EQUAL_MESSAGE(1, pool.size, "[datapool size]");

    datapool_free(&pool);
}

void datapool_tests_main()
{
    RUN_TEST(test_datapool_init);
    RUN_TEST(test_datapool_free);
    RUN_TEST(test_datapool_write);
}
