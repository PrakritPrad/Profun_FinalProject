#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  


#define MAX_FLIGHTS 3
#define MAX_SEATS 15

typedef struct
{
    char seatID[4];
    char name[50];
    int booked; // 0=available, 1=booked
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

// ==== Function Prototypes ====
void init_flights();
void main_menu();
void manage_flight(Flight *f);
void view_seats(Flight *f);
void add_passenger(Flight *f);
void cancel_passenger(Flight *f);
void change_seat(Flight *f);
void search_menu();
void save_to_csv();
void load_from_csv();
void update_passenger(Flight *f);
void generate_unique_code(char code[10]);

// =============================
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
    strcpy(flights[0].date, "2025-10-20");

    strcpy(flights[1].code, "FL002");
    strcpy(flights[1].from, "Chiangmai");
    strcpy(flights[1].to, "Seoul");
    strcpy(flights[1].date, "2025-10-23");

    strcpy(flights[2].code, "FL003");
    strcpy(flights[2].from, "Phuket");
    strcpy(flights[2].to, "Tokyo");
    strcpy(flights[2].date, "2025-11-01");

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
            printf("%d) %s %s -> %s (%s)\n", i + 1,
                   flights[i].code, flights[i].from, flights[i].to, flights[i].date);
        }
        printf("4) Search\n");
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
            printf("Please enter a number.\n");
            continue;
        }

        // แปลงเป็นตัวเลข
        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            continue;
        }

        // ตรวจช่วงค่าถูกไหม
        if (choice < 0 || choice > flightCount && choice != 4)
        {
            printf("Invalid choice!\n");
            continue;
        }

        if (choice == 0)
        {
            save_to_csv();
            printf("Goodbye!\n");
            break;
        }
        else if (choice == 4)
        {

            search_menu();

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

        printf("\n--- Manage %s (%s -> %s, %s) ---\n",
               f->code, f->from, f->to, f->date);

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
            printf("Please enter a number.\n");
            continue;
        }

        if (sscanf(input, "%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
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
            printf("Invalid choice! Please enter 1–5.\n");
        }
    }
}

// ========== VIEW SEATS ==========
void view_seats(Flight *f)
{
    printf("\nPassenger list for %s (%s -> %s, %s)\n",
           f->code, f->from, f->to, f->date);
    printf("-------------------------------------------------------\n");
    printf("%-6s | %-20s\n", "Seat", "Name");
    printf("-------------------------------------------------------\n");

    int bookedCount = 0;
    for (int i = 0; i < MAX_SEATS; i++)
    {
        printf("%-6s | %-20s\n",
               f->seats[i].seatID,
               f->seats[i].booked ? f->seats[i].name : "-");
        if (f->seats[i].booked)
            bookedCount++;
    }
    printf("-------------------------------------------------------\n");
    printf("Total booked: %d / %d seats\n", bookedCount, MAX_SEATS);

    // seat map ต่อท้าย
    printf("\nSeat map for %s (%s -> %s, %s)\n", f->code, f->from, f->to, f->date);
    printf("----------------------------------\n");
    for (int i = 0; i < MAX_SEATS; i++)
    {
        printf("%s[%s]\t", f->seats[i].seatID, f->seats[i].booked ? "X" : "O");
        if ((i + 1) % 5 == 0)
            printf("\n");
    }
    printf("----------------------------------\n");
}

// ========== ADD PASSENGER ==========
void add_passenger(Flight *f)
{
    char seat[4];
    int seatFound = 0;

    while (1)
    {
        printf("Enter seat ID to book (A1->C5): ");
        scanf("%s", seat);
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

                if (f->seats[i].booked)
                {
                    printf("\nSeat %s is already booked! Please choose another.\n", f->seats[i].seatID);
                    break; // กลับไปให้กรอกใหม่
                }

                while (1)
                {
                    printf("Enter passenger name (First Last): ");
                    scanf(" %[^\n]", f->seats[i].name);

                    // trim ช่องว่างหัวท้าย
                    while (f->seats[i].name[0] == ' ')
                        memmove(f->seats[i].name, f->seats[i].name + 1, strlen(f->seats[i].name));

                    // ตรวจชื่อว่างหรือไม่มีนามสกุล
                    if (strlen(f->seats[i].name) < 3)
                    {
                        printf("Name too short! Please enter full name.\n");
                        continue;
                    }

                    if (strchr(f->seats[i].name, ' ') == NULL)
                    {
                        printf("Please enter first and last name (ex, John Doe).\n");
                        continue;
                    }

                    break;
                }

                f->seats[i].booked = 1;
                printf("\nSeat %s booked successfully for %s!\n",
                       f->seats[i].seatID, f->seats[i].name);
                save_to_csv();
                return;
            }
        }

        if (!seatFound)
        {
            printf("Seat not found! Please try again (A1->C5).\n");
        }
    }
}

// ========== CANCEL PASSENGER ==========
void cancel_passenger(Flight *f)
{
    char seat[16]; // เผื่อ buffer กว้างขึ้น
    int seatFound = 0;

    while (1)
    {
        printf("Enter seat ID to cancel (A1 -> C5, or 0 to cancel): ");

        // ใช้ fgets เพื่อกัน enter ว่าง
        if (!fgets(seat, sizeof(seat), stdin))
        {
            clearerr(stdin);
            continue;
        }

        // ตัด newline ออก
        seat[strcspn(seat, "\n")] = '\0';

        // ถ้า user พิมพ์ 0 → ยกเลิก
        if (strcmp(seat, "0") == 0)
        {
            printf("Cancel operation aborted.\n");
            return;
        }

        // ถ้ากด Enter เฉย ๆ หรือช่องว่าง
        if (strlen(seat) == 0)
        {
            printf("Please enter a seat ID (A1–C5).\n");
            continue;
        }

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

                if (!f->seats[i].booked)
                {
                    printf("Seat %s is already available.\n", f->seats[i].seatID);
                    break; // ให้วนต่อเพื่อเลือกใหม่
                }

                // confirm ก่อนลบ
                char confirm[8];
                printf("Are you sure you want to cancel %s (%s)? (y/n): ",
                       f->seats[i].seatID, f->seats[i].name);
                fgets(confirm, sizeof(confirm), stdin);
                if (confirm[0] != 'y' && confirm[0] != 'Y')
                {
                    printf("Cancellation aborted.\n");
                    return;
                }

                f->seats[i].booked = 0;
                strcpy(f->seats[i].name, "");
                printf("Seat %s is now available.\n", f->seats[i].seatID);
                save_to_csv();
                return;
            }
        }

        if (!seatFound)
        {
            printf("Seat not found. Please try again (A1–C5).\n");
        }
    }
}

void update_passenger(Flight *f)
{
    char seatID[4];
    int index = -1;

    while (1)
    {
        printf("Enter seat ID to update: ");
        scanf("%s", seatID);
        to_lowercase(seatID);

        for (int i = 0; i < MAX_SEATS; i++)
        {
            char seatLower[4];
            strcpy(seatLower, f->seats[i].seatID);
            to_lowercase(seatLower);

            if (strcmp(seatLower, seatID) == 0)
            {
                if (!f->seats[i].booked)
                {
                    printf("That seat is empty. Please choose a booked seat.\n");
                    index = -1;
                }
                else
                {
                    index = i;
                }
                break;
            }
        }

        if (index == -1)
        {
            printf("Seat not found or not booked. Try again.\n");
            continue;
        }
        else
        {
            break;
        }
    }

    printf("\n--- Update Passenger (%s) ---\n", seatID);
    printf("Passenger: %s\n", f->seats[index].name);
    printf("1) Edit Name\n");
    printf("2) Change Seat\n");
    printf("3) Change Flight\n");
    printf("4) Exit\n");
    printf("Select option: ");

    int choice;
    scanf("%d", &choice);

    if (choice == 4)
    {
        printf("Cancelled update.\n");
        return;
    }

    char confirm;
    printf("Are you sure you want to continue? (y/n): ");
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y')
    {
        printf("Update aborted.\n");
        return;
    }

    // 1) EDIT NAME
    if (choice == 1)
    {
        char newName[50];
        while (1)
        {
            printf("Current name: %s\n", f->seats[index].name);
            printf("Enter new name (First Last): ");
            scanf(" %[^\n]", newName);

            // trim ช่องว่างหัวท้าย
            while (newName[0] == ' ')
                memmove(newName, newName + 1, strlen(newName));

            // ตรวจชื่อว่างหรือไม่มีนามสกุล
            if (strlen(newName) < 3)
            {
                printf("Name too short. Please enter full name.\n");
                continue;
            }

            if (strchr(newName, ' ') == NULL)
            {
                printf("Please enter both first and last name.\n");
                continue;
            }

            break;
        }

        strcpy(f->seats[index].name, newName);
        printf("Name updated successfully.\n");
        save_to_csv();
        return;
    }

    // 2) CHANGE SEAT
    else if (choice == 2)
    {
        char newSeat[4];
        int valid = 0;
        while (!valid)
        {
            printf("Enter new seat ID: ");
            scanf("%s", newSeat);
            to_lowercase(newSeat);

            for (int i = 0; i < MAX_SEATS; i++)
            {
                char seatLower[4];
                strcpy(seatLower, f->seats[i].seatID);
                to_lowercase(seatLower);

                if (strcmp(seatLower, newSeat) == 0)
                {
                    if (f->seats[i].booked)
                    {
                        printf("New seat is already booked. Try another.\n");
                        valid = 0;
                    }
                    else
                    {
                        strcpy(f->seats[i].name, f->seats[index].name);
                        f->seats[i].booked = 1;
                        f->seats[index].booked = 0;
                        strcpy(f->seats[index].name, "");
                        printf("%s moved from %s -> %s successfully.\n",
                               f->seats[i].name, seatID, f->seats[i].seatID);
                        save_to_csv();
                        return;
                    }
                    valid = 1;
                    break;
                }
            }
            if (!valid)
                printf("Seat not found. Please try again.\n");
        }
    }

    // 3) CHANGE FLIGHT
    else if (choice == 3)
    {
        printf("\nAvailable Flights:\n");
        for (int i = 0; i < flightCount; i++)
        {
            printf("%d) %s (%s -> %s, %s)\n",
                   i + 1, flights[i].code, flights[i].from, flights[i].to, flights[i].date);
        }

        int destChoice;
        while (1)
        {
            printf("Select destination flight: ");
            scanf("%d", &destChoice);
            if (destChoice < 1 || destChoice > flightCount)
            {
                printf("Invalid selection. Try again.\n");
                continue;
            }
            break;
        }

        Flight *target = &flights[destChoice - 1];

        if (target == f)
        {
            printf("You are already in this flight.\n");
            return;
        }

        char targetSeat[4];
        int foundSeat = -1;

        while (1)
        {
            printf("Enter seat ID in target flight: ");
            scanf("%s", targetSeat);
            to_lowercase(targetSeat);

            for (int i = 0; i < MAX_SEATS; i++)
            {
                char seatLower[4];
                strcpy(seatLower, target->seats[i].seatID);
                to_lowercase(seatLower);
                if (strcmp(seatLower, targetSeat) == 0)
                {
                    foundSeat = i;
                    break;
                }
            }

            if (foundSeat == -1)
            {
                printf("Seat not found. Try again.\n");
                continue;
            }

            if (target->seats[foundSeat].booked)
            {
                printf("That seat is already booked. Try again.\n");
                foundSeat = -1;
                continue;
            }

            break;
        }

        printf("Move %s from %s (%s -> %s, %s) to %s (%s -> %s, %s) seat %s? (y/n): ",
               f->seats[index].name,
               f->code, f->from, f->to, f->date,
               target->code, target->from, target->to, target->date,
               targetSeat);

        scanf(" %c", &confirm);
        if (confirm != 'y' && confirm != 'Y')
        {
            printf("Cancelled moving passenger.\n");
            return;
        }

        strcpy(target->seats[foundSeat].name, f->seats[index].name);
        target->seats[foundSeat].booked = 1;
        f->seats[index].booked = 0;
        strcpy(f->seats[index].name, "");

        printf("Passenger moved successfully.\n");
        printf("Now in flight %s seat %s.\n",
               target->code, target->seats[foundSeat].seatID);
        save_to_csv();
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
        printf("4) Search by Flight Date (Exact Match)\n");
        printf("0) Back\n");
        printf("==========================\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 0)
            return;

        char keyword[50];
        while (1)
        {
            printf("\nEnter keyword (0 to cancel): ");
            scanf(" %[^\n]", keyword);

            // ถ้าพิมพ์ 0 -> กลับเมนูก่อนหน้า
            if (strcmp(keyword, "0") == 0)
                break;

            to_lowercase(keyword);
            int found = 0;

            for (int f = 0; f < flightCount; f++)
            {
                char destLower[30], sourceLower[30], dateLower[20];
                strcpy(destLower, flights[f].to);
                strcpy(sourceLower, flights[f].from);
                strcpy(dateLower, flights[f].date);
                to_lowercase(destLower);
                to_lowercase(sourceLower);
                to_lowercase(dateLower);

                for (int s = 0; s < MAX_SEATS; s++)
                {
                    char nameLower[50];
                    strcpy(nameLower, flights[f].seats[s].name);
                    to_lowercase(nameLower);

                    int match = 0;

                    if (choice == 1 && flights[f].seats[s].booked &&
                        strstr(nameLower, keyword))
                        match = 1;

                    else if (choice == 2 && flights[f].seats[s].booked &&
                             strstr(sourceLower, keyword))
                        match = 1;

                    else if (choice == 3 && flights[f].seats[s].booked &&
                             strstr(destLower, keyword))
                        match = 1;

                    // ✅ Date ต้องตรงทั้งหมดเท่านั้น
                    else if (choice == 4 && strcmp(dateLower, keyword) == 0)
                        match = 1;

                    if (match)
                    {
                        found = 1;
                        printf("---------------------------------------------\n");
                        printf("Flight: %s (%s -> %s, %s)\n",
                               flights[f].code, flights[f].from, flights[f].to, flights[f].date);
                        printf("Seat: %s | Passenger: %s | Status: %s\n",
                               flights[f].seats[s].seatID,
                               flights[f].seats[s].name[0] ? flights[f].seats[s].name : "-",
                               flights[f].seats[s].booked ? "Booked" : "Available");
                    }
                }
            }

            if (!found)
            {
                printf("No results found for '%s'. Please try again.\n", keyword);
            }
            else
            {
                int c;
                printf("\nPlease press ENTER to go back to menu...");
                while ((c = getchar()) != '\n' && c != EOF)
                {
                }
                getchar();
                break;
            }
        }
    }
}

// ========== SAVE TO CSV ==========
void save_to_csv()
{
    FILE *fp = fopen("passengers.csv", "w");
    if (!fp)
        return;
    fprintf(fp, "FlightCode,From,To,Date,SeatID,Name,Status\n");

    for (int f = 0; f < flightCount; f++)
    {
        for (int s = 0; s < MAX_SEATS; s++)
        {
            fprintf(fp, "%s,%s,%s,%s,%s,%s,%s\n",
                    flights[f].code,
                    flights[f].from,
                    flights[f].to,
                    flights[f].date,
                    flights[f].seats[s].seatID,
                    flights[f].seats[s].name,
                    flights[f].seats[s].booked ? "Booked" : "Available");
        }
    }
    fclose(fp);
}

// ========== LOAD FROM CSV ==========
void load_from_csv()
{
    FILE *fp = fopen("passengers.csv", "r");
    if (!fp)
        return;

    char line[256];
    fgets(line, sizeof(line), fp); // skip header

    while (fgets(line, sizeof(line), fp))
    {
        char code[10] = "", from[30] = "", to[30] = "", date[20] = "";
        char seat[4] = "", name[50] = "", status[20] = "";

        // ตัด newline ออกก่อน
        line[strcspn(line, "\n")] = 0;

        // ✅ ใช้ strtok() อ่านแต่ละช่องแบบไม่พังเมื่อเจอช่องว่าง
        char *token = strtok(line, ",");
        if (token)
            strcpy(code, token);
        token = strtok(NULL, ",");
        if (token)
            strcpy(from, token);
        token = strtok(NULL, ",");
        if (token)
            strcpy(to, token);
        token = strtok(NULL, ",");
        if (token)
            strcpy(date, token);
        token = strtok(NULL, ",");
        if (token)
            strcpy(seat, token);
        token = strtok(NULL, ",");
        if (token)
            strcpy(name, token);
        token = strtok(NULL, ",");
        if (token)
            strcpy(status, token);

        // เชื่อมโยงกลับเข้า struct
        for (int f = 0; f < flightCount; f++)
        {
            if (strcmp(flights[f].code, code) == 0)
            {
                for (int s = 0; s < MAX_SEATS; s++)
                {
                    if (strcmp(flights[f].seats[s].seatID, seat) == 0)
                    {
                        strcpy(flights[f].seats[s].name, name);
                        flights[f].seats[s].booked = (strcmp(status, "Booked") == 0);
                        break;
                    }
                }
                break;
            }
        }
    }

    fclose(fp);
}

void to_lowercase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] += 32;
    }
}
