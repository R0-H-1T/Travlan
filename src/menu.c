#include "menu.h"

void print_main_menu(WINDOW *menu_win, int highlight) {
  int xMax, yMax;

  getmaxyx(menu_win, yMax, xMax);

  char *choices[] = {
      "- Login",
      "- Register",
      "- Get Travel Recommendations",
      "- Add Custom Trip",
      "- Generate Trip Summary",
      "- Logout",
      "- Quit",
  };

  int num_choices = sizeof(choices) / sizeof(choices[0]);

  wclear(menu_win);

  mvwprintw(menu_win, 1, 1, "Welcome to Travlan");
  mvwprintw(menu_win, 2, 1, "------------------");

  for (int i = 0; i < num_choices; i++) {
    if (highlight == i + 1) /* Highlight the present choice */
    {
      wattron(menu_win, A_REVERSE);
      mvwprintw(menu_win, i + 3, 1, "%s", choices[i]);
      wattroff(menu_win, A_REVERSE);
    } else
      mvwprintw(menu_win, i + 3, 1, "%s", choices[i]);
  }

  wrefresh(menu_win);
}

int get_main_menu_choice(WINDOW *menu_win) {
  keypad(menu_win, TRUE);
  int choice = wgetch(menu_win);

  switch (choice) {
    case KEY_UP:
      return MENU_UP;
    case KEY_DOWN:
      return MENU_DOWN;
    case 10:  // ASCII Enter key
      return MENU_ENTER;
    case 27:  // ASCII Escape key
      return MENU_EXIT;
    default:
      return MENU_INVALID;
  }
}
