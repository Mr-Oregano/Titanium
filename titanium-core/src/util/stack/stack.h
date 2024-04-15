#ifndef TITANIUM_STACK
#define TITANIUM_STACK

#include <common.h>
#include <stdint.h>
#include <util/util.h>

#define STACK_MAX 1024

typedef struct
{
    uint8_t data[STACK_MAX];
    uint8_t *sp;
}
Stack;

void stack_init(Stack *stack);
void stack_free(Stack *stack);
void stack_reset(Stack *stack);

void stack_push(Stack *stack, uint8_t value);
uint8_t stack_pop(Stack *stack);

void stack_pretty_print(const Stack *const stack, FILE *out);

static ATTR_FORCE_INLINE void stack_push_t(Stack *stack, uint64_t value, size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        uint8_t v = (value >> BYTES_TO_BITS(i)) & UMAX_OF(uint8_t);
        stack_push(stack, v);
    }
}

static ATTR_FORCE_INLINE uint64_t stack_pop_t(Stack *stack, size_t size)
{
    uint64_t result = 0;

    for (size_t i = 0; i < size; ++i)
    {
        uint64_t v = stack_pop(stack);
        result |= v << BYTES_TO_BITS(size - 1 - i);
    }

    return result;
}

#define STACK_PUSH_FUNC_T(NAME, TYPE) \
    static ATTR_FORCE_INLINE void NAME(Stack *stack, TYPE value) { stack_push_t(stack, (uint64_t) value, sizeof(TYPE)); }

#define STACK_POP_FUNC_T(NAME, TYPE) \
    static ATTR_FORCE_INLINE TYPE NAME(Stack *stack) { return stack_pop_t(stack, sizeof(TYPE)) & UMAX_OF(TYPE); }

STACK_PUSH_FUNC_T(stack_push_uint8, uint8_t)
STACK_PUSH_FUNC_T(stack_push_uint16, uint16_t)
STACK_PUSH_FUNC_T(stack_push_uint32, uint32_t)
STACK_PUSH_FUNC_T(stack_push_uint64, uint64_t)

STACK_POP_FUNC_T(stack_pop_uint8, uint8_t)
STACK_POP_FUNC_T(stack_pop_uint16, uint16_t)
STACK_POP_FUNC_T(stack_pop_uint32, uint32_t)
STACK_POP_FUNC_T(stack_pop_uint64, uint64_t)

#undef STACK_PUSH_FUNC_T
#undef STACK_POP_FUNC_T

#endif
