#ifndef MENU_H
#define MENU_H

#include <ncurses.h>

// Define constants for menu choices
#define MENU_LOGIN 1
#define MENU_REGISTER 2
#define MENU_GET_RECOMMENDATIONS 3
#define MENU_ADD_TRIP 4
#define MENU_GENERATE_SUMMARY 5
#define MENU_LOGOUT 6
#define MENU_QUIT 7

// Define constants for user input
#define MENU_UP 65
#define MENU_DOWN 66
#define MENU_ENTER 10  // ASCII value for Enter
#define MENU_EXIT 27   // ASCII value for Escape
#define MENU_INVALID -1

// Function declarations
void print_main_menu(WINDOW *menu_win, int highlight);
int get_main_menu_choice(WINDOW *menu_win);

#endif  // MENU_H
