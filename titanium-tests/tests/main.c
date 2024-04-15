
#include <unity.h>

#include "chunk_test.h"
#include "datapool_test.h"
#include "stack_test.h"

#define TITAN_TEST_SUITE(...) TITAN_TEST_SUITE_WITH_NAME(__VA_ARGS__, throwaway)
#define TITAN_TEST_SUITE_WITH_NAME(func, ...) RunTestSuite(func, #func)

void setUp()
{
    // Not implemented
}

void tearDown()
{
    // Not implemented
}

void RunTestSuite(UnityTestFunction Func, const char *FuncName)
{
    printf("\n======================= START %s SUITE =======================\n", FuncName);
    Func();
}

int main()
{
    UNITY_BEGIN();
    TITAN_TEST_SUITE(chunk_tests_main);
    TITAN_TEST_SUITE(datapool_tests_main);
    TITAN_TEST_SUITE(stack_tests_main);
    return UNITY_END();
}
