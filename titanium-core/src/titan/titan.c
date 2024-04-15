
#include "titan.h"

#include <common.h>

#include <util/util.h>
#include <util/stack/stack.h>
#include <titan/opcodes.h>
#include <titan/datapool/datapool.h>
#include <titan/debug/chunk_disassembler.h>

#define SET_BIT(val, bit) (titan.cmp ^= ((val) << (bit)))
#define SET_LT_BIT(val) SET_BIT(val, LT_BIT)
#define SET_EQ_BIT(val) SET_BIT(val, EQ_BIT)
#define SET_GT_BIT(val) SET_BIT(val, GT_BIT)
#define SET_NZ_BIT(val) SET_BIT(val, NZ_BIT)

#define GET_BIT(bit) (!!(titan.cmp & (1 << bit)))

// TODO: Unit test titan VM...

typedef struct
{
    Stack stack;
    const Chunk *chunk;

    // Registers
    uint8_t cmp;
    const uint8_t *ip;
}
TitanVM;

static TitanVM titan;

void titan_init()
{
    stack_init(&titan.stack);
}

void titan_free()
{
    stack_free(&titan.stack);
}

void titan_pretty_print_registers(FILE *out)
{
    fprintf(out, "\t\t\t    IP: %p\n", titan.ip);
    fprintf(out, "\t\t\t    SP: %p\n", titan.stack.sp);
    fprintf(out, "\t\t\t   CMP: "BYTE_TO_BINARY_STR_PATTERN"\n", BYTE_TO_BINARY_STR(titan.cmp));
}

//

static ATTR_FORCE_INLINE void op_i32toi8()
{
    uint32_t value = stack_pop_uint32(&titan.stack);
    stack_push_uint8(&titan.stack, i2b(value));
}

static ATTR_FORCE_INLINE void op_i32toi16()
{
    uint32_t value = stack_pop_uint32(&titan.stack);
    stack_push_uint16(&titan.stack, i2s(value));
}

static ATTR_FORCE_INLINE void op_i64toi32()
{
    uint64_t value = stack_pop_uint64(&titan.stack);
    stack_push_uint32(&titan.stack, l2i(value));
}

static ATTR_FORCE_INLINE void op_f64tof32()
{
    double value = i2f(stack_pop_uint64(&titan.stack));
    stack_push_uint32(&titan.stack, f2i(d2f(value)));
}

//

static ATTR_FORCE_INLINE void op_push8i()
{
    uint8_t value = chunk_next_inst_uint8(&titan.ip);
    stack_push_uint8(&titan.stack, value);
}

static ATTR_FORCE_INLINE void op_push16i()
{
    uint16_t value = chunk_next_inst_uint16(&titan.ip);
    stack_push_uint16(&titan.stack, value);
}

//

static ATTR_FORCE_INLINE void op_push8p()
{
    pool_size_t index = chunk_next_inst_pool_idx(&titan.ip);
    uint8_t value = chunk_read_uint8_pool(titan.chunk, index);
    stack_push_uint8(&titan.stack, value);
}

static ATTR_FORCE_INLINE void op_push16p()
{
    pool_size_t index = chunk_next_inst_pool_idx(&titan.ip);
    uint16_t value = chunk_read_uint16_pool(titan.chunk, index);
    stack_push_uint16(&titan.stack, value);
}

static ATTR_FORCE_INLINE void op_push32p()
{
    pool_size_t index = chunk_next_inst_pool_idx(&titan.ip);
    uint32_t value = chunk_read_uint32_pool(titan.chunk, index);
    stack_push_uint32(&titan.stack, value);
}

static ATTR_FORCE_INLINE void op_push64p()
{
    pool_size_t index = chunk_next_inst_pool_idx(&titan.ip);
    uint64_t value = chunk_read_uint64_pool(titan.chunk, index);
    stack_push_uint64(&titan.stack, value);
}

//

static ATTR_FORCE_INLINE void op_i32add()
{
    int32_t v2 = u2s(stack_pop_uint32(&titan.stack));
    int32_t v1 = u2s(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, s2u(v1 + v2));
}

static ATTR_FORCE_INLINE void op_i32sub()
{
    int32_t v2 = u2s(stack_pop_uint32(&titan.stack));
    int32_t v1 = u2s(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, s2u(v1 - v2));
}

static ATTR_FORCE_INLINE void op_i32mul()
{
    int32_t v2 = u2s(stack_pop_uint32(&titan.stack));
    int32_t v1 = u2s(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, s2u(v1 * v2));
}

static ATTR_FORCE_INLINE void op_i32div()
{
    int32_t v2 = u2s(stack_pop_uint32(&titan.stack));
    int32_t v1 = u2s(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, s2u(v1 / v2));
}

static ATTR_FORCE_INLINE void op_i32rem()
{
    int32_t v2 = u2s(stack_pop_uint32(&titan.stack));
    int32_t v1 = u2s(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, s2u(v1 % v2));
}

//

static ATTR_FORCE_INLINE void op_u32add()
{
    uint32_t v2 = stack_pop_uint32(&titan.stack);
    uint32_t v1 = stack_pop_uint32(&titan.stack);
    stack_push_uint32(&titan.stack, v1 + v2);
}

static ATTR_FORCE_INLINE void op_u32sub()
{
    uint32_t v2 = stack_pop_uint32(&titan.stack);
    uint32_t v1 = stack_pop_uint32(&titan.stack);
    stack_push_uint32(&titan.stack, v1 - v2);
}

static ATTR_FORCE_INLINE void op_u32mul()
{
    uint32_t v2 = stack_pop_uint32(&titan.stack);
    uint32_t v1 = stack_pop_uint32(&titan.stack);
    stack_push_uint32(&titan.stack, v1 * v2);
}

static ATTR_FORCE_INLINE void op_u32div()
{
    uint32_t v2 = stack_pop_uint32(&titan.stack);
    uint32_t v1 = stack_pop_uint32(&titan.stack);
    stack_push_uint32(&titan.stack, v1 / v2);
}

static ATTR_FORCE_INLINE void op_u32rem()
{
    uint32_t v2 = stack_pop_uint32(&titan.stack);
    uint32_t v1 = stack_pop_uint32(&titan.stack);
    stack_push_uint32(&titan.stack, v1 % v2);
}

//

static ATTR_FORCE_INLINE void op_i64add()
{
    int64_t v2 = u2s(stack_pop_uint64(&titan.stack));
    int64_t v1 = u2s(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, s2u(v1 + v2));
}

static ATTR_FORCE_INLINE void op_i64sub()
{
    int64_t v2 = u2s(stack_pop_uint64(&titan.stack));
    int64_t v1 = u2s(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, s2u(v1 - v2));
}

static ATTR_FORCE_INLINE void op_i64mul()
{
    int64_t v2 = u2s(stack_pop_uint64(&titan.stack));
    int64_t v1 = u2s(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, s2u(v1 * v2));
}

static ATTR_FORCE_INLINE void op_i64div()
{
    int64_t v2 = u2s(stack_pop_uint64(&titan.stack));
    int64_t v1 = u2s(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, s2u(v1 / v2));
}

static ATTR_FORCE_INLINE void op_i64rem()
{
    int64_t v2 = u2s(stack_pop_uint64(&titan.stack));
    int64_t v1 = u2s(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, s2u(v1 % v2));
}


//

static ATTR_FORCE_INLINE void op_u64add()
{
    uint64_t v2 = stack_pop_uint64(&titan.stack);
    uint64_t v1 = stack_pop_uint64(&titan.stack);
    stack_push_uint64(&titan.stack, v1 + v2);
}

static ATTR_FORCE_INLINE void op_u64sub()
{
    uint64_t v2 = stack_pop_uint64(&titan.stack);
    uint64_t v1 = stack_pop_uint64(&titan.stack);
    stack_push_uint64(&titan.stack, v1 - v2);
}

static ATTR_FORCE_INLINE void op_u64mul()
{
    uint64_t v2 = stack_pop_uint64(&titan.stack);
    uint64_t v1 = stack_pop_uint64(&titan.stack);
    stack_push_uint64(&titan.stack, v1 * v2);
}

static ATTR_FORCE_INLINE void op_u64div()
{
    uint64_t v2 = stack_pop_uint64(&titan.stack);
    uint64_t v1 = stack_pop_uint64(&titan.stack);
    stack_push_uint64(&titan.stack, v1 / v2);
}

static ATTR_FORCE_INLINE void op_u64rem()
{
    uint64_t v2 = stack_pop_uint64(&titan.stack);
    uint64_t v1 = stack_pop_uint64(&titan.stack);
    stack_push_uint64(&titan.stack, v1 % v2);
}


//

static ATTR_FORCE_INLINE void op_f32add()
{
    float v2 = i2f(stack_pop_uint32(&titan.stack));
    float v1 = i2f(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, f2i(v1 + v2));
}

static ATTR_FORCE_INLINE void op_f32sub()
{
    float v2 = i2f(stack_pop_uint32(&titan.stack));
    float v1 = i2f(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, f2i(v1 - v2));
}

static ATTR_FORCE_INLINE void op_f32mul()
{
    float v2 = i2f(stack_pop_uint32(&titan.stack));
    float v1 = i2f(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, f2i(v1 * v2));
}

static ATTR_FORCE_INLINE void op_f32div()
{
    float v2 = i2f(stack_pop_uint32(&titan.stack));
    float v1 = i2f(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, f2i(v1 / v2));
}


//

static ATTR_FORCE_INLINE void op_f64add()
{
    double v2 = i2f(stack_pop_uint64(&titan.stack));
    double v1 = i2f(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, f2i(v1 + v2));
}

static ATTR_FORCE_INLINE void op_f64sub()
{
    double v2 = i2f(stack_pop_uint64(&titan.stack));
    double v1 = i2f(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, f2i(v1 - v2));
}

static ATTR_FORCE_INLINE void op_f64mul()
{
    double v2 = i2f(stack_pop_uint64(&titan.stack));
    double v1 = i2f(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, f2i(v1 * v2));
}

static ATTR_FORCE_INLINE void op_f64div()
{
    double v2 = i2f(stack_pop_uint64(&titan.stack));
    double v1 = i2f(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, f2i(v1 / v2));
}


//

static ATTR_FORCE_INLINE void op_i32neg()
{
    int32_t value = u2s(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, s2u(-value));
}

static ATTR_FORCE_INLINE void op_i64neg()
{
    int64_t value = u2s(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, s2u(-value));
}

static ATTR_FORCE_INLINE void op_f32neg()
{
    float value = i2f(stack_pop_uint32(&titan.stack));
    stack_push_uint32(&titan.stack, f2i(-value));
}

static ATTR_FORCE_INLINE void op_f64neg()
{
    double value = i2f(stack_pop_uint64(&titan.stack));
    stack_push_uint64(&titan.stack, f2i(-value));
}

//

static ATTR_FORCE_INLINE void op_i32cmp()
{
    int32_t v2 = u2s(stack_pop_uint32(&titan.stack));
    int32_t v1 = u2s(stack_pop_uint32(&titan.stack));

    titan.cmp = 0;
    SET_LT_BIT(v1 < v2);
    SET_EQ_BIT(v1 == v2);
    SET_GT_BIT(v1 > v2);
}

static ATTR_FORCE_INLINE void op_i64cmp()
{
    int64_t v2 = u2s(stack_pop_uint64(&titan.stack));
    int64_t v1 = u2s(stack_pop_uint64(&titan.stack));

    titan.cmp = 0;
    SET_LT_BIT(v1 < v2);
    SET_EQ_BIT(v1 == v2);
    SET_GT_BIT(v1 > v2);
}

static ATTR_FORCE_INLINE void op_u32cmp()
{
    uint32_t v2 = stack_pop_uint32(&titan.stack);
    uint32_t v1 = stack_pop_uint32(&titan.stack);

    titan.cmp = 0;
    SET_LT_BIT(v1 < v2);
    SET_EQ_BIT(v1 == v2);
    SET_GT_BIT(v1 > v2);
}

static ATTR_FORCE_INLINE void op_u64cmp()
{
    uint64_t v2 = stack_pop_uint64(&titan.stack);
    uint64_t v1 = stack_pop_uint64(&titan.stack);

    titan.cmp = 0;
    SET_LT_BIT(v1 < v2);
    SET_EQ_BIT(v1 == v2);
    SET_GT_BIT(v1 > v2);
}

static ATTR_FORCE_INLINE void op_f32cmp()
{
    float v2 = i2f(stack_pop_uint32(&titan.stack));
    float v1 = i2f(stack_pop_uint32(&titan.stack));

    titan.cmp = 0;
    SET_LT_BIT(v1 < v2);
    SET_EQ_BIT(v1 == v2); // TODO: Do we want to allow EQ float comparison?
    SET_GT_BIT(v1 > v2);
}

static ATTR_FORCE_INLINE void op_f64cmp()
{
    double v2 = i2f(stack_pop_uint64(&titan.stack));
    double v1 = i2f(stack_pop_uint64(&titan.stack));

    titan.cmp = 0;
    SET_LT_BIT(v1 < v2);
    SET_EQ_BIT(v1 == v2); // TODO: Do we want to allow EQ float comparison?
    SET_GT_BIT(v1 > v2);
}

static ATTR_FORCE_INLINE void op_i8cmpz()
{
    uint8_t v = stack_pop_uint8(&titan.stack);

    titan.cmp = 0;
    SET_NZ_BIT(v != 0);
}

//

static ATTR_FORCE_INLINE void util_jmp_or_stay(uint8_t cmp_bit)
{
    chunk_size_t new_ip = chunk_next_inst_chunk_idx(&titan.ip);
    uintptr_t new_ip_ptr = (uintptr_t)(titan.chunk->instructions + new_ip);
    titan.ip = (uint8_t *)(cmp_bit * new_ip_ptr + !cmp_bit * (uintptr_t)titan.ip);
}

static ATTR_FORCE_INLINE void op_jmp()
{
    chunk_size_t new_ip = chunk_next_inst_chunk_idx(&titan.ip);
    titan.ip = titan.chunk->instructions + new_ip;
}

static ATTR_FORCE_INLINE void op_jle()
{
    util_jmp_or_stay(GET_BIT(LT_BIT) | GET_BIT(EQ_BIT));
}

static ATTR_FORCE_INLINE void op_jlt()
{
    util_jmp_or_stay(GET_BIT(LT_BIT));
}

static ATTR_FORCE_INLINE void op_jeq()
{
    util_jmp_or_stay(GET_BIT(EQ_BIT));
}

static ATTR_FORCE_INLINE void op_jgt()
{
    util_jmp_or_stay(GET_BIT(GT_BIT));
}

static ATTR_FORCE_INLINE void op_jge()
{
    util_jmp_or_stay(GET_BIT(GT_BIT) | GET_BIT(EQ_BIT));
}

static ATTR_FORCE_INLINE void op_jnz()
{
    util_jmp_or_stay(GET_BIT(NZ_BIT));
}

static ATTR_FORCE_INLINE void op_return()
{
    fprintf(stdout, "%d\n", u2s(stack_pop_uint32(&titan.stack)));
}

ProcessResult titan_interpret(const Chunk *chunk)
{
    titan.chunk = chunk;
    titan.ip = &chunk->instructions[0];

    for (;;)
    {
#ifdef DBG_TRACE_EXECUTION
        opcode_pretty_print(stdout, chunk, (chunk_size_t)(titan.ip - chunk->instructions));
#endif

#ifdef DBG_STEP_EXECUTION
        char c;
        fscanf(stdin, "%c", &c);
#endif

        switch (chunk_next_inst_opcode(&titan.ip))
        {
        case TI_I32TOI8:  op_i32toi8();  break;
        case TI_I32TOI16: op_i32toi16(); break;
        case TI_I64TOI32: op_i64toi32(); break;
        case TI_F64TOF32: op_f64tof32(); break;

        case  TI_PUSH8I: op_push8i();  break;
        case TI_PUSH16I: op_push16i(); break;

        case  TI_PUSH8P: op_push8p();  break;
        case TI_PUSH16P: op_push16p(); break;
        case TI_PUSH32P: op_push32p(); break;
        case TI_PUSH64P: op_push64p(); break;

        case TI_I32ADD: op_i32add(); break;
        case TI_I32SUB: op_i32sub(); break;
        case TI_I32MUL: op_i32mul(); break;
        case TI_I32DIV: op_i32div(); break;
        case TI_I32REM: op_i32rem(); break;

        case TI_U32ADD: op_u32add(); break;
        case TI_U32SUB: op_u32sub(); break;
        case TI_U32MUL: op_u32mul(); break;
        case TI_U32DIV: op_u32div(); break;
        case TI_U32REM: op_u32rem(); break;

        case TI_I64ADD: op_i64add(); break;
        case TI_I64SUB: op_i64sub(); break;
        case TI_I64MUL: op_i64mul(); break;
        case TI_I64DIV: op_i64div(); break;
        case TI_I64REM: op_i64rem(); break;

        case TI_U64ADD: op_u64add(); break;
        case TI_U64SUB: op_u64sub(); break;
        case TI_U64MUL: op_u64mul(); break;
        case TI_U64DIV: op_u64div(); break;
        case TI_U64REM: op_u64rem(); break;

        case TI_F32ADD: op_f32add(); break;
        case TI_F32SUB: op_f32sub(); break;
        case TI_F32MUL: op_f32mul(); break;
        case TI_F32DIV: op_f32div(); break;

        case TI_F64ADD: op_f64add(); break;
        case TI_F64SUB: op_f64sub(); break;
        case TI_F64MUL: op_f64mul(); break;
        case TI_F64DIV: op_f64div(); break;

        case TI_I32NEG: op_i32neg(); break;
        case TI_I64NEG: op_i64neg(); break;
        case TI_F32NEG: op_f32neg(); break;
        case TI_F64NEG: op_f64neg(); break;

        case TI_I32CMP: op_i32cmp(); break;
        case TI_I64CMP: op_i64cmp(); break;
        case TI_U32CMP: op_u32cmp(); break;
        case TI_U64CMP: op_u64cmp(); break;
        case TI_F32CMP: op_f32cmp(); break;
        case TI_F64CMP: op_f64cmp(); break;
        case TI_I8CMPZ: op_i8cmpz(); break;

        case TI_JMP: op_jmp(); break;
        case TI_JLE: op_jle(); break;
        case TI_JLT: op_jlt(); break;
        case TI_JEQ: op_jeq(); break;
        case TI_JGT: op_jgt(); break;
        case TI_JGE: op_jge(); break;
        case TI_JNZ: op_jnz(); break;

        case TI_RETURN: op_return(); break;

        default: exit(TI_ERR_UNKNOWN_OPCODE);
        }

#ifdef DBG_TRACE_EXECUTION
        stack_pretty_print(&titan.stack, stdout);
        titan_pretty_print_registers(stdout);
#endif

        if (titan.ip == chunk->instructions + chunk->size)
            break;
    }

    return RESULT_OK;
}
