#include "util.h"
#include <csv.c>
#include <menu.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
recommendations_menu ()
{
  ITEM **my_items;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;
  // int n_choices, i;

  /* Initialize curses */
  // initscr ();
  // cbreak ();
  // noecho ();
  // keypad (stdscr, TRUE);

  /* Create items */
  // n_choices = ARRAY_SIZE (choices);
  // my_items = (ITEM **)calloc (n_choices, sizeof (ITEM *));
  // for (i = 0; i < n_choices; ++i)
  // my_items[i] = new_item (choices[i], choices[i]);

  // TODO
  TRIP *trips = load_recommendations_from_csv ();
  TRIP *temp = trips;

  int i = 0;
  while (temp->destination != NULL && temp->cost != NULL && temp->days != NULL)
    {
      i++;
      temp++;
    }

  temp = trips;
  // mvprintw (0, 0, "count of trips: %d", i);
  // refresh ();
  my_items = (ITEM **)calloc (i, sizeof (ITEM *));
  // reassigning it to trips
  // as temp has moved to the end of the memory block
  i = 0;
  while (temp->destination != NULL && temp->cost != NULL && temp->days != NULL)
    {
      my_items[i] = new_item ((const char *)temp->destination,
                              (const char *)temp->days);
      // TODO:  fix i trips are counted wrong
      if (i < 135)
        i++;
      temp++;
    }

  /* Crate menu */
  my_menu = new_menu ((ITEM **)my_items);

  /* Create the window to be associated with the menu */
  // my_menu_win = newwin (10, 40, 4, 4);
  my_menu_win = newwin (10, 40, (LINES - 10) / 2, (COLS - 40) / 2);
  // int y, x;
  // getmaxyx (stdscr, y, x);
  // my_menu_win = newwin (y, x, 4, 4);
  keypad (my_menu_win, TRUE);

  /* Set main window and sub window */
  set_menu_win (my_menu, my_menu_win);
  set_menu_sub (my_menu, derwin (my_menu_win, 6, 38, 3, 1));
  set_menu_format (my_menu, 5, 1);

  /* Set menu mark to the string " > " */
  set_menu_mark (my_menu, " > ");

  /* Print a border around the main window and print a title */
  box (my_menu_win, 0, 0);
  print_in_middle (my_menu_win, 1, 0, 40, "Recommendations");
  mvwaddch (my_menu_win, 2, 0, ACS_LTEE);
  mvwhline (my_menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch (my_menu_win, 2, 39, ACS_RTEE);

  /* Post the menu */
  post_menu (my_menu);
  wrefresh (my_menu_win);

  mvprintw (0 + 1,
            (COLS - strlen ("Use PageUp and PageDown to scoll down or up"))
                / 2,
            "Use PageUp and PageDown to scoll down or up");
  // mvprintw (LINES - 1, 0, "Arrow Keys to navigate (F1 to Exit)");
  refresh ();

  while ((c = wgetch (my_menu_win)) != KEY_BACKSPACE)
    {
      switch (c)
        {
        case KEY_DOWN:
          menu_driver (my_menu, REQ_DOWN_ITEM);
          break;
        case KEY_UP:
          menu_driver (my_menu, REQ_UP_ITEM);
          break;
        case KEY_NPAGE:
          menu_driver (my_menu, REQ_SCR_DPAGE);
          break;
        case KEY_PPAGE:
          menu_driver (my_menu, REQ_SCR_UPAGE);
          break;
        case KEY_CR:
          ITEM *cur;
          // Function pointer needs a signature here too
          //   void (*f) (WINDOW *, MENU *, ITEM **, ITEM *);

          cur = current_item (my_menu);
          //   f = item_userptr (cur);
          //   f (menu_win, menu, menu_items, cur);
          //   pos_menu_cursor (menu);
          move (0, 0);
          clrtoeol ();
          mvprintw (0, 0, "%s", item_name (cur));
          refresh ();
          break;
        }
      wrefresh (my_menu_win);
    }

  /* Unpost and free all the memory taken up */
  unpost_menu (my_menu);
  free_menu (my_menu);
  for (int j = 0; j < i; ++j)
    free_item (my_items[j]);
  // endwin ();
  free (trips);
  wclear (stdscr);
  delwin (my_menu_win);
}
