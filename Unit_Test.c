#include <stdio.h>
#include <assert.h>
#include "booking_system.h"

void test_is_leap_year() {
    printf("========= Testing FUNC is_leap_year() ============\n");

    printf("Test year: 2024 ");  assert(is_leap_year(2024) == 1);  printf("\nResult : PASS\n");
    printf("Test year: 2025 ");  assert(is_leap_year(2025) == 0);  printf("\nResult : PASS\n");
    printf("Test year: 1900 ");  assert(is_leap_year(1900) == 0);  printf("\nResult : PASS\n");
    printf("Test year: 2000 ");  assert(is_leap_year(2000) == 1);  printf("\nResult : PASS\n");
    printf("Test year: 1 ");     assert(is_leap_year(1) == 0);     printf("\nResult : PASS\n");
    printf("Test year: 4 ");     assert(is_leap_year(4) == 1);     printf("\nResult : PASS\n");
    printf("Test year: 2100 ");  assert(is_leap_year(2100) == 0);  printf("\nResult : PASS\n");
    printf("Test year: 2400 ");  assert(is_leap_year(2400) == 1);  printf("\nResult : PASS\n");

    printf("All tests for is_leap_year passed.\n\n");
}

void test_is_valid_future_date() {
    printf("========= Testing: is_valid_future_date() =========\n");

    printf("Test 2025/12/25 Expect: ");  assert(is_valid_future_date("2025/12/25") == 0);  printf("Result : PASS\n");
    printf("Test 25-12-2025 Expect: ");  assert(is_valid_future_date("25-12-2025") == 0);  printf("Result : PASS\n");
    printf("Test 2025-13-10 Expect: ");  assert(is_valid_future_date("2025-13-10") == 0);  printf("Result : PASS\n");
    printf("Test 2025-04-31 Expect: ");  assert(is_valid_future_date("2025-04-31") == 0);  printf("Result : PASS\n");
    printf("Test 2025-02-29 Expect: ");  assert(is_valid_future_date("2025-02-29") == 0);  printf("Result : PASS\n");
    printf("Test 2025-01-00 Expect: ");  assert(is_valid_future_date("2025-01-00") == 0);  printf("Result : PASS\n");
    printf("Test 2025-01-32 Expect: ");  assert(is_valid_future_date("2025-01-32") == 0);  printf("Result : PASS\n");
    printf("Test 2025-10-17... ");  assert(is_valid_future_date("2025-10-17") == 1);  printf("\nResult : PASS\n");
    printf("Test 2025-12-25... ");  assert(is_valid_future_date("2025-12-25") == 1);  printf("\nResult : PASS\n");
    printf("Test 2999-12-31 Expect: ");  assert(is_valid_future_date("2999-12-31") == 0);  printf("Result : PASS\n");
    printf("Test 0000-01-01 Expect: ");  assert(is_valid_future_date("0000-01-01") == 0);  printf("Result : PASS\n");

    printf("All tests for is_valid_future_date passed.\n\n");
}
//
void Run_units() {
    printf("Running unit tests...\n\n");

    test_is_leap_year();
    test_is_valid_future_date();

    printf("All unit tests finished successfully.\n");
    printf("Press ENTER to return...");
    while (getchar() != '\n')
        ;
}
