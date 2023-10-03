#include <database.c>
#include <login.c>
#include <mymenu.c>
#include <ncurses.h>
#include <stdio.h>

int
main ()
{
  // Initialization of curses
  initscr ();
  // Disable input buffering
  cbreak ();
  noecho ();
  // Allow Keyboard events
  keypad (stdscr, TRUE);

  // int yMax, xMax;
  // getmaxyx (stdscr, yMax, xMax);

  // setup database
  // bool database_initialized = false;
  // Database *db;
  // sqlite3 *db;
  init_database ();
  // create_users_table (db);
  // close_database (db);
  // if (init_database (db) && create_users_table (db) && close_database (db))
  //   {
  //     database_initialized = true;
  //   }
  // if (database_initialized)
  //   {
  //     mount_main_menu ();
  //   }
  mount_main_menu ();
  endwin ();

  // WINDOW *menu_win = newwin (10, xMax - 2, 0, 0);
  // keypad (menu_win, TRUE);

  // int choice;
  // int highlight = 1;

  // while (1)
  //   {
  //     mount_main_menu (menu_win, highlight);
  //     choice = get_main_menu_choice (menu_win);

  //     switch (choice)
  //       {
  //       case MENU_LOGIN:
  //         // Implement login functionality
  //         break;
  //       case MENU_REGISTER:
  //         // Implement registration functionality
  //         break;
  //       case MENU_GET_RECOMMENDATIONS:
  //         // Implement get travel recommendations functionality
  //         break;
  //       case MENU_ADD_TRIP:
  //         // Implement add custom trips functionality
  //         break;
  //       case MENU_GENERATE_SUMMARY:
  //         // Implement generate trip summary functionality
  //         break;
  //       case MENU_LOGOUT:
  //         // Implement logout functionality
  //         break;
  //       case MENU_QUIT:
  //         endwin ();
  //         return 0;
  //       }
  //   }

  return 0;
}
