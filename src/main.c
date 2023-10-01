#include <ncurses.h>
#include <stdio.h>

#include <menu.c>

int main() {
  // Initialization of curses
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);

  WINDOW *menu_win = newwin(10, xMax - 2, 0, 0);
  keypad(menu_win, TRUE);

  int choice;
  int highlight = 1;

  while (1) {
    print_main_menu(menu_win, highlight);
    choice = get_main_menu_choice(menu_win);

    switch (choice) {
      case MENU_LOGIN:
        // Implement login functionality
        break;
      case MENU_REGISTER:
        // Implement registration functionality
        break;
      case MENU_GET_RECOMMENDATIONS:
        // Implement get travel recommendations functionality
        break;
      case MENU_ADD_TRIP:
        // Implement add custom trips functionality
        break;
      case MENU_GENERATE_SUMMARY:
        // Implement generate trip summary functionality
        break;
      case MENU_LOGOUT:
        // Implement logout functionality
        break;
      case MENU_QUIT:
        endwin();
        return 0;
    }
  }

  return 0;
}
