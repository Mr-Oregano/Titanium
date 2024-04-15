#ifndef TITAN
#define TITAN

#include <titan/chunk/chunk.h>

typedef enum
{
    RESULT_OK,
    RESULT_FAIL
}
ProcessResult;

void titan_init();
void titan_free();

ProcessResult titan_interpret(const Chunk *chunk);

#endif
