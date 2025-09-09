#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


void welcome_screen();
void display_menu();
void save_to_csv();
void load_from_csv();
void add_user();
void search_user();
void update_user();
void delete_user();

void welcome_screen()
{
    system("cls");

    printf("=====================================\n");
    printf("🌟 Travel booking management system 🌟\n");
    printf("=====================================\n");

    Sleep(2000);
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

    while (1)
    {
        display_menu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

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
            search_user();
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
            printf("Exiting program\n");
            return 0;
        default:
            printf("Invalid choice\n");
        }

        Sleep(1000);
        system("cls");
    }

    return 0;
}


void save_to_csv()
{
    printf("Save data to csv...\n");
}

void load_from_csv()
{
    printf("Read data from csv...\n");
}

void add_user()
{
    printf("Add user...\n");
}

void search_user()
{
    printf("Search user...\n");
}

void update_user()
{
    printf("Update user...\n");
}

void delete_user()
{
    printf("Delete user...\n");
}
