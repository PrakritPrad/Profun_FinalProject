#ifndef UTILS_H
#define UTILS_H

#define MAX_BOOKINGS 100

extern int ids[MAX_BOOKINGS];
extern char names[MAX_BOOKINGS][50];
extern char destinations[MAX_BOOKINGS][50];
extern char dates[MAX_BOOKINGS][20];
extern char booking_codes[MAX_BOOKINGS][10];
extern int bookingCount;

void trim_newline(char *s);
void to_lowercase(char *str);
int is_number(const char *str);
int is_name(const char *str);
int is_valid_date(const char *str);
void generate_unique_code(char code[10]);
void reset_ids();
void unit_test();
void e2e_test();

#endif
