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
  noecho ();
  ITEM **my_items;
  int c;
  MENU *my_menu;
  WINDOW *my_menu_win;

  // TODO
  TRIP *trips = load_recommendations_from_csv ();
  TRIP *temp = trips;

  int i = 0;
  int width = 0;
  while (temp->destination != NULL && temp->cost != NULL && temp->days != NULL)
    {
      i++;
      temp++;
      // TODO fix i
      if (i < 135)
        width = strlen (temp->cost) + 1 > width
                    ? strlen (temp->cost) + 1
                    : width; // for $ sign infront of price
    }

  temp = trips;
  my_items = (ITEM **)calloc (i, sizeof (ITEM *));
  // reassigning it to trips
  // as temp has moved to the end of the memory block
  i = 0;
  int c_width;
  while (temp->destination != NULL && temp->cost != NULL && temp->days != NULL)
    {

      int buffer_size = strlen ((temp->days)) + strlen ((temp->cost));
      char *buffer = (char *)calloc (buffer_size * 10, sizeof (char));

      if (strlen (temp->cost) + 1 == width)
        {
          c_width = width;
        }
      else
        {
          c_width = width + 1;
        }

      sprintf (buffer, "$%s%*s days", temp->cost, c_width, temp->days);
      // TODO:  fix i trips are counted wrong
      my_items[i]
          = new_item ((const char *)temp->destination, (const char *)buffer);
      if (i < 135)
        i++;
      temp++;
    }

  /* Crate menu */
  my_menu = new_menu ((ITEM **)my_items);

  /* Create the window to be associated with the menu */
  // my_menu_win = newwin (10, 40, 4, 4);
  my_menu_win = newwin (LINES - 4, COLS, 4, 0);
  // int y, x;
  // getmaxyx (stdscr, y, x);
  // my_menu_win = newwin (y, x, 4, 4);
  keypad (my_menu_win, TRUE);

  /* Set main window and sub window */
  set_menu_win (my_menu, my_menu_win);

  set_menu_sub (my_menu, derwin (my_menu_win, LINES - 4 - 4, COLS - 2, 3, 1));
  set_menu_format (my_menu, LINES - 4 - 4, 0);

  /* Set menu mark to the string " > " */
  set_menu_mark (my_menu, " > ");

  /* Print a border around the main wi ndow and print a title */
  box (my_menu_win, 0, 0);
  print_in_middle (my_menu_win, 1, 0, COLS, "Recommendations");
  mvwaddch (my_menu_win, 2, 0, ACS_LTEE);
  mvwhline (my_menu_win, 2, 1, ACS_HLINE, COLS - 2);
  mvwaddch (my_menu_win, 2, COLS - 1, ACS_RTEE);

  /* Post the menu */
  post_menu (my_menu);
  wrefresh (my_menu_win);

  int help_alignment
      = strlen ("Use PageUp and PageDown to scoll down or up a page")
                > strlen ("Use Up or Down arrow keys to navigate the menu")
            ? strlen ("Use PageUp and PageDown to scoll down or up a page")
            : strlen ("Use PageUp and PageDown to scoll down or up a page");
  strlen ("Use Up or Down arrow keys to navigate the menu");

  mvprintw (0 + 1, help_alignment,
            "Use Up or Down arrow keys to navigate the menu");
  mvprintw (0 + 2, help_alignment,
            "Use PageUp and PageDown to scoll down or up a page");
  /* mvprintw (0 + 1,
            (COLS - strlen ("Use PageUp and PageDown to scoll down or up"))
                / 2,
            "Use PageUp and PageDown to scoll down or up");
 */
  /* print_in_middle (stdscr, 0 + 2, 0, getmaxx (stdscr),
                   "Use Up or Down arrow keys to navigate the menu"); */
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
          pos_menu_cursor (my_menu);
          move (0, 0);
          clrtoeol ();
          int i = item_index (cur);
          mvprintw (0, 0, "Destination: %s, Days: %s, Cost: %s",
                    trips[i].destination, trips[i].days, trips[i].cost);
          // menu_items ();
          refresh ();
          break;
          // item_
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
