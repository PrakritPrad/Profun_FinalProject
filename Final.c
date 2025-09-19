#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <ctype.h>
#include <string.h>

#define MAX_BOOKINGS 100

// เก็บข้อมูลชั่วคราว
int ids[MAX_BOOKINGS];
char names[MAX_BOOKINGS][50];
char destinations[MAX_BOOKINGS][50];
char dates[MAX_BOOKINGS][20];
int bookingCount = 0;

void save_to_csv();
void load_from_csv();
void add_user();
void search_menu();
void search_user(const char *field);
void update_user();
void delete_user();

void trim_newline(char *s);
void display_menu();
void welcome_screen();
void to_lowercase(char *str);
int is_number(const char *str);
int is_name(const char *str);
int is_valid_date(const char *str);

void welcome_screen()
{
    system("cls");
    printf("=====================================\n");
    printf("🌟 Travel booking management system 🌟\n");
    printf("=====================================\n\n");
    Sleep(100);
}

void display_menu()
{
    printf("========== MAIN MENU ==========\n");
    printf("1. Save data to csv\n");
    printf("2. Read data from csv\n");
    printf("3. Add data\n");
    printf("4. Search data\n");
    printf("5. Update data\n");
    printf("6. Delete data\n");
    printf("7. Display menu\n");
    printf("8. Exit\n");
    printf("===============================\n");
}

int main()
{
    int choice;
    welcome_screen();
    display_menu();

    while (1)
    {
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
            display_menu();
            break;
        case 8:
            printf("Exiting program...\n");
            return 0;
        }
    }
    return 0;
}

void save_to_csv()
{
    FILE *fp = fopen("data.csv", "a");
    if (!fp)
    {
        printf("Error opening file!\n");
        return;
    }

    for (int i = 0; i < bookingCount; i++)
    {
        fprintf(fp, "%d,%s,%s,%s\n", ids[i], names[i], destinations[i], dates[i]);
    }
    fclose(fp);

    printf("Saved %d bookings to data.csv\n", bookingCount);
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
    printf("------------------------------------------------------------\n");
    printf("| %-5s | %-15s | %-15s | %-12s |\n", "ID", "Name", "Destination", "Date");
    printf("------------------------------------------------------------\n");

    while (fgets(line, sizeof(line), fp))
    {
        line[strcspn(line, "\n")] = 0;
        char *id = strtok(line, ",");
        char *name = strtok(NULL, ",");
        char *destination = strtok(NULL, ",");
        char *date = strtok(NULL, ",");

        if (id && name && destination && date)
            printf("| %-5s | %-15s | %-15s | %-12s |\n", id, name, destination, date);
    }
    printf("------------------------------------------------------------\n");
    fclose(fp);
}

void add_user()
{
    char id_str[10], name[50], destination[50], date[20];
    int id;

    // รับ ID
    while (1)
    {
        printf("Enter id (numbers only): ");
        scanf("%9s", id_str);
        getchar();
        if (is_number(id_str))
        {
            id = atoi(id_str);
            break;
        }
        printf("Invalid ID! Try again.\n");
    }

    // รับ Name
    while (1)
    {
        printf("Enter name (letters only, allow spaces): ");
        if (fgets(name, sizeof(name), stdin) == NULL)
        {
            continue; // ถ้าอ่านไม่ได้ก็วนใหม่
        }
        trim_newline(name); // ตัด \n ถ้ามี

        if (is_name(name))
            break;

        printf("Invalid Name! Try again.\n");
    }

    // รับ Destination
    while (1)
    {
        printf("Enter destination (letters only): ");
        scanf("%49s", destination);

        if (is_name(destination))
            break;
        printf("Invalid Destination! Try again.\n");
    }

    // รับ Date
    while (1)
    {
        printf("Enter date (YYYY-MM-DD): ");
        scanf("%19s", date);
        if (is_valid_date(date))
            break;
        printf("Invalid Date format! Try again.\n");
    }

    // บันทึกลง array
    if (bookingCount < MAX_BOOKINGS)
    {
        ids[bookingCount] = id;
        strcpy(names[bookingCount], name);
        strcpy(destinations[bookingCount], destination);
        strcpy(dates[bookingCount], date);
        bookingCount++;
        printf("Booking stored in memory (not saved yet)\n");
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
    printf("1. Search by Name\n");
    printf("2. Search by Destination\n");
    printf("3. Search by Date\n");
    printf("Enter choice: ");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        search_user("name");
        break;
    case 2:
        search_user("destination");
        break;
    case 3:
        search_user("date");
        break;
    default:
        printf("Invalid choice!\n");
    }
}

void search_user(const char *field)
{
    char keyword[50];
    char line[256];
    char temp[256];

    printf("Enter %s to search: ", field);
    scanf("%49s", keyword);
    to_lowercase(keyword);

    FILE *fp = fopen("data.csv", "r");
    if (!fp)
    {
        printf("Error opening file!\n");
        return;
    }

    int found = 0;
    printf("\n------ Search Result (%s) ------\n", field);
    while (fgets(line, sizeof(line), fp))
    {
        strcpy(temp, line);
        to_lowercase(temp);
        if (strstr(temp, keyword))
        {
            printf("%s", line);
            found = 1;
        }
    }
    fclose(fp);

    if (!found)
        printf("No records found for %s\n", keyword);
    printf("--------------------------------\n");
}

void update_user()
{
    printf("Update user (not implemented yet)\n");
}

void delete_user()
{
    printf("Delete user (not implemented yet)\n");
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
    if (sscanf(str, "%4d-%2d-%2d", &y, &m, &d) != 3)
        return 0;
    if (m < 1 || m > 12)
        return 0;
    if (d < 1 || d > 31)
        return 0;
    return 1;
}
