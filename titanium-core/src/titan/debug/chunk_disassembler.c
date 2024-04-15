
#include <titan/debug/chunk_disassembler.h>

// TODO: Unit test disassembler...

static ATTR_FORCE_INLINE chunk_size_t op_print(FILE *out, const char *name, chunk_size_t offset)
{
    fprintf(out, "%-10s", name);
    return offset + 1;
}

static ATTR_FORCE_INLINE chunk_size_t op_print_with_8i(FILE *out, const char *name, chunk_size_t offset, const Chunk *chunk)
{
    uint8_t value = chunk_read_uint8_code(chunk, offset + (chunk_size_t)1);
    fprintf(out, "%-10s 0x%X", name, value);
    return offset + 1 + sizeof(uint8_t);
}

static ATTR_FORCE_INLINE chunk_size_t op_print_with_16i(FILE *out, const char *name, chunk_size_t offset, const Chunk *chunk)
{
    uint16_t value = chunk_read_uint16_code(chunk, offset + (chunk_size_t)1);
    fprintf(out, "%-10s 0x%X", name, value);
    return offset + 1 + sizeof(uint16_t);
}

static ATTR_FORCE_INLINE chunk_size_t op_print_with_chunk_idx(FILE *out, const char *name, chunk_size_t offset, const Chunk *chunk)
{
    chunk_size_t value = chunk_read_chunk_idx(chunk, offset + (chunk_size_t)1);
    fprintf(out, "%-10s 0x%X", name, value);
    return offset + 1 + sizeof(chunk_size_t);
}

static ATTR_FORCE_INLINE chunk_size_t op_print_with_8p(FILE *out, const char *name, chunk_size_t offset, const Chunk *chunk)
{
    pool_size_t index = chunk->instructions[offset + 1];
    uint8_t value = chunk_read_uint8_pool(chunk, index);
    fprintf(out, "%-10s @[0x%02X] (0x%X)", name, index, value);
    return offset + 1 + sizeof(pool_size_t);
}

static ATTR_FORCE_INLINE chunk_size_t op_print_with_16p(FILE *out, const char *name, chunk_size_t offset, const Chunk *chunk)
{
    pool_size_t index = chunk->instructions[offset + 1];
    uint16_t value = chunk_read_uint16_pool(chunk, index);
    fprintf(out, "%-10s @[0x%02X] (0x%X)", name, index, value);
    return offset + 1 + sizeof(pool_size_t);
}

static ATTR_FORCE_INLINE chunk_size_t op_print_with_32p(FILE *out, const char *name, chunk_size_t offset, const Chunk *chunk)
{
    pool_size_t index = chunk->instructions[offset + 1];
    uint32_t value = chunk_read_uint32_pool(chunk, index);
    fprintf(out, "%-10s @[0x%02X] (0x%X)", name, index, value);
    return offset + 1 + sizeof(pool_size_t);
}

static ATTR_FORCE_INLINE chunk_size_t op_print_with_64p(FILE *out, const char *name, chunk_size_t offset, const Chunk *chunk)
{
    pool_size_t index = chunk->instructions[offset + 1];
    uint64_t value = chunk_read_uint64_pool(chunk, index);
    fprintf(out, "%-10s @[0x%02X] (0x%llX)", name, index, value);
    return offset + 1 + sizeof(pool_size_t);
}

void chunk_pretty_print(FILE *out, const Chunk *chunk, const char *name)
{
    fprintf(out, "===== %s =====\n", name);

    for (chunk_size_t offset = 0; offset < chunk->size;)
    {
        offset = opcode_pretty_print(out, chunk, offset);
        fprintf(out, "\n");
    }
}

chunk_size_t opcode_pretty_print(FILE *out, const Chunk *chunk, chunk_size_t offset)
{
    fprintf(out, "%04x", offset);

    if (offset > 0 && chunk->dbg_lines[offset] == chunk->dbg_lines[offset - 1])
        fprintf(out, "   | ");

    else
        fprintf(out, "%4d ", chunk->dbg_lines[offset]);


    uint8_t instruction = chunk->instructions[offset];

    switch (instruction)
    {
    case TI_I32TOI8:  return op_print(out, "TI_I32TOI8", offset);
    case TI_I32TOI16: return op_print(out, "TI_I32TOI16", offset);
    case TI_I64TOI32: return op_print(out, "TI_I64TOI32", offset);
    case TI_F64TOF32: return op_print(out, "TI_F64TOF32", offset);

    case  TI_PUSH8I: return op_print_with_8i(out, "TI_PUSH8I", offset, chunk);
    case TI_PUSH16I: return op_print_with_16i(out, "TI_PUSH16I", offset, chunk);

    case  TI_PUSH8P: return op_print_with_8p(out, "TI_PUSH8P", offset, chunk);
    case TI_PUSH16P: return op_print_with_16p(out, "TI_PUSH16P", offset, chunk);
    case TI_PUSH32P: return op_print_with_32p(out, "TI_PUSH32P", offset, chunk);
    case TI_PUSH64P: return op_print_with_64p(out, "TI_PUSH64P", offset, chunk);

    case TI_I32ADD: return op_print(out, "TI_I32ADD", offset);
    case TI_I32SUB: return op_print(out, "TI_I32SUB", offset);
    case TI_I32MUL: return op_print(out, "TI_I32MUL", offset);
    case TI_I32DIV: return op_print(out, "TI_I32DIV", offset);
    case TI_I32REM: return op_print(out, "TI_I32REM", offset);

    case TI_U32ADD: return op_print(out, "TI_U32ADD", offset);
    case TI_U32SUB: return op_print(out, "TI_U32SUB", offset);
    case TI_U32MUL: return op_print(out, "TI_U32MUL", offset);
    case TI_U32DIV: return op_print(out, "TI_U32DIV", offset);
    case TI_U32REM: return op_print(out, "TI_U32REM", offset);

    case TI_I64ADD: return op_print(out, "TI_I64ADD", offset);
    case TI_I64SUB: return op_print(out, "TI_I64SUB", offset);
    case TI_I64MUL: return op_print(out, "TI_I64MUL", offset);
    case TI_I64DIV: return op_print(out, "TI_I64DIV", offset);
    case TI_I64REM: return op_print(out, "TI_I64REM", offset);

    case TI_U64ADD: return op_print(out, "TI_U64ADD", offset);
    case TI_U64SUB: return op_print(out, "TI_U64SUB", offset);
    case TI_U64MUL: return op_print(out, "TI_U64MUL", offset);
    case TI_U64DIV: return op_print(out, "TI_U64DIV", offset);
    case TI_U64REM: return op_print(out, "TI_U64REM", offset);

    case TI_F32ADD: return op_print(out, "TI_F32ADD", offset);
    case TI_F32SUB: return op_print(out, "TI_F32SUB", offset);
    case TI_F32MUL: return op_print(out, "TI_F32MUL", offset);
    case TI_F32DIV: return op_print(out, "TI_F32DIV", offset);

    case TI_F64ADD: return op_print(out, "TI_F64ADD", offset);
    case TI_F64SUB: return op_print(out, "TI_F64SUB", offset);
    case TI_F64MUL: return op_print(out, "TI_F64MUL", offset);
    case TI_F64DIV: return op_print(out, "TI_F64DIV", offset);

    case TI_I32NEG: return op_print(out, "TI_I32NEG", offset);
    case TI_I64NEG: return op_print(out, "TI_I64NEG", offset);
    case TI_F32NEG: return op_print(out, "TI_F32NEG", offset);
    case TI_F64NEG: return op_print(out, "TI_F64NEG", offset);

    case TI_I32CMP: return op_print(out, "TI_I32CMP", offset);
    case TI_I64CMP: return op_print(out, "TI_I64CMP", offset);
    case TI_U32CMP: return op_print(out, "TI_U32CMP", offset);
    case TI_U64CMP: return op_print(out, "TI_U64CMP", offset);
    case TI_F32CMP: return op_print(out, "TI_F32CMP", offset);
    case TI_F64CMP: return op_print(out, "TI_F64CMP", offset);
    case TI_I8CMPZ: return op_print(out, "TI_I8CMPZ", offset);

    case TI_JMP:    return op_print_with_chunk_idx(out, "TI_JMP", offset, chunk);
    case TI_JLE:    return op_print_with_chunk_idx(out, "TI_JLE", offset, chunk);
    case TI_JLT:    return op_print_with_chunk_idx(out, "TI_JLT", offset, chunk);
    case TI_JEQ:    return op_print_with_chunk_idx(out, "TI_JEQ", offset, chunk);
    case TI_JGT:    return op_print_with_chunk_idx(out, "TI_JGT", offset, chunk);
    case TI_JGE:    return op_print_with_chunk_idx(out, "TI_JGE", offset, chunk);
    case TI_JNZ:    return op_print_with_chunk_idx(out, "TI_JNZ", offset, chunk);

    case TI_RETURN: return op_print(out, "TI_RETURN", offset);
    }

    return op_print(out, "!UNKNOWN_OP_CODE!", offset);
}

