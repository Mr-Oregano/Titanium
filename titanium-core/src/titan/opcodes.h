#ifndef TITAN_OPCODES
#define TITAN_OPCODES

#include <common.h>
#include <titan/debug/error_codes.h>

#define LT_BIT 0
#define EQ_BIT 1
#define GT_BIT 2
#define NZ_BIT 3

typedef enum ATTR_PACKED
{
    TI_I32TOI8,  // TI_I32TOI8;                         "Pop 32-bit int value from the stack and push truncated 8-bit value"
    TI_I32TOI16, // TI_I32TOI16;                        "Pop 32-bit int value from the stack and push truncated 16-bit value"
    TI_I64TOI32, // TI_I64TOI32;                        "Pop 64-bit int value from the stack and push truncated 32-bit value"
    TI_F64TOF32, // TI_F64TOF32;                        "Pop 64-bit float value from the stack and push corresponding 32-bit float value"

    TI_PUSH8I,   // TI_PUSH8I  <value:1b>;              "Push immediate  8-bit value onto the stack"
    TI_PUSH16I,  // TI_PUSH16I <value:2b>;              "Push immediate 16-bit value onto the stack"

    TI_PUSH8P,   // TI_PUSH8P  <index:1b>;              "Load  8 bits from constant pool and push onto the stack"
    TI_PUSH16P,  // TI_PUSH16P <index:1b>;              "Load 16 bits from constant pool and push onto the stack"
    TI_PUSH32P,  // TI_PUSH32P <index:1b>;              "Load 32 bits from constant pool and push onto the stack"
    TI_PUSH64P,  // TI_PUSH64P <index:1b>;              "Load 64 bits from constant pool and push onto the stack"

    TI_I32ADD,   // TI_I32ADD;                          "Pop two 32-bit signed int values from stack, add, and push result back"
    TI_I32SUB,   // TI_I32SUB;                          "Pop two 32-bit signed int values from stack, subtract, and push result back"
    TI_I32MUL,   // TI_I32MUL;                          "Pop two 32-bit signed int values from stack, multiply, and push result back"
    TI_I32DIV,   // TI_I32DIV;                          "Pop two 32-bit signed int values from stack, divide, and push result back"
    TI_I32REM,   // TI_I32REM;                          "Pop two 32-bit signed int values from stack, divide, and push remainder back"

    TI_U32ADD,   // TI_U32ADD;                          "Pop two 32-bit unsigned int values from stack, add, and push result back"
    TI_U32SUB,   // TI_U32SUB;                          "Pop two 32-bit unsigned int values from stack, subtract, and push result back"
    TI_U32MUL,   // TI_U32MUL;                          "Pop two 32-bit unsigned int values from stack, multiply, and push result back"
    TI_U32DIV,   // TI_U32DIV;                          "Pop two 32-bit unsigned int values from stack, divide, and push result back"
    TI_U32REM,   // TI_U32REM;                          "Pop two 32-bit unsigned int values from stack, divide, and push remainder back"

    TI_I64ADD,   // TI_I64ADD;                          "Pop two 64-bit signed int values from stack, add, and push result back"
    TI_I64SUB,   // TI_I64SUB;                          "Pop two 64-bit signed int values from stack, subtract, and push result back"
    TI_I64MUL,   // TI_I64MUL;                          "Pop two 64-bit signed int values from stack, multiply, and push result back"
    TI_I64DIV,   // TI_I64DIV;                          "Pop two 64-bit signed int values from stack, divide, and push result back"
    TI_I64REM,   // TI_I64REM;                          "Pop two 64-bit signed int values from stack, divide, and push remainder back"

    TI_U64ADD,   // TI_U64ADD;                          "Pop two 64-bit unsigned int values from stack, add, and push result back"
    TI_U64SUB,   // TI_U64SUB;                          "Pop two 64-bit unsigned int values from stack, subtract, and push result back"
    TI_U64MUL,   // TI_U64MUL;                          "Pop two 64-bit unsigned int values from stack, multiply, and push result back"
    TI_U64DIV,   // TI_U64DIV;                          "Pop two 64-bit unsigned int values from stack, divide, and push result back"
    TI_U64REM,   // TI_U64REM;                          "Pop two 64-bit unsigned int values from stack, divide, and push remainder back"

    TI_F32ADD,   // TI_F32ADD;                          "Pop two 32-bit float values from stack, add, and push result back"
    TI_F32SUB,   // TI_F32SUB;                          "Pop two 32-bit float values from stack, subtract, and push result back"
    TI_F32MUL,   // TI_F32MUL;                          "Pop two 32-bit float values from stack, multiply, and push result back"
    TI_F32DIV,   // TI_F32DIV;                          "Pop two 32-bit float values from stack, divide, and push result back"

    TI_F64ADD,   // TI_F64ADD;                          "Pop two 64-bit float values from stack, add, and push result back"
    TI_F64SUB,   // TI_F64SUB;                          "Pop two 64-bit float values from stack, subtract, and push result back"
    TI_F64MUL,   // TI_F64MUL;                          "Pop two 64-bit float values from stack, multiply, and push result back"
    TI_F64DIV,   // TI_F64DIV;                          "Pop two 64-bit float values from stack, divide, and push result back"

    TI_I32NEG,   // TI_I32NEG;                          "Pop 32-bit int value from stack, negate, and push value back"
    TI_I64NEG,   // TI_I64NEG;                          "Pop 64-bit int value from stack, negate, and push value back"
    TI_F32NEG,   // TI_F32NEG;                          "Pop 32-bit float value from stack, negate, and push value back"
    TI_F64NEG,   // TI_F64NEG;                          "Pop 64-bit float value from stack, negate, and push value back"

    TI_I32CMP,   // TI_I32CMP;                          "Pop two 32-bit int values from stack and compare them"
    TI_I64CMP,   // TI_I64CMP;                          "Pop two 64-bit int values from stack and compare them"
    TI_U32CMP,   // TI_U32CMP;                          "Pop two 32-bit unsigned int values from stack and compare them"
    TI_U64CMP,   // TI_U64CMP;                          "Pop two 64-bit unsigned int values from stack and compare them"
    TI_F32CMP,   // TI_F32CMP;                          "Pop two 32-bit float values from stack and compare them"
    TI_F64CMP,   // TI_F64CMP;                          "Pop two 64-bit float values from stack and compare them"
    TI_I8CMPZ,   // TI_I8CMPZ;                          "Pop one  8-bit unsigned int value from stack and compare with zero"

    TI_JMP,      // TI_JMP <value:2b>;                  "Indirect branch to instruction pointed by 'value' (relative to start of chunk)"
    TI_JLE,      // TI_JLE <value:2b>;                  "If LE flag is set, branch to instruction pointed by 'value' (relative to start of chunk)"
    TI_JLT,      // TI_JLT <value:2b>;                  "If LT flag is set, branch to instruction pointed by 'value' (relative to start of chunk)"
    TI_JEQ,      // TI_JEQ <value:2b>;                  "If EQ flag is set, branch to instruction pointed by 'value' (relative to start of chunk)"
    TI_JGT,      // TI_JGT <value:2b>;                  "If GT flag is set, branch to instruction pointed by 'value' (relative to start of chunk)"
    TI_JGE,      // TI_JGE <value:2b>;                  "If GE flag is set, branch to instruction pointed by 'value' (relative to start of chunk)"
    TI_JNZ,      // TI_JGE <value:2b>;                  "If NZ flag is set, branch to instruction pointed by 'value' (relative to start of chunk)"

    TI_RETURN    // TI_RETURN;                          "Jump to return address"
}
TiOpCode;

#endif
