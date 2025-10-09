#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "booking_system.h"

#define TEST_CSV "test_passengers.csv"


static Booking e2eBookings[1000];
static int e2eBookingCount = 0;


void assert_equal_int(int expected, int actual, const char *message)
{
    if (expected == actual)
        printf("[PASS] %s (Expected=%d, Got=%d)\n", message, expected, actual);
    else
        printf("[FAIL] %s (Expected=%d, Got=%d)\n", message, expected, actual);
    assert(expected == actual);
}

void assert_equal_string(const char *expected, const char *actual, const char *message)
{
    if (strcmp(expected, actual) == 0)
        printf("[PASS] %s (Matched: %s)\n", message, expected);
    else
        printf("[FAIL] %s (Expected: %s, Got: %s)\n", message, expected, actual);
    assert(strcmp(expected, actual) == 0);
}


int file_exists(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (f)
    {
        fclose(f);
        return 1;
    }
    return 0;
}


// END-TO-END TEST CASE 1: Complete Flight Booking Workflow

void test_complete_flight_booking_workflow()
{
    printf("=== END-TO-END TEST CASE 1: Complete Flight Booking Workflow ===\n");
    printf("Scenario: Add passengers -> Save mock CSV -> Reload -> Validate data integrity\n\n");

    
    printf("[ARRANGE] Prepare test environment\n");
    const char *test_filename = TEST_CSV;

    e2eBookingCount = 0;
    for (int i = 0; i < 1000; i++)
        memset(&e2eBookings[i], 0, sizeof(Booking));

    remove(test_filename);
    init_flights(); 

    struct
    {
        const char *flightCode;
        const char *from;
        const char *to;
        const char *date;
        const char *seatID;
        const char *name;
        const char *code;
    } test_bookings[] = {
        {"FL001", "Bangkok", "Paris", "2025-12-25", "A1", "Jirayu Wittayakarn", "MOCK01"},
        {"FL002", "Chiangmai", "Seoul", "2025-12-31", "B3", "Piyada Sooksai", "MOCK02"},
        {"FL003", "Phuket", "Tokyo", "2026-01-05", "C2", "Thanakorn Mangmee", "MOCK03"}};

    int expected_count = 3;
    printf("   - Test file: %s\n", test_filename);
    printf("   - Number of mock bookings: %d\n", expected_count);

    // ==================== ACT ====================
    printf("\n[ACT] Execute workflow\n");

    // Step 1: Add mock bookings
    for (int i = 0; i < expected_count; i++)
    {
        Booking b;
        strcpy(b.flightCode, test_bookings[i].flightCode);
        strcpy(b.from, test_bookings[i].from);
        strcpy(b.to, test_bookings[i].to);
        strcpy(b.date, test_bookings[i].date);
        strcpy(b.seatID, test_bookings[i].seatID);
        strcpy(b.name, test_bookings[i].name);
        strcpy(b.code, test_bookings[i].code);
        b.booked = 1;
        e2eBookings[e2eBookingCount++] = b;
        printf("   Added: %s (%s -> %s, Seat %s)\n", b.name, b.from, b.to, b.seatID);
    }

    // Step 2: Save mock CSV
    FILE *fp = fopen(test_filename, "w");
    assert(fp != NULL);
    fprintf(fp, "FlightCode,From,To,Date,SeatID,Name,Status,Code\n");
    for (int i = 0; i < e2eBookingCount; i++)
    {
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s\n",
                e2eBookings[i].flightCode,
                e2eBookings[i].from,
                e2eBookings[i].to,
                e2eBookings[i].date,
                e2eBookings[i].seatID,
                e2eBookings[i].name,
                e2eBookings[i].booked ? "Booked" : "Available",
                e2eBookings[i].code);
    }
    fclose(fp);
    printf("   Saved mock CSV successfully.\n");

    // Step 3: Clear memory
    e2eBookingCount = 0;
    for (int i = 0; i < 1000; i++)
        memset(&e2eBookings[i], 0, sizeof(Booking));
    printf("   Cleared in-memory data.\n");

    // Step 4: Reload from mock CSV
    fp = fopen(test_filename, "r");
    assert(fp != NULL);
    char line[256];
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp))
    {
        Booking b;
        char status[20];
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^,],%3[^,],%49[^,],%19[^,],%9[^,\n]",
               b.flightCode, b.from, b.to, b.date, b.seatID, b.name, status, b.code);
        b.booked = (strcmp(status, "Booked") == 0);
        e2eBookings[e2eBookingCount++] = b;
    }
    fclose(fp);
    printf("   Reloaded %d bookings from CSV.\n", e2eBookingCount);

    // แสดงข้อมูลที่โหลดมา
    printf("\n[DEBUG] Contents of test_passengers.csv after reload:\n");
    printf("------------------------------------------------------------\n");
    printf("%-8s %-12s %-12s %-12s %-6s %-20s %-8s\n",
           "Flight", "From", "To", "Date", "Seat", "Name", "Code");
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < e2eBookingCount; i++)
    {
        printf("%-8s %-12s %-12s %-12s %-6s %-20s %-8s\n",
               e2eBookings[i].flightCode,
               e2eBookings[i].from,
               e2eBookings[i].to,
               e2eBookings[i].date,
               e2eBookings[i].seatID,
               e2eBookings[i].name,
               e2eBookings[i].code);
    }
    printf("------------------------------------------------------------\n");

    // ==================== ASSERT ====================
    printf("\n[ASSERT] Validate results\n");

    assert_equal_int(1, file_exists(test_filename), "Mock CSV file was created");
    assert_equal_int(expected_count, e2eBookingCount, "Booking count after reload");

    for (int i = 0; i < expected_count; i++)
    {
        char msg[100];
        sprintf(msg, "Booking %d Flight Code", i + 1);
        assert_equal_string(test_bookings[i].flightCode, e2eBookings[i].flightCode, msg);

        sprintf(msg, "Booking %d Passenger Name", i + 1);
        assert_equal_string(test_bookings[i].name, e2eBookings[i].name, msg);

        sprintf(msg, "Booking %d Destination", i + 1);
        assert_equal_string(test_bookings[i].to, e2eBookings[i].to, msg);

        sprintf(msg, "Booking %d Date", i + 1);
        assert_equal_string(test_bookings[i].date, e2eBookings[i].date, msg);

        sprintf(msg, "Booking %d Seat", i + 1);
        assert_equal_string(test_bookings[i].seatID, e2eBookings[i].seatID, msg);
    }

    int line_count = 0;
    fp = fopen(test_filename, "r");
    while (fgets(line, sizeof(line), fp))
        line_count++;
    fclose(fp);
    line_count -= 1;
    assert_equal_int(expected_count, line_count, "CSV line count matches records");

    printf("\n=== Test Case 1 Completed Successfully ===\n");
    printf("   (Kept file for inspection: %s)\n\n", test_filename);
}

// END-TO-END TEST CASE 2: Data Persistence and Search Functionality

void test_data_persistence_and_search()
{
    printf("=== END-TO-END TEST CASE 2: Data Persistence and Search Functionality ===\n");
    printf("Scenario: Create mock CSV → Load → Perform search → Validate results\n\n");

    // ==================== ARRANGE ====================
    printf("[ARRANGE] Setup mock data file\n");
    const char *test_filename = TEST_CSV;
    FILE *fp = fopen(test_filename, "w");
    assert(fp != NULL);
    fprintf(fp, "FlightCode,From,To,Date,SeatID,Name,Status,Code\n");
    fprintf(fp, "FL001,Bangkok,Paris,2025-12-25,A1,Jirayu Wittayakarn,Booked,M001\n");
    fprintf(fp, "FL002,Chiangmai,Seoul,2025-12-31,B3,Piyada Sooksai,Booked,M002\n");
    fprintf(fp, "FL003,Phuket,Tokyo,2026-01-05,C2,Thanakorn Mangmee,Booked,M003\n");
    fclose(fp);
    printf("   - Mock CSV created.\n");

    // ==================== ACT ====================
    printf("\n[ACT] Load data and simulate search\n");
    e2eBookingCount = 0;
    fp = fopen(test_filename, "r");
    assert(fp != NULL);
    char line[256];
    fgets(line, sizeof(line), fp);
    while (fgets(line, sizeof(line), fp))
    {
        Booking b;
        char status[20];
        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^,],%3[^,],%49[^,],%19[^,],%9[^,\n]",
               b.flightCode, b.from, b.to, b.date, b.seatID, b.name, status, b.code);
        b.booked = (strcmp(status, "Booked") == 0);
        e2eBookings[e2eBookingCount++] = b;
    }
    fclose(fp);

    // แสดงข้อมูลโหลดมาทั้งหมด
    printf("\n[DEBUG] Loaded bookings from %s:\n", test_filename);
    printf("------------------------------------------------------------\n");
    for (int i = 0; i < e2eBookingCount; i++)
    {
        printf("%s | %s -> %s | %s | %s | %s\n",
               e2eBookings[i].flightCode,
               e2eBookings[i].from,
               e2eBookings[i].to,
               e2eBookings[i].date,
               e2eBookings[i].seatID,
               e2eBookings[i].name);
    }
    printf("------------------------------------------------------------\n");

    int found_paris = 0;
    int found_tokyo = 0;
    for (int i = 0; i < e2eBookingCount; i++)
    {
        if (strstr(e2eBookings[i].to, "Paris"))
            found_paris = 1;
        if (strstr(e2eBookings[i].to, "Tokyo"))
            found_tokyo = 1;
    }

    // ==================== ASSERT ====================
    printf("\n[ASSERT] Validate results\n");
    assert_equal_int(1, file_exists(test_filename), "Mock CSV file exists");
    assert_equal_int(3, e2eBookingCount, "All mock records loaded successfully");
    assert_equal_int(1, found_paris, "Search found passenger to Paris");
    assert_equal_int(1, found_tokyo, "Search found passenger to Tokyo");

    printf("\n=== Test Case 2 Completed Successfully ===\n");
    printf("   (Kept file for inspection: %s)\n\n", test_filename);
}


void Run_E2E_Tests()
{
    printf("========== RUNNING END-TO-END TESTS ==========\n\n");

    test_complete_flight_booking_workflow();
    test_data_persistence_and_search();

    printf("========== ALL END-TO-END TESTS COMPLETED ==========\n");
    printf("Press ENTER to return...");
    while (getchar() != '\n')
        ;
}
