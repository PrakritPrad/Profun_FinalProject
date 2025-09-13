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
    display_menu();
    while (1)
    {
       
        printf("Enter your choice: ");
        scanf(" %d", &choice);

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

        //Sleep(1000);
        //system("cls");
    }

    return 0;
}

void save_to_csv()
{
    printf("Save data to csv...\n");
    Sleep(1000);
    FILE *fp = fopen("data.csv", "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fprintf(fp, "id,name,destination,date\n");
    fprintf(fp, "1,John,Tokyo,2025-09-13\n");
    fprintf(fp, "2,Alice,Bangkok,2025-09-14\n");

    fclose(fp);
    printf("Data saved to data.csv\n");
    Sleep(500);
    display_menu();
}

void load_from_csv()
{
    printf("Load data from csv...\n");
    Sleep(200);
    FILE  *fp = fopen("data.csv", "r");
    if(fp == NULL){
        printf("Error opening file!\n");
    }
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    fclose(fp);

    // ล้าง buffer จาก scanf
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    printf("\n-Please Enter to back to menu-");
    getchar();
    display_menu();
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
