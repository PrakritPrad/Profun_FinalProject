#ifndef BOOKING_SYSTEM_H
#define BOOKING_SYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// ====== CONFIG ======
#define MAX_FLIGHTS 3
#define MAX_SEATS 15

// ====== STRUCTS ======
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
    char date[20];
    Seat seats[MAX_SEATS];
} Flight;

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

// ====== GLOBAL VARIABLES ======
extern Flight flights[MAX_FLIGHTS];
extern int flightCount;

extern Booking allBookings[1000];
extern int bookingCount;

// ====== FUNCTION PROTOTYPES ======

// ─── Core ───
void init_flights();
void main_menu();
void manage_flight(Flight *f);
void view_seats(Flight *f);
void add_passenger(Flight *f);
void cancel_passenger(Flight *f);
void update_passenger(Flight *f);
void search_menu();
void Run_E2E_Tests(void);
    
// ─── CSV ───
void save_all_to_csv();
void load_from_csv();

// ─── Utilities ───
void generate_unique_code(char code[10]);
int is_seat_booked_on_date(Flight *f, const char *seatID, const char *date);
int is_leap_year(int year);
int is_valid_future_date(const char *dateStr);
int str_casecmp(const char *a, const char *b);
void trim(char *s);
void to_lowercase(char *str);

#endif // BOOKING_SYSTEM_H
