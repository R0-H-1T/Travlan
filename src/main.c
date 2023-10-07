#include <auth.h>
#include <database.h>
#include <mymenu.h>
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

  int yMax, xMax;
  getmaxyx (stdscr, yMax, xMax);
  keypad (stdscr, false);
  curs_set (0);
  init_database ();
  mount_main_menu ();
  endwin ();
  return 0;
}
