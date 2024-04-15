
#include <stdio.h>

#include <util/util.h>
#include <titan/opcodes.h>
#include <titan/chunk/chunk.h>
#include <titan/debug/chunk_disassembler.h>
#include <titan/titan.h>

#include <common.h>

int main(int argc, char const *argv[])
{
    titan_init();

    Chunk chunk;
    chunk_init(&chunk);

    // We store a 16-bit integer in the constant pool
    uint32_t v = 1;
    pool_size_t idx1 = chunk_write_uint32_pool(&chunk, v);

    // Push 1
    chunk_size_t start = chunk_write_opcode(&chunk, TI_PUSH32P, 123);
    chunk_write_pool_idx(&chunk, idx1, 123);

    // Majority of operators only work with 32b or 64b types
    chunk_write_opcode(&chunk, TI_I32NEG, 124);
    chunk_write_opcode(&chunk, TI_RETURN, 124);
    chunk_write_opcode(&chunk, TI_JMP, 125);
    chunk_write_chunk_idx(&chunk, start, 125);

    // Interpreting
    chunk_stdout_print(&chunk, "My Chunk");

    // Clean up
    titan_free();
    chunk_free(&chunk);

    return 0;
}
