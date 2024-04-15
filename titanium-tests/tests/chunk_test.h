
#include <unity.h>

#include <titan/chunk/chunk.h>
#include <titan/opcodes.h>

Chunk chunk;

void test_chunk_init()
{
    chunk_init(&chunk);

    TEST_ASSERT_EQUAL_MESSAGE(0, chunk.capacity, "[chunk capacity]");
    TEST_ASSERT_EQUAL_MESSAGE(0, chunk.size, "[chunk size]");
    TEST_ASSERT_NULL_MESSAGE(chunk.instructions, "[chunk instructions]");

    TEST_ASSERT_EQUAL_MESSAGE(0, chunk.datapool.capacity, "[datapool capacity]");
    TEST_ASSERT_EQUAL_MESSAGE(0, chunk.datapool.size, "[datapool size]");
    TEST_ASSERT_NULL_MESSAGE(chunk.datapool.values, "[datapool values]");

    chunk_free(&chunk);
}

void test_chunk_free()
{
    chunk_init(&chunk);
    chunk_free(&chunk);

    TEST_ASSERT_EQUAL_MESSAGE(0, chunk.capacity, "[chunk capacity]");
    TEST_ASSERT_EQUAL_MESSAGE(0, chunk.size, "[chunk size]");
    TEST_ASSERT_NULL_MESSAGE(chunk.instructions, "[chunk instructions]");

    TEST_ASSERT_EQUAL_MESSAGE(0, chunk.datapool.capacity, "[datapool capacity]");
    TEST_ASSERT_EQUAL_MESSAGE(0, chunk.datapool.size, "[datapool size]");
    TEST_ASSERT_NULL_MESSAGE(chunk.datapool.values, "[datapool values]");
}


void test_chunk_read_and_write_uint8_code()
{
    chunk_init(&chunk);

    chunk_size_t idx = chunk_write_uint8_code(&chunk, 0x12, 420);
    uint8_t val1 = chunk_read_uint8_code(&chunk, idx);

    const uint8_t *instructions = chunk.instructions;
    uint8_t val2 = chunk_next_inst_uint8(&instructions);

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x12, val1, "[chunk_read_uint8_code]");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x12, val2, "[chunk_next_inst_uint8 (value check)]");
    TEST_ASSERT_EQUAL_MESSAGE(chunk.instructions + sizeof(uint8_t), instructions, "[chunk_next_inst_uint8 (IP check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint8_t), chunk.size, "[chunk size]");

    chunk_free(&chunk);
}

void test_chunk_read_and_write_uint16_code()
{
    chunk_init(&chunk);

    chunk_size_t idx = chunk_write_uint16_code(&chunk, 0x1234, 420);
    uint16_t val1 = chunk_read_uint16_code(&chunk, idx);

    const uint8_t *instructions = chunk.instructions;
    uint16_t val2 = chunk_next_inst_uint16(&instructions);

    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x1234, val1, "[chunk_read_uint16_code]");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x1234, val2, "[chunk_next_inst_uint16 (value check)]");
    TEST_ASSERT_EQUAL_MESSAGE(chunk.instructions + sizeof(uint16_t), instructions, "[chunk_next_inst_uint16 (IP check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint16_t), chunk.size, "[chunk size]");

    chunk_free(&chunk);
}

void test_chunk_read_and_write_uint32_code()
{
    chunk_init(&chunk);

    chunk_size_t idx = chunk_write_uint32_code(&chunk, 0x12345678, 420);
    uint32_t val1 = chunk_read_uint32_code(&chunk, idx);

    const uint8_t *instructions = chunk.instructions;
    uint32_t val2 = chunk_next_inst_uint32(&instructions);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x12345678, val1, "[chunk_read_uint32_code]");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x12345678, val2, "[chunk_next_inst_uint32 (value check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint32_t), chunk.size, "[chunk_next_inst_uint16 (IP check)]");
    TEST_ASSERT_EQUAL(chunk.instructions + sizeof(uint32_t), instructions);

    chunk_free(&chunk);
}

void test_chunk_read_and_write_uint64_code()
{
    chunk_init(&chunk);

    chunk_size_t idx = chunk_write_uint64_code(&chunk, 0x1234567887654321, 420);
    uint64_t val1 = chunk_read_uint64_code(&chunk, idx);

    const uint8_t *instructions = chunk.instructions;
    uint64_t val2 = chunk_next_inst_uint64(&instructions);

    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0x1234567887654321, val1, "[chunk_read_uint64_code]");
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0x1234567887654321, val2, "[chunk_next_inst_uint64 (value check)]");
    TEST_ASSERT_EQUAL_MESSAGE(chunk.instructions + sizeof(uint64_t), instructions, "[chunk_next_inst_uint64 (IP check)]");
    TEST_ASSERT_EQUAL(sizeof(uint64_t), chunk.size);

    chunk_free(&chunk);
}

void test_chunk_read_and_write_pool_idx()
{
    chunk_init(&chunk);

    chunk_size_t idx = chunk_write_pool_idx(&chunk, 0x69, 420);
    pool_size_t val1 = chunk_read_pool_idx(&chunk, idx);

    const uint8_t *instructions = chunk.instructions;
    pool_size_t val2 = chunk_next_inst_pool_idx(&instructions);

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x69, val1, "[chunk_read_pool_idx]");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x69, val2, "[chunk_next_inst_pool_idx (value check)]");
    TEST_ASSERT_EQUAL_MESSAGE(chunk.instructions + sizeof(pool_size_t), instructions, "[chunk_next_inst_pool_idx (IP check)]");
    TEST_ASSERT_EQUAL(sizeof(pool_size_t), chunk.size);

    chunk_free(&chunk);
}

void test_chunk_read_and_write_chunk_idx()
{
    chunk_init(&chunk);

    chunk_size_t idx = chunk_write_chunk_idx(&chunk, 0x69, 420);
    chunk_size_t val1 = chunk_read_chunk_idx(&chunk, idx);

    const uint8_t *instructions = chunk.instructions;
    pool_size_t val2 = chunk_next_inst_pool_idx(&instructions);

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x69, val1, "[chunk_read_pool_idx]");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x69, val2, "[chunk_next_inst_pool_idx (value check)]");
    TEST_ASSERT_EQUAL_MESSAGE(chunk.instructions + sizeof(pool_size_t), instructions, "[chunk_next_inst_pool_idx (IP check)]");
    TEST_ASSERT_EQUAL(sizeof(pool_size_t), chunk.size);

    chunk_free(&chunk);
}

void test_chunk_read_and_write_opcode()
{
    chunk_init(&chunk);

    chunk_size_t idx = chunk_write_pool_idx(&chunk, TI_F32ADD, 420);
    TiOpCode val1 = chunk_read_opcode(&chunk, idx);

    const uint8_t *instructions = chunk.instructions;
    TiOpCode val2 = chunk_next_inst_opcode(&instructions);

    TEST_ASSERT_EQUAL_HEX_MESSAGE(TI_F32ADD, val1, "[chunk_read_opcode]");
    TEST_ASSERT_EQUAL_HEX_MESSAGE(TI_F32ADD, val2, "[chunk_next_inst_opcode (value check)]");
    TEST_ASSERT_EQUAL_MESSAGE(chunk.instructions + sizeof(TiOpCode), instructions, "[chunk_next_inst_opcode (IP check)]");
    TEST_ASSERT_EQUAL(sizeof(TiOpCode), chunk.size);

    chunk_free(&chunk);
}


void test_chunk_read_and_write_uint8_pool()
{
    chunk_init(&chunk);

    pool_size_t idx = chunk_write_uint8_pool(&chunk, 0x12);
    uint8_t val = chunk_read_uint8_pool(&chunk, idx);

    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0x12, val, "[chunk_read_uint8_pool (value check)]");
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(0, idx, "[chunk_read_uint8_pool (idx check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint8_t), chunk.datapool.size, "[datapool size]");

    idx = chunk_write_uint8_pool(&chunk, 0x34);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(sizeof(uint8_t), idx, "[chunk_read_uint8_pool (idx check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint8_t) * 2, chunk.datapool.size, "[datapool size]");

    chunk_free(&chunk);
}

void test_chunk_read_and_write_uint16_pool()
{
    chunk_init(&chunk);

    pool_size_t idx = chunk_write_uint16_pool(&chunk, 0x1234);
    uint16_t val = chunk_read_uint16_pool(&chunk, idx);

    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0x1234, val, "[chunk_read_uint16_pool (value check)]");
    TEST_ASSERT_EQUAL_HEX16_MESSAGE(0, idx, "[chunk_read_uint16_pool (idx check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint16_t), chunk.datapool.size, "[datapool size]");

    idx = chunk_write_uint16_pool(&chunk, 0x5678);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(sizeof(uint16_t), idx, "[chunk_read_uint16_pool (idx check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint16_t) * 2, chunk.datapool.size, "[datapool size]");

    chunk_free(&chunk);
}

void test_chunk_read_and_write_uint32_pool()
{
    chunk_init(&chunk);

    pool_size_t idx = chunk_write_uint32_pool(&chunk, 0x12345678);
    uint32_t val = chunk_read_uint32_pool(&chunk, idx);

    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0x12345678, val, "[chunk_read_uint32_pool (value check)]");
    TEST_ASSERT_EQUAL_HEX32_MESSAGE(0, idx, "[chunk_read_uint32_pool (idx check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint32_t), chunk.datapool.size, "[datapool size]");

    idx = chunk_write_uint32_pool(&chunk, 0x87654321);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(sizeof(uint32_t), idx, "[chunk_read_uint32_pool (idx check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint32_t) * 2, chunk.datapool.size, "[datapool size]");

    chunk_free(&chunk);
}

void test_chunk_read_and_write_uint64_pool()
{
    chunk_init(&chunk);

    pool_size_t idx = chunk_write_uint64_pool(&chunk, 0x1234567887654321);
    uint64_t val = chunk_read_uint64_pool(&chunk, idx);

    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0x1234567887654321, val, "[chunk_read_uint64_pool (value check)]");
    TEST_ASSERT_EQUAL_HEX64_MESSAGE(0, idx, "[chunk_read_uint64_pool (idx check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint64_t), chunk.datapool.size, "[datapool size]");

    idx = chunk_write_uint64_pool(&chunk, 0x1234567887654321);
    TEST_ASSERT_EQUAL_HEX8_MESSAGE(sizeof(uint64_t), idx, "[chunk_read_uint64_pool (idx check)]");
    TEST_ASSERT_EQUAL_MESSAGE(sizeof(uint64_t) * 2, chunk.datapool.size, "[datapool size]");

    chunk_free(&chunk);
}


void test_chunk_get_loc()
{
    chunk_init(&chunk);

    chunk_size_t idx = chunk_write_uint8_code(&chunk, 0x12, 420);
    uint16_t loc = chunk_get_loc(&chunk, idx);

    TEST_ASSERT_EQUAL_HEX16_MESSAGE(420, loc, "[chunk_get_loc]");

    chunk_free(&chunk);
}


void chunk_tests_main()
{
    RUN_TEST(test_chunk_init);
    RUN_TEST(test_chunk_free);

    RUN_TEST(test_chunk_read_and_write_uint8_code);
    RUN_TEST(test_chunk_read_and_write_uint16_code);
    RUN_TEST(test_chunk_read_and_write_uint32_code);
    RUN_TEST(test_chunk_read_and_write_uint64_code);
    RUN_TEST(test_chunk_read_and_write_pool_idx);
    RUN_TEST(test_chunk_read_and_write_opcode);

    RUN_TEST(test_chunk_read_and_write_uint8_pool);
    RUN_TEST(test_chunk_read_and_write_uint16_pool);
    RUN_TEST(test_chunk_read_and_write_uint32_pool);
    RUN_TEST(test_chunk_read_and_write_uint64_pool);

    RUN_TEST(test_chunk_get_loc);
}
