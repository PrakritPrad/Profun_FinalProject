#include <stdio.h>
#include <assert.h>
#include "Unit_E2E.h"
#include <string.h>
#include <windows.h>

int test_is_number()
{
    assert(is_number("123") == 1);
    assert(is_number("abc") == 0);
    assert(is_number("12a3") == 0);
    assert(is_number("0") == 1);
    return 1;
}
int test_is_name()
{
    assert(is_name("John Doe") == 1);
    assert(is_name("A") == 0);
    assert(is_name("John123") == 0);
    assert(is_name("") == 0);
    assert(is_name("Mary Jane") == 1);
    return 1;
}
int test_is_valid_date()
{
    assert(is_valid_date("2025-05-20") == 1);
    assert(is_valid_date("2023-12-31") == 0);
    assert(is_valid_date("2024-13-01") == 0);
    assert(is_valid_date("2024-00-10") == 0);
    assert(is_valid_date("2024-02-30") == 0);
    (is_valid_date("invalid-date") == 0);
    return 1;
}
int test_generate_unique_code()
{
    char code1[10], code2[10];
    generate_unique_code(code1);
    generate_unique_code(code2);
    assert(strcmp(code1, code2) != 0);
    return 1;
}

void unit_test()
{
    int total = 0, passed = 0;

    printf("==================== RUNNING UNIT TESTS ====================\n");

    printf("Running test func : is_name...\n");
    total++;
    if (test_is_name())
    {
        printf("[PASS] FUNC is_name()\n");
        passed++;
        Sleep(500);
    }
    else
    {
        printf("[FAIL] FUNC is_name()\n");
    }

    printf("Running test func : is_number...\n");
    Sleep(500);
    total++;
    if (test_is_number())
    {
        printf("[PASS] Func is_number()\n");
        Sleep(500);
        passed++;
    }
    else
    {
        printf("[FAIL] Func is_number()\n");
    }

    printf("Running test func : is_valid_date...\n");
    total++;
    Sleep(500);
    if (test_is_valid_date())
    {
        printf("[PASS] Func is_valid_date()\n");
        passed++;
        Sleep(500);
    }
    else
    {
        printf("[FAIL] Func is_valid_date()\n");
    }

    printf("Running test func : generate_unique_code...\n");
    total++;
    Sleep(500);
    if (test_generate_unique_code())
    {
        printf("[PASS] Func generate_unique_code()\n");
        passed++;
        Sleep(500);
    }
    else
    {
        printf("[FAIL] Func generate_unique_code()\n");
    }

    printf("============================================================\n");
    printf("SUMMARY: %d/%d TESTS PASSED\n", passed, total);
    if (passed == total)
    {

        printf(" ALL TESTS PASSED! \n");
        Sleep(500);
    }
    else
        printf("Some tests FAILED!\n");
    printf("============================================================\n");
}

void e2e_test()
{

    printf("Running e2e Tests...\n");
}
