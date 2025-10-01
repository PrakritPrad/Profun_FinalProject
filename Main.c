#include <stdio.h>
#include <stdlib.h>
#include <windows.h> //ยังไม่ได้ทำ cross platform อย่าลืมม
#include <ctype.h>
#include <string.h>
#include "Unit_E2E.h"
#include <time.h> 

#define MAX_BOOKINGS 100

// เก็บข้อมูลชั่วคราววว
int ids[MAX_BOOKINGS];
char names[MAX_BOOKINGS][50];
char destinations[MAX_BOOKINGS][50];
char dates[MAX_BOOKINGS][20];
char booking_codes[MAX_BOOKINGS][10];
int bookingCount = 0;

void save_to_csv();
void load_from_csv();
void add_user();
void search_menu();
void search_user(const char *field);
void update_user();
void delete_user();
void sync_from_csv();

void trim_newline(char *s);
void to_lowercase(char *str);
int is_number(const char *str);
int is_name(const char *str);
int is_valid_date(const char *str);
void reset_ids();
void generate_unique_code(char code[10]);
void display_menu();
void welcome_screen();

void welcome_screen()
{
    system("cls");
    printf("=====================================\n");
    printf("🌟 Travel booking management system 🌟\n");
    printf("=====================================\n\n");
    Sleep(1000);
}

void display_menu()
{
    printf("========== MAIN MENU ==========\n");
    printf("1. Save data to csv\n");
    printf("2. Load data from csv\n");
    printf("3. Add data\n");
    printf("4. Search data\n");
    printf("5. Update data\n");
    printf("6. Delete data\n");
    printf("7. Unit Test\n");
    printf("8. E2E Test\n");
    printf("9. Exit\n");
    printf("===============================\n");
}

int main()
{
    int choice;
    welcome_screen();
    sync_from_csv();
    srand(time(NULL)); //กัน seed ซ้ำตอนเปิดโปรแกรมใหม่
    while (1)
    {
        display_menu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number between 1 and 8.\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
            {
            }
            continue;
        }

        if (choice < 1 || choice > 8)
        {
            printf("Invalid input! Please enter a number between 1 and 8.\n");
            continue;
        }

        switch (choice)
        {
        case 1:
            save_to_csv();
            break;
        case 2:
            load_from_csv();
            break;
        case 3:
            add_user();
            break;
        case 4:
            search_menu();
            break;
        case 5:
            update_user();
            break;
        case 6:
            delete_user();
            break;
        case 7:
            unit_test();
            printf("\nPlease press ENTER to go back to menu...");
            int c;
            while ((c = getchar()) != '\n' && c != EOF){}
            getchar();
            break;
            case 8:
            e2e_test();
            printf("\nPlease press ENTER to go back to menu...");
            while ((c = getchar()) != '\n' && c != EOF){}
            getchar();
            break;
        case 9:
            printf("Exiting program...\n");
            return 0;
        }
    }
    return 0;
}

void save_to_csv()
{
    FILE *fp = fopen("data.csv", "w");
    if (!fp)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < bookingCount; i++)
    {
        fprintf(fp, "%d,%s,%s,%s,%s\n", i + 1, booking_codes[i], names[i], destinations[i], dates[i]);
    }
    fclose(fp);

    printf("Save to csv successfully!\n");
}

void load_from_csv()
{
    FILE *fp = fopen("data.csv", "r");
    if (!fp)
    {
        printf("Error opening file!\n");
        return;
    }

    char line[256];
    printf("-------------------------------------------------------------------------------\n");
    printf("| %-5s | %-8s | %-20s | %-15s | %-12s |\n",
           "ID", "Booking ID", "Name", "Destination", "Date");
    printf("-------------------------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = 0;

        // อ่านตามลำดับ ID, Code, Name, Destination, Date
        char *id = strtok(line, ",");
        char *code = strtok(NULL, ",");
        char *name = strtok(NULL, ",");
        char *destination = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (id && code && name && destination && date)
        {
            printf("| %-5s | %-10s | %-20s | %-15s | %-12s |\n",
                   id, code, name, destination, date);
        }
    }

    printf("-------------------------------------------------------------------------------\n");
    printf("\nPlease press ENTER to go back to menu...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
    getchar();

    fclose(fp);
}

void add_user()
{
    char name[50], destination[50], date[20];

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // clear buffer skibidi

    // Name sybau
    while (1)
    {
        printf("Enter name (letters only, allow spaces): ");
        if (fgets(name, sizeof(name), stdin) == NULL)
            continue;
        trim_newline(name);
        if (is_name(name))
            break;
        printf("Invalid Name! Try again.\n");
    }

    // Destination
    while (1)
    {
        printf("Enter destination (letters only, allow spaces): ");
        if (fgets(destination, sizeof(destination), stdin) == NULL)
            continue;
        trim_newline(destination);
        if (is_name(destination))
            break;
        printf("Invalid Destination! Try again.\n");
    }

    // Date
    while (1)
    {
        printf("Enter date (YYYY-MM-DD): ");
        if (fgets(date, sizeof(date), stdin) == NULL)
            continue;
        trim_newline(date);
        if (is_valid_date(date))
            break;
        printf("Invalid Date format! Try again.\n");
    }

    if (bookingCount < MAX_BOOKINGS)
    {
        generate_unique_code(booking_codes[bookingCount]); 

        strcpy(names[bookingCount], name);
        strcpy(destinations[bookingCount], destination);
        strcpy(dates[bookingCount], date);

        printf("-----------------------------\n");
        printf("\nBooking stored with ID %d\n", bookingCount + 1);
        printf("Your booking ID is: %s\n", booking_codes[bookingCount]); // << แสดงรหัส
        printf("-----------------------------\n");
        printf("Please Save to confirm!!\n");

        bookingCount++;
        printf("\nPlease press ENTER to go back to menu...");
        while ((c = getchar()) != '\n' && c != EOF)
        {
        }
    }

    else
    {
        printf("Memory full, cannot add more!\n");
    }
}

void search_menu()
{
    int option;
    printf("\n====== Search Menu ======\n");
    printf("1. Search by Booking ID\n");
    printf("2. Search by Name\n");
    printf("3. Search by Destination\n");
    printf("4. Search by Date\n");
    printf("==== Enter 0 to Exit ====\n");
    printf("Enter choice: ");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        search_user("code");
        break;
    case 2:
        search_user("name");
        break;
    case 3:
        search_user("destination");
        break;
    case 4:
        search_user("date");
        break;
    case 0:
        printf("Exiting search menu...\n");
        Sleep(1000);
        return;
        break;
    default:
        printf("Invalid choice!\n");
    }
}

void search_user(const char *field)
{
    char keyword[50];
    int found = 0;

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }

    while (1)
    {

        printf("Enter %s to search: ", field);
        fgets(keyword, sizeof(keyword), stdin);
        trim_newline(keyword);
        to_lowercase(keyword);

        if (strlen(keyword) == 0)
        {
            printf("Search keyword cannot be empty!\n");
            continue;
        }
        break;
    }

    if (strcmp(field, "code") == 0)
    {
        for (int i = 0; i < bookingCount; i++)
        {
            char temp[10];
            strcpy(temp, booking_codes[i]); // copy code เดิมมา
            to_lowercase(temp);             // แปลงเป็นตัวเล็ก

            if (strcmp(keyword, temp) == 0) // เทียบกับ keyword ที่เป็น lower แล้ว
            {
                printf("Booking Found:\n");
                printf("ID: %d | Code: %s | Name: %s | Destination: %s | Date: %s\n",
                       i + 1, booking_codes[i], names[i], destinations[i], dates[i]);
                found = 1;
                break;
            }
        }
    }

    else
    {
        // ค้นหาในไฟล์เหมือนเดิม
        FILE *fp = fopen("data.csv", "r");
        if (!fp)
        {
            printf("Error opening file!\n");
            return;
        }

        char line[256], temp[256];
        while (fgets(line, sizeof(line), fp))
        {
            strcpy(temp, line);
            to_lowercase(temp);

            if (strstr(temp, keyword))
            {
                line[strcspn(line, "\n")] = 0;
                char *id = strtok(line, ",");
                char *code = strtok(NULL, ",");
                char *name = strtok(NULL, ",");
                char *destination = strtok(NULL, ",");
                char *date = strtok(NULL, ",");

                if (id && code && name && destination && date)
                {
                    printf("ID: %s | Code: %s | Name: %s | Destination: %s | Date: %s\n",
                           id, code, name, destination, date);
                    found = 1;
                }
            }
        }
        fclose(fp);
    }

    if (!found)
        printf("No records found for %s\n", keyword);

    printf("\nPlease press ENTER to go back to menu...");
    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

void update_user()
{
    int id;
    while (1)
    {
        printf("Enter ID to update (0 to cancel): ");
        scanf("%d", &id);

        if (id == 0)
        {
            printf("Update canceled by user.\n");
            Sleep(1000);
            display_menu();
            return;
        }

        if (id < 1 || id > bookingCount)
        {
            printf("Invalid ID!\n");
            continue;
        }
        break;
    }

    id--; // index array

    printf("\nCurrent record:\n");
    printf("ID: %d | Name: %s | Destination: %s | Date: %s\n",
           id + 1, names[id], destinations[id], dates[id]);

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // clear buffer

    char confirm[10];
    printf("Do you want to update this record? (yes/no): ");
    fgets(confirm, sizeof(confirm), stdin);
    trim_newline(confirm);

    if (strcmp(confirm, "yes") != 0 && strcmp(confirm, "YES") != 0)
    {
        printf("Update canceled.\n");
        Sleep(1000);
        return;
    }

    // --- กรอกค่าใหม่ ---
    printf("Enter new name: ");
    fgets(names[id], sizeof(names[id]), stdin);
    trim_newline(names[id]);

    printf("Enter new destination: ");
    fgets(destinations[id], sizeof(destinations[id]), stdin);
    trim_newline(destinations[id]);

    printf("Enter new date (YYYY-MM-DD): ");
    fgets(dates[id], sizeof(dates[id]), stdin);
    trim_newline(dates[id]);

    if (!is_valid_date(dates[id]))
    {
        printf("Invalid date format! Update canceled.\n");
        return;
    }

    printf("Pleas Save to confirm!!!\n");
}

void delete_user()
{
    int id;
    while (1)
    {
        printf("Enter ID to delete (0 to cancel): ");
        scanf("%d", &id);

        if (id == 0)
        {
            printf("Delete canceled by user.\n");
            Sleep(1000);
            return;
        }

        if (id < 1 || id > bookingCount)
        {
            printf("Invalid ID! Please try again.\n");
            continue; // วนใหม่
        }

        break; // valid แล้ว ออกจาก loop
    }
    id--; // index

    printf("\nRecord to delete:\n");
    printf("ID: %d | Name: %s | Destination: %s | Date: %s\n", id + 1, names[id], destinations[id], dates[id]);

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
    } // clear buffer

    char confirm[10];
    printf("Are you sure you want to delete this record? (yes/no): ");
    fgets(confirm, sizeof(confirm), stdin);
    trim_newline(confirm);

    if (strcmp(confirm, "yes") == 0 || strcmp(confirm, "YES") == 0)
    {
        // Shift array
        for (int i = id; i < bookingCount - 1; i++)
        {
            strcpy(booking_codes[i], booking_codes[i + 1]);
            strcpy(names[i], names[i + 1]);
            strcpy(destinations[i], destinations[i + 1]);
            strcpy(dates[i], dates[i + 1]);
        }
        bookingCount--;

        printf("Pleas Save to confirm \n");
        Sleep(1000);
        display_menu();
    }
    else
    {
        printf("Delete canceled.\n");
        Sleep(1000);
    }
}

void sync_from_csv()
{
    FILE *fp = fopen("data.csv", "r");
    if (!fp)
        return;

    char line[256];
    bookingCount = 0;

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = 0;

        char *id = strtok(line, ",");
        char *code = strtok(NULL, ",");
        char *name = strtok(NULL, ",");
        char *destination = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (id && code && name && destination && date)
        {
            strcpy(booking_codes[bookingCount], code);
            strcpy(names[bookingCount], name);
            strcpy(destinations[bookingCount], destination);
            strcpy(dates[bookingCount], date);
            bookingCount++;
        }
    }
    fclose(fp);
}

void trim_newline(char *s)
{
    size_t n = strlen(s);
    if (n > 0 && s[n - 1] == '\n')
        s[n - 1] = '\0';
}

void to_lowercase(char *str)
{
    for (int i = 0; str[i]; i++)
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + 32;
}

int is_number(const char *str)
{
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i]))
            return 0;
    return 1;
}
int is_name(const char *str)
{
    int len = strlen(str);
    if (len < 2)
        return 0;
    for (int i = 0; str[i]; i++)
    {
        if (!isalpha((unsigned char)str[i]) && str[i] != ' ')
            return 0;
    }
    return 1;
}

int is_valid_date(const char *str)
{
    int y, m, d;
    if (sscanf(str, "%d-%d-%d", &y, &m, &d) != 3)
        return 0;
    if (y < 2025 || y > 2030)
        return 0;
    if (m < 1 || m > 12)
        return 0;
    if (d < 1 || d > 31)
        return 0;
    return 1;
}

void reset_ids()
{
    for (int i = 0; i < bookingCount; i++)
    {
        ids[i] = i + 1;
    }
}

void generate_unique_code(char code[10])
{
    const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int unique;
    do
    {
        for (int i = 0; i < 6; i++)
        {
            code[i] = charset[rand() % (sizeof(charset) - 1)];
        }
        code[6] = '\0';
        unique = 1;
        for (int j = 0; j < bookingCount; j++)
        {
            if (strcmp(code, booking_codes[j]) == 0)
            {
                unique = 0;
                break;
            }
        }
    } while (!unique);
}
