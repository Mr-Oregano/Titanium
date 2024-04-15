
#include "stack.h"

#include <titan/debug/error_codes.h>
#include <util/util.h>

void stack_init(Stack *stack)
{
    stack_reset(stack);
}

void stack_free(Stack *stack)
{
    stack->sp = NULL;
}

void stack_reset(Stack *stack)
{
    stack->sp = stack->data;
}

void stack_push(Stack *stack, uint8_t value)
{
    ASSERT(stack->sp != NULL, "Stack pointer was not initialized!");

    if (stack->sp == &stack->data[STACK_MAX] + 1)
        exit(TI_ERR_STACK_OVERFLOW);

    *stack->sp = value;
    ++stack->sp;
}

uint8_t stack_pop(Stack *stack)
{
    ASSERT(stack->sp != NULL, "Stack pointer was not initialized!");

    if (stack->sp == stack->data)
        exit(TI_ERR_STACK_UNDERFLOW);

    --stack->sp;
    return *stack->sp;
}

void stack_pretty_print(const Stack *const stack, FILE *out)
{
    ASSERT(stack->sp != NULL, "Stack pointer was not initialized!");

    fprintf(out, "\t\t\t---> STACK: ");
    for (const uint8_t *item = stack->data; item < stack->sp; ++item)
        fprintf(out, "[ 0x%02X ]", *item);
    fprintf(stdout, "\n");
}
