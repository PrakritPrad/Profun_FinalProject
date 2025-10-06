// Unit_Test.c
#include <stdio.h>
#include <assert.h>
#include "booking_system.h"

// ===============================
// Test Function: is_leap_year()
// ===============================
void test_is_leap_year()
{
    printf("=== Testing: is_leap_year() ===\n");

    int result;

    // General cases
    result = is_leap_year(2024);
    printf("Year 2024 (divisible by 4, not by 100) -> Expected Leap Year: %s\n",
           result ? "PASS" : "FAIL");

    result = is_leap_year(2025);
    printf("Year 2025 (not divisible by 4) -> Expected Not Leap Year: %s\n",
           !result ? "PASS" : "FAIL");

    // Century year cases
    result = is_leap_year(1900);
    printf("Year 1900 (divisible by 100 but not by 400) -> Expected Not Leap Year: %s\n",
           !result ? "PASS" : "FAIL");

    result = is_leap_year(2000);
    printf("Year 2000 (divisible by 400) -> Expected Leap Year: %s\n",
           result ? "PASS" : "FAIL");

    // Lower boundary
    result = is_leap_year(1);
    printf("Year 1 (very early boundary) -> Expected Not Leap Year: %s\n",
           !result ? "PASS" : "FAIL");

    result = is_leap_year(4);
    printf("Year 4 (first divisible by 4) -> Expected Leap Year: %s\n",
           result ? "PASS" : "FAIL");

    // Future boundary
    result = is_leap_year(2100);
    printf("Year 2100 (divisible by 100 but not 400) -> Expected Not Leap Year: %s\n",
           !result ? "PASS" : "FAIL");

    result = is_leap_year(2400);
    printf("Year 2400 (divisible by 400) -> Expected Leap Year: %s\n",
           result ? "PASS" : "FAIL");

    printf("=== Finished test_is_leap_year ===\n\n");
}

// ========================================
// Test Function: is_valid_future_date()
// ========================================
void test_is_valid_future_date()
{
    printf("=== Testing: is_valid_future_date() ===\n");

    int result;

    // Invalid format
    result = is_valid_future_date("2025/12/25");
    printf("Case '2025/12/25' -> Wrong format, Expected FAIL: %s\n", !result ? "PASS" : "FAIL");

    result = is_valid_future_date("25-12-2025");
    printf("Case '25-12-2025' -> Wrong format, Expected FAIL: %s\n", !result ? "PASS" : "FAIL");

    // Invalid month
    result = is_valid_future_date("2025-13-10");
    printf("Case month = 13 -> Expected FAIL: %s\n", !result ? "PASS" : "FAIL");

    // Invalid day for month
    result = is_valid_future_date("2025-04-31");
    printf("Case day = 31 in April -> Expected FAIL: %s\n", !result ? "PASS" : "FAIL");

    // February cases
    result = is_valid_future_date("2025-02-29");
    printf("Case 2025-02-29 (non-leap year) -> Expected FAIL: %s\n", !result ? "PASS" : "FAIL");

    // Out of range days
    result = is_valid_future_date("2025-01-00");
    printf("Case day = 0 -> Expected FAIL: %s\n", !result ? "PASS" : "FAIL");

    result = is_valid_future_date("2025-01-32");
    printf("Case day = 32 -> Expected FAIL: %s\n", !result ? "PASS" : "FAIL");

    // Current and near-future
    result = is_valid_future_date("2025-10-06");
    printf("Case current date -> Expected PASS: %s\n", result ? "PASS" : "FAIL");

    result = is_valid_future_date("2025-12-25");
    printf("Case future date -> Expected PASS: %s\n", result ? "PASS" : "FAIL");

    // Extreme cases
    result = is_valid_future_date("2999-12-31");
    printf("Case far future -> Expected PASS: %s\n", result ? "PASS" : "FAIL");

    result = is_valid_future_date("0000-01-01");
    printf("Case year = 0000 (nonsensical) -> Expected FAIL: %s\n", !result ? "PASS" : "FAIL");

    printf("=== Finished test_is_valid_future_date ===\n\n");
}

// ===============================
// Test Runner
// ===============================
void Run_units()
{
    printf("========== RUNNING UNIT TESTS ==========\n\n");

    test_is_leap_year();
    test_is_valid_future_date();

    printf("========== ALL TESTS FINISHED SUCCESSFULLY ==========\n");
    printf("Press ENTER to return...");
    while (getchar() != '\n')
        ;
}
