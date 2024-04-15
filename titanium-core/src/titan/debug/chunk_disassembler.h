#ifndef TITAN_CHUNK_DISASSEMBLER
#define TITAN_CHUNK_DISASSEMBLER

#include <common.h>
#include <titan/chunk/chunk.h>
#include <titan/opcodes.h>

void         chunk_pretty_print(FILE *out, const Chunk *chunk, const char *name);
chunk_size_t opcode_pretty_print(FILE *out, const Chunk *chunk, chunk_size_t offset);

static ATTR_FORCE_INLINE void chunk_stdout_print(const Chunk *chunk, const char *name)
{
    chunk_pretty_print(stdout, chunk, name);
}

#endif
