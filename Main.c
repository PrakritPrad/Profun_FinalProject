#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#define MAX_FLIGHTS 3
#define MAX_SEATS 15

typedef struct
{
    char seatID[4];
    char name[50];
    int booked;
    char code[10];
    char date[20];
} Seat;

typedef struct
{
    char code[10];
    char from[30];
    char to[30];
    char date[20]; // Done เก็บวันที่บิน
    Seat seats[MAX_SEATS];
} Flight;

Flight flights[MAX_FLIGHTS];
int flightCount = 3;
typedef struct
{
    char flightCode[10];
    char from[30];
    char to[30];
    char date[20];
    char seatID[4];
    char name[50];
    char code[10];
    int booked;
} Booking;

Booking allBookings[1000];
int bookingCount = 0;


void init_flights();
void main_menu();
void manage_flight(Flight *f);
void view_seats(Flight *f);
void add_passenger(Flight *f);
void cancel_passenger(Flight *f);
void change_seat(Flight *f);
void search_menu();
void save_all_to_csv();
void load_from_csv();
void update_passenger(Flight *f);
void generate_unique_code(char code[10]);
void Run_units();
void Run_E2E_Tests(void);

int is_seat_booked_on_date(Flight *f, const char *seatID, const char *date);
void trim(char *s);
int str_casecmp(const char *a, const char *b);
int is_leap_year(int year);
int is_valid_future_date(const char *dateStr);
void to_lowercase(char *str);
void welcome_screen();

void welcome_screen()
{
    system("cls");
    printf("=====================================\n");
    printf("🌟 Travel booking management system 🌟\n");
    printf("=====================================\n\n");
}

int main()
{
    init_flights();
    load_from_csv();
    main_menu();
    return 0;
}

// ========== INIT FLIGHTS ==========
void init_flights()
{
    srand(time(NULL));
    welcome_screen();
    strcpy(flights[0].code, "FL001");
    strcpy(flights[0].from, "Bangkok");
    strcpy(flights[0].to, "Paris");

    strcpy(flights[1].code, "FL002");
    strcpy(flights[1].from, "Chiangmai");
    strcpy(flights[1].to, "Seoul");

    strcpy(flights[2].code, "FL003");
    strcpy(flights[2].from, "Phuket");
    strcpy(flights[2].to, "Tokyo");

    // generate seats A1->C5
    char rows[] = {'A', 'B', 'C'};
    for (int f = 0; f < MAX_FLIGHTS; f++)
    {
        int n = 0;
        for (int r = 0; r < 3; r++)
        {
            for (int c = 1; c <= 5; c++)
            {
                sprintf(flights[f].seats[n].seatID, "%c%d", rows[r], c);
                memset(flights[f].seats[n].name, 0, sizeof(flights[f].seats[n].name));
                flights[f].seats[n].booked = 0;
                flights[f].seats[n].date[0] = '\0';
                n++;
            }
        }
    }
}

// ========== MAIN MENU ==========
void main_menu()
{
    char input[16];
    int choice;

    while (1)
    {
        printf("\n========== FLIGHT MENU ==========\n");
        for (int i = 0; i < flightCount; i++)
        {
            printf("%d) %s %s -> %s\n", i + 1, flights[i].code, flights[i].from, flights[i].to);
        }
        printf("4) Search\n");
        printf("5) Units Test\n");
        printf("6) E2E Test\n");
        printf("0) Exit\n");
        printf("=================================\n");
        printf("Select option: ");

        // อ่าน input ด้วย fgets
        if (!fgets(input, sizeof(input), stdin))
        {
            clearerr(stdin);
            continue;
        }

        // ตัด newline
        input[strcspn(input, "\n")] = '\0';

        // ถ้า user กด enter เฉย ๆ
        if (strlen(input) == 0)
        {
            printf("Please enter a number\n");
            continue;
        }

        // แปลงเป็นตัวเลข
        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number\n");
            continue;
        }

        // ตรวจช่วงค่าถูกไหม
        if (choice < 0 || choice > flightCount && choice != 4 && choice != 5 && choice != 6)
        {
            printf("Invalid choice!\n");
            continue;
        }

        if (choice == 0)
        {
            save_all_to_csv();

            printf("Goodbye!\n");
            break;
        }
        else if (choice == 4)
        {

            search_menu();

            continue;
        }
        else if (choice == 5)
        {

            Run_units();

            continue;
        }
        else if (choice == 6)
        {

            Run_E2E_Tests();

            continue;
        }

        manage_flight(&flights[choice - 1]);
    }
}

// ========== MANAGE FLIGHT ==========
void manage_flight(Flight *f)
{
    char input[16];
    int choice;

    while (1)
    {

        printf("\n--- Manage %s (%s -> %s) ---\n", f->code, f->from, f->to);

        printf("1) View Seats\n");
        printf("2) Add Passenger\n");
        printf("3) Cancel Passenger\n");
        printf("4) Update Passenger\n");
        printf("5) Back\n");
        printf("Enter choice: ");

        if (!fgets(input, sizeof(input), stdin))
        {
            clearerr(stdin);
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
        {
            printf("Please enter a number\n");
            continue;
        }

        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            view_seats(f);
            printf("\nPress ENTER to return...");
            getchar();
            break;
        case 2:
            add_passenger(f);
            int c;
            printf("\nPlease press ENTER to go back to menu...");
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            getchar();
            break;
        case 3:
            cancel_passenger(f);
            break;
        case 4:
            update_passenger(f);
            break;
        case 5:
            return;
        default:
            printf("Invalid choice! Please enter 1->5\n");
        }
    }
}

// ========== VIEW SEATS ==========
void view_seats(Flight *f)
{
    char day[20];
    while (1)
    {
        printf("\nEnter flight date to view (YYYY-MM-DD): ");
        scanf("%19s", day);
        if (is_valid_future_date(day))
            break;
    }
    while (getchar() != '\n')
        ; // เคลียร์ input buffer

    printf("\nPassenger list for %s (%s -> %s, %s)\n",
           f->code, f->from, f->to, day);
    printf("-------------------------------------------------------\n");
    printf("%-6s | %-20s | %-8s\n", "Seat", "Name", "Code");
    printf("-------------------------------------------------------\n");

    int bookedCount = 0;

    //   ดึงข้อมูลจาก allBookings (ฐานข้อมูลรวม)
    for (int i = 0; i < bookingCount; i++)
    {
        if (strcmp(allBookings[i].flightCode, f->code) == 0 &&
            strcmp(allBookings[i].date, day) == 0 &&
            allBookings[i].booked)
        {
            printf("%-6s | %-20s | %-8s\n",
                   allBookings[i].seatID,
                   allBookings[i].name,
                   allBookings[i].code);
            bookedCount++;
        }
    }

    printf("-------------------------------------------------------\n");
    printf("Total booked on %s: %d / %d seats\n", day, bookedCount, MAX_SEATS);

    //   สร้าง seat map โดยเช็กแต่ละที่นั่งในวันนั้นจริง
    printf("\nSeat map for %s (%s -> %s, %s)\n",
           f->code, f->from, f->to, day);
    printf("----------------------------------\n");

    char rows[] = {'A', 'B', 'C'};
    for (int r = 0; r < 3; r++)
    {
        for (int c = 1; c <= 5; c++)
        {
            char seatID[4];
            sprintf(seatID, "%c%d", rows[r], c);

            // เช็กว่าที่นั่งนี้ถูกจองในวันที่เลือกหรือไม่
            int booked = 0;
            for (int i = 0; i < bookingCount; i++)
            {
                if (strcmp(allBookings[i].flightCode, f->code) == 0 &&
                    strcmp(allBookings[i].seatID, seatID) == 0 &&
                    strcmp(allBookings[i].date, day) == 0 &&
                    allBookings[i].booked)
                {
                    booked = 1;
                    break;
                }
            }

            printf("%s[%s]\t", seatID, booked ? "X" : "O");
        }
        printf("\n");
    }

    printf("----------------------------------\n");
}

int is_seat_booked_on_date(Flight *f, const char *seatID, const char *date)
{
    for (int i = 0; i < bookingCount; i++)
    {
        if (strcmp(allBookings[i].flightCode, f->code) == 0 &&
            strcmp(allBookings[i].seatID, seatID) == 0 &&
            strcmp(allBookings[i].date, date) == 0 &&
            allBookings[i].booked)
        {
            return 1; // ที่นั่งนี้จองแล้วในวันนั้น
        }
    }
    return 0; // ยังไม่จอง
}

// ========== ADD PASSENGER ==========
void add_passenger(Flight *f)
{
    char seat[4];
    int seatFound = 0;

    // 🔹 ถามวันบินก่อน
    // 🔹 ถามวันบินก่อน (พร้อมตรวจ format + เงื่อนไข)
    char travelDate[20];
    while (1)
    {
        printf("Enter travel date (YYYY-MM-DD): ");
        scanf("%19s", travelDate);

        if (is_valid_future_date(travelDate))
            break;
    }

    while (1)
    {
        printf("Enter seat ID to book (A1->C5): ");
        scanf("%3s", seat);
        to_lowercase(seat);

        seatFound = 0;

        for (int i = 0; i < MAX_SEATS; i++)
        {
            char seatLower[4];
            strcpy(seatLower, f->seats[i].seatID);
            to_lowercase(seatLower);

            if (strcmp(seatLower, seat) == 0)
            {
                seatFound = 1;

                //   เช็คในฐานข้อมูลก่อนว่าที่นั่งวันนี้ถูกจองหรือยัง
                if (is_seat_booked_on_date(f, seat, travelDate))
                {
                    printf("\nSeat %s is already booked for %s! Please choose another\n",
                           f->seats[i].seatID, travelDate);
                    break; // กลับไปให้กรอกใหม่
                }

                // 🆕 บันทึกวันที่บิน
                strcpy(f->seats[i].date, travelDate);

                // 🧍‍♂️ กรอกชื่อผู้โดยสาร
                while (1)
                {
                    printf("Enter passenger name (First Last): ");
                    scanf(" %[^\n]", f->seats[i].name);

                    while (f->seats[i].name[0] == ' ')
                        memmove(f->seats[i].name, f->seats[i].name + 1, strlen(f->seats[i].name));

                    if (strlen(f->seats[i].name) < 3)
                    {
                        printf("Name too short! Please enter full name\n");
                        continue;
                    }

                    if (strchr(f->seats[i].name, ' ') == NULL)
                    {
                        printf("Please enter first and last name (ex, John Doe)\n");
                        continue;
                    }

                    break;
                }

                f->seats[i].booked = 1;
                generate_unique_code(f->seats[i].code);

                //   เตรียม Booking ใหม่
                Booking b;
                strcpy(b.flightCode, f->code);
                strcpy(b.from, f->from);
                strcpy(b.to, f->to);
                strcpy(b.date, travelDate);
                strcpy(b.seatID, f->seats[i].seatID);
                strcpy(b.name, f->seats[i].name);
                strcpy(b.code, f->seats[i].code);
                b.booked = 1;

                // 🔄 ถ้ามี booking เดิม (flight+seat+date) → เขียนทับ
                int replaced = 0;
                for (int j = 0; j < bookingCount; j++)
                {
                    if (strcmp(allBookings[j].flightCode, b.flightCode) == 0 &&
                        strcmp(allBookings[j].seatID, b.seatID) == 0 &&
                        strcmp(allBookings[j].date, b.date) == 0)
                    {
                        allBookings[j] = b;
                        replaced = 1;
                        break;
                    }
                }

                // ➕ ถ้าไม่มี ให้เพิ่มใหม่
                if (!replaced && bookingCount < 1000)
                    allBookings[bookingCount++] = b;

                // 💾 เขียนไฟล์ใหม่ทั้งไฟล์ (ไม่ append)
                save_all_to_csv();

                printf("\n-----------------------------\n");
                printf("Seat: %s booked successfully for %s!\n",
                       f->seats[i].seatID, f->seats[i].name);
                printf("Flight: %s (%s -> %s)\n", f->code, f->from, f->to);
                printf("Date: %s\n", f->seats[i].date);
                printf("Booking Code: %s\n", f->seats[i].code);
                printf("-----------------------------\n");
                printf("Booking saved to CSV!\n");

                return;
            }
        }

        if (!seatFound)
            printf("\nSeat not found! Please try again (A1->C5)\n");
    }
}

// ========== CANCEL PASSENGER ==========
void cancel_passenger(Flight *f)
{
    char seat[8];
    char travelDate[20];
    int found = 0;

    // 🔹 ถามวันที่และตรวจ format + เงื่อนไขอนาคต
    while (1)
    {
        printf("Enter travel date (YYYY-MM-DD): ");
        scanf("%19s", travelDate);
        if (is_valid_future_date(travelDate))
            break;
    }
    while (getchar() != '\n')
        ; // เคลียร์ buffer

    while (1)
    {
        printf("Enter seat ID to cancel (A1 -> C5, or 0 to cancel): ");

        if (!fgets(seat, sizeof(seat), stdin))
        {
            clearerr(stdin);
            continue;
        }

        seat[strcspn(seat, "\n")] = '\0'; // ตัด newline
        trim(seat);                       // กันช่องว่างหลุด
        to_lowercase(seat);

        if (strcmp(seat, "0") == 0)
        {
            printf("Cancellation process aborted.\n");
            return;
        }

        found = 0;

        //   หาในฐานข้อมูล allBookings แบบไม่สนตัวพิมพ์ใหญ่-เล็ก
        for (int i = 0; i < bookingCount; i++)
        {
            // ล้างช่องว่างและ \r ที่อาจมาจาก CSV
            trim(allBookings[i].flightCode);
            trim(allBookings[i].seatID);
            trim(allBookings[i].date);

            if (str_casecmp(allBookings[i].flightCode, f->code) == 0 &&
                str_casecmp(allBookings[i].seatID, seat) == 0 &&
                strcmp(allBookings[i].date, travelDate) == 0 &&
                allBookings[i].booked)
            {
                found = 1;

                printf("\nSeat %s (%s) is currently booked by: %s\n",
                       allBookings[i].seatID, travelDate, allBookings[i].name);

                char confirm[8];
                printf("Are you sure you want to cancel this booking? (y/n): ");

                if (!fgets(confirm, sizeof(confirm), stdin))
                {
                    clearerr(stdin);
                    continue;
                }

                if (tolower(confirm[0]) != 'y')
                {
                    printf("Cancellation aborted.\n");
                    return;
                }

                //  ยกเลิกจอง
                allBookings[i].booked = 0;
                strcpy(allBookings[i].name, "");
                strcpy(allBookings[i].code, "");

                printf("\nSeat %s on %s has been canceled and is now available.\n",
                       allBookings[i].seatID, travelDate);

                save_all_to_csv();
                return;
            }
        }

        if (!found)
        {
            printf("No booking found for Seat %s on %s.\n", seat, travelDate);
        }
    }
}

void update_passenger(Flight *f)
{
    char seatID[8], travelDate[20];
    int foundIndex = -1;

    // กรอกวันเดินทางก่อน เพื่อเจาะจง booking
    while (1)
    {
        printf("Enter travel date (YYYY-MM-DD): ");
        scanf("%19s", travelDate);
        if (is_valid_future_date(travelDate))
            break;
    }
    while (getchar() != '\n')
        ; // เคลียร์ buffer

    // กรอกหมายเลขที่นั่ง
    while (1)
    {
        printf("Enter seat ID to update (A1 -> C5): ");
        scanf("%7s", seatID);
        to_lowercase(seatID);
        while (getchar() != '\n')
            ;

        foundIndex = -1;
        for (int i = 0; i < bookingCount; i++)
        {
            if (str_casecmp(allBookings[i].flightCode, f->code) == 0 &&
                str_casecmp(allBookings[i].seatID, seatID) == 0 &&
                strcmp(allBookings[i].date, travelDate) == 0 &&
                allBookings[i].booked)
            {
                foundIndex = i;
                break;
            }
        }

        if (foundIndex == -1)
        {
            printf("No booked seat %s found on %s.\n", seatID, travelDate);
            continue;
        }

        // ยืนยันว่าถูกผู้โดยสารหรือไม่
        char confirm;
        printf("Seat %s is booked by: %s\n", seatID, allBookings[foundIndex].name);
        printf("Is this the passenger you want to edit? (y/n): ");
        scanf(" %c", &confirm);
        if (tolower(confirm) != 'y')
        {
            printf("Cancelled. Please enter another seat ID.\n\n");
            continue;
        }

        break;
    }

    // แสดงเมนูอัปเดต
    printf("\n--- Update Passenger (%s on %s) ---\n", seatID, travelDate);
    printf("Passenger: %s\n", allBookings[foundIndex].name);
    printf("1) Edit Name\n");
    printf("2) Change Seat (same flight)\n");
    printf("3) Change Flight (new flight + date + seat)\n");
    printf("4) Exit\n");
    printf("Select option: ");

    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n')
        ;

    if (choice == 4)
    {
        printf("Cancelled update.\n");
        return;
    }

    // 1) EDIT NAME
    if (choice == 1)
    {
        char newName[50];
        while (1)
        {
            printf("Current name: %s\n", allBookings[foundIndex].name);
            printf("Enter new passenger name (First Last): ");
            scanf(" %[^\n]", newName);
            trim(newName);

            if (strlen(newName) < 3 || strchr(newName, ' ') == NULL)
            {
                printf("Invalid name. Please enter full name.\n");
                continue;
            }

            printf("\nChange name from '%s' to '%s'? (y/n): ",
                   allBookings[foundIndex].name, newName);
            char confirm[8];
            scanf(" %7s", confirm);

            if (tolower(confirm[0]) == 'y')
            {
                strcpy(allBookings[foundIndex].name, newName);
                printf("Name updated successfully.\n");
                save_all_to_csv();
            }
            else
            {
                printf("Edit cancelled.\n");
            }
            break;
        }
        return;
    }

    // 2) CHANGE SEAT (ใน flight เดิม)
    else if (choice == 2)
    {
        char newSeat[8];
        while (1)
        {
            printf("Enter new seat ID (A1 -> C5): ");
            scanf("%7s", newSeat);
            to_lowercase(newSeat);
            while (getchar() != '\n')
                ;

            int seatTaken = 0;
            for (int i = 0; i < bookingCount; i++)
            {
                if (str_casecmp(allBookings[i].flightCode, f->code) == 0 &&
                    str_casecmp(allBookings[i].date, travelDate) == 0 &&
                    str_casecmp(allBookings[i].seatID, newSeat) == 0 &&
                    allBookings[i].booked)
                {
                    seatTaken = 1;
                    break;
                }
            }

            if (seatTaken)
            {
                printf("That seat is already booked. Try again.\n");
                continue;
            }

            strcpy(allBookings[foundIndex].seatID, newSeat);
            printf("Seat changed successfully to %s\n", newSeat);
            save_all_to_csv();
            return;
        }
    }

    // 3) CHANGE FLIGHT
    else if (choice == 3)
    {
        printf("\nAvailable Flights:\n");
        for (int i = 0; i < flightCount; i++)
        {
            printf("%d) %s (%s -> %s)\n", i + 1,
                   flights[i].code, flights[i].from, flights[i].to);
        }

        int destChoice;
        while (1)
        {
            printf("Select destination flight: ");
            scanf("%d", &destChoice);
            if (destChoice < 1 || destChoice > flightCount)
            {
                printf("Invalid flight selection. Try again.\n");
                continue;
            }
            break;
        }

        Flight *target = &flights[destChoice - 1];
        char newDate[20];
        char newSeat[8];

        // กรอกวันใหม่
        while (1)
        {
            printf("Enter new travel date (YYYY-MM-DD): ");
            scanf("%19s", newDate);
            if (is_valid_future_date(newDate))
                break;
        }
        while (getchar() != '\n')
            ;

        // เลือกที่นั่งใหม่
        while (1)
        {
            printf("Enter seat ID in %s (A1 -> C5): ", target->code);
            scanf("%7s", newSeat);
            to_lowercase(newSeat);
            while (getchar() != '\n')
                ;

            int seatTaken = 0;
            for (int i = 0; i < bookingCount; i++)
            {
                if (str_casecmp(allBookings[i].flightCode, target->code) == 0 &&
                    str_casecmp(allBookings[i].date, newDate) == 0 &&
                    str_casecmp(allBookings[i].seatID, newSeat) == 0 &&
                    allBookings[i].booked)
                {
                    seatTaken = 1;
                    break;
                }
            }

            if (seatTaken)
            {
                printf("That seat is already booked. Try again.\n");
                continue;
            }
            break;
        }

        printf("\nConfirm move %s -> %s (%s -> %s) Seat %s on %s? (y/n): ",
               f->code, target->code, target->from, target->to, newSeat, newDate);
        char confirm;
        scanf(" %c", &confirm);
        if (tolower(confirm) != 'y')
        {
            printf("Cancelled flight change.\n");
            return;
        }

        strcpy(allBookings[foundIndex].flightCode, target->code);
        strcpy(allBookings[foundIndex].from, target->from);
        strcpy(allBookings[foundIndex].to, target->to);
        strcpy(allBookings[foundIndex].date, newDate);
        strcpy(allBookings[foundIndex].seatID, newSeat);

        printf("Passenger moved successfully to %s seat %s on %s\n",
               target->code, newSeat, newDate);
        save_all_to_csv();
        return;
    }

    else
    {
        printf("Invalid choice.\n");
    }
}

// ========== SEARCH MENU ==========
void search_menu()
{
    int choice;

    while (1)
    {
        printf("\n====== SEARCH MENU ======\n");
        printf("1) Search by Passenger Name\n");
        printf("2) Search by Source (Booked Only)\n");
        printf("3) Search by Destination (Booked Only)\n");
        printf("4) Search by Travel Date (Booked Only)\n");
        printf("5) Search by Booking Code\n");
        printf("0) Back\n");
        printf("==========================\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        while (getchar() != '\n')
            ;

        if (choice == 0)
            return;

        if (choice < 1 || choice > 5)
        {
            printf("Invalid choice! Please select 1–5.\n");
            continue;
        }

        char keyword[50];
        printf("\nEnter keyword (0 to cancel): ");
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword, "\n")] = '\0';
        if (strcmp(keyword, "0") == 0)
            continue;

        to_lowercase(keyword);

        int found = 0;
        printf("\n=== Search Results ===\n");

        for (int i = 0; i < bookingCount; i++)
        {
            char nameLower[50], fromLower[30], toLower[30], dateLower[20], codeLower[10];
            strcpy(nameLower, allBookings[i].name);
            strcpy(fromLower, allBookings[i].from);
            strcpy(toLower, allBookings[i].to);
            strcpy(dateLower, allBookings[i].date);
            strcpy(codeLower, allBookings[i].code);
            to_lowercase(nameLower);
            to_lowercase(fromLower);
            to_lowercase(toLower);
            to_lowercase(dateLower);
            to_lowercase(codeLower);

            int match = 0;

            if (choice == 1 && strstr(nameLower, keyword))
                match = 1;
            else if (choice == 2 && strstr(fromLower, keyword) && allBookings[i].booked)
                match = 1;
            else if (choice == 3 && strstr(toLower, keyword) && allBookings[i].booked)
                match = 1;
            else if (choice == 4 && strcmp(dateLower, keyword) == 0 && allBookings[i].booked)
                match = 1;
            else if (choice == 5 && strcmp(codeLower, keyword) == 0)
                match = 1;

            if (match)
            {
                found = 1;
                printf("---------------------------------------------\n");
                printf("Flight: %s (%s -> %s)\n",
                       allBookings[i].flightCode,
                       allBookings[i].from,
                       allBookings[i].to);
                printf("Date: %s | Seat: %s | Passenger: %s | Code: %s | Status: %s\n",
                       allBookings[i].date,
                       allBookings[i].seatID,
                       allBookings[i].name[0] ? allBookings[i].name : "-",
                       allBookings[i].code[0] ? allBookings[i].code : "-",
                       allBookings[i].booked ? "Booked" : "Available");
            }
        }

        if (!found)
            printf("No results found for '%s'.\n", keyword);

        printf("\nSearch complete. Press ENTER to continue...");
        while (getchar() != '\n')
            ;
    }
}

// ========== SAVE TO CSV ==========

// ========== LOAD FROM CSV ==========
void load_from_csv()
{
    FILE *fp = fopen("passengers.csv", "r");
    if (!fp)
    {
        printf("No existing CSV found. Starting fresh.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // skip header
    bookingCount = 0;
    for (int i = 0; i < 1000; i++)
        memset(&allBookings[i], 0, sizeof(Booking));

    while (fgets(line, sizeof(line), fp))
    {
        // 🔹 ข้าม header ที่หลุดมาซ้ำ
        if (strstr(line, "FlightCode") != NULL)
            continue;

        Booking b;
        char status[20];
        line[strcspn(line, "\n")] = 0;

        sscanf(line, "%9[^,],%29[^,],%29[^,],%19[^,],%3[^,],%49[^,],%19[^,],%9[^,\n]",
               b.flightCode, b.from, b.to, b.date,
               b.seatID, b.name, status, b.code);

        b.booked = (strcmp(status, "Booked") == 0);

        // 🔹 กันข้อมูลซ้ำ (flight+seat+date เดิม)
        int duplicate = 0;
        for (int i = 0; i < bookingCount; i++)
        {
            if (strcmp(allBookings[i].flightCode, b.flightCode) == 0 &&
                strcmp(allBookings[i].seatID, b.seatID) == 0 &&
                strcmp(allBookings[i].date, b.date) == 0)
            {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate)
            allBookings[bookingCount++] = b;
    }

    fclose(fp);
    printf("Loaded %d unique bookings from CSV...\n", bookingCount);
}

void generate_unique_code(char code[10])
{
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int unique;

    for (int attempt = 0; attempt < 1000; attempt++) // ป้องกันค้าง
    {
        // สุ่มรหัส 6 ตัว
        for (int i = 0; i < 6; i++)
            code[i] = charset[rand() % (sizeof(charset) - 1)];
        code[6] = '\0';

        unique = 1; // สมมติว่ายังไม่ซ้ำ

        // ตรวจว่าซ้ำไหม
        for (int f = 0; f < flightCount; f++)
        {
            for (int s = 0; s < MAX_SEATS; s++)
            {
                // ถ้า code เดิมว่าง ไม่ต้องเทียบ
                if (flights[f].seats[s].code[0] == '\0' || strlen(flights[f].seats[s].code) < 3)
                    continue;

                // ถ้าเจอซ้ำ -> สุ่มใหม่
                if (strcmp(flights[f].seats[s].code, code) == 0)
                {
                    unique = 0;
                    break;
                }
            }
            if (!unique)
                break;
        }

        if (unique)
            return; // เจอ code ที่ไม่ซ้ำ -> ใช้เลย
    }

    // ถ้าเกิน 1000 รอบแล้วยังซ้ำ (เกือบเป็นไปไม่ได้) ให้ generate แบบ random ไปเลย
    sprintf(code, "R%05d", rand() % 100000);
}

void to_lowercase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32; // แปลงเป็นตัวพิมพ์เล็ก
    }
}
void save_all_to_csv()
{
    FILE *fp = fopen("passengers.csv", "w");
    if (!fp)
        return;

    fprintf(fp, "FlightCode,From,To,Date,SeatID,Name,Status,Code\n");
    for (int i = 0; i < bookingCount; i++)
    {
        fprintf(fp, "%s,%s,%s,%s,%s,%s,%s,%s\n",
                allBookings[i].flightCode,
                allBookings[i].from,
                allBookings[i].to,
                allBookings[i].date,
                allBookings[i].seatID,
                allBookings[i].name,
                allBookings[i].booked ? "Booked" : "Available",
                allBookings[i].code);
    }

    fclose(fp);
    printf("Saved all %d bookings!\n", bookingCount);
}

int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// ตรวจสอบวันที่ถูกต้องและต้องเป็นอนาคต
int is_valid_future_date(const char *dateStr)
{
    int y, m, d;
    if (sscanf(dateStr, "%d-%d-%d", &y, &m, &d) != 3)
    {
        printf("Invalid format. Please use YYYY-MM-DD\n");
        return 0;
    }

    //  ตรวจปี-เดือน-วันเบื้องต้น
    if (y < 2025 || m < 1 || m > 12 || d < 1 || d > 31)
    {
        printf("Invalid date components.\n");
        return 0;
    }

    //  จำนวนวันสูงสุดในแต่ละเดือน
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    //  ปรับกุมภา 29 วัน ถ้าเป็น leap year
    if (is_leap_year(y))
        daysInMonth[2] = 29;

    if (d > daysInMonth[m])
    {
        printf("Invalid day for month %d.\n", m);
        return 0;
    }

    //  ตรวจว่าต้องเป็น "วันนี้หรืออนาคต"
    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    int cy = now->tm_year + 1900;
    int cm = now->tm_mon + 1;
    int cd = now->tm_mday;

    if (y < cy || (y == cy && (m < cm || (m == cm && d < cd))))
    {
        printf("You can only book future dates.\n");
        return 0;
    }

    return 1;
}
int str_casecmp(const char *a, const char *b)
{
    while (*a && *b)
    {
        char ca = tolower((unsigned char)*a);
        char cb = tolower((unsigned char)*b);
        if (ca != cb)
            return ca - cb;
        a++;
        b++;
    }
    return *a - *b;
}

//  ตัดช่องว่างซ้ายขวาออก (กันบั๊กจาก CSV)
void trim(char *s)
{
    int len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1]))
        s[--len] = '\0';
    while (*s && isspace((unsigned char)*s))
        memmove(s, s + 1, len--);
}