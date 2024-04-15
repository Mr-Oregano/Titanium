#ifndef TITAN_ERROR_CODES
#define TITAN_ERROR_CODES

typedef enum
{
    TI_ERR_OUT_OF_MEMORY = 69,
    TI_ERR_UNKNOWN_OPCODE = 68,
    TI_ERR_DATAPOOL_MAX_CAPACITY = 67,
    TI_ERR_CHUNK_MAX_CAPACITY = 66,
    TI_ERR_STACK_OVERFLOW = 65,
    TI_ERR_STACK_UNDERFLOW = 64
}
ErrorCode;

#endif