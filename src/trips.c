#include "trips.h"
#include "database.h"

void
add_trip (int64_t rowid)
{
  int y, x;
  getmaxyx (stdscr, y, x);
  WINDOW *win = newwin (y, x, 0, 0);
  keypad (win, TRUE);
  cbreak ();
  if (rowid == INT64_MIN)
    {
      mvwprintw (win, y / 2,
                 (x - strlen ("You'll need to login to add trips.")) / 2,
                 "You'll need to login to add trips.");
      wrefresh (win);
    }
  else
    {

      TRIP t;
      // TODO: fix static sizing
      char destination[64];
      char cost[64];
      char days[64];

      curs_set (1);
      echo ();

      int left_justify_x = (x - strlen ("Enter Destination: ")) / 2;

      mvwprintw (win, y / 2, left_justify_x, "Enter Destination: ");
      wgetstr (win, destination);

      mvwprintw (win, (y / 2) + 1, left_justify_x, "Enter Cost: ");
      wgetstr (win, cost);

      mvwprintw (win, (y / 2) + 2, left_justify_x, "Enter Days: ");
      wgetstr (win, days);

      noecho ();
      curs_set (0);

      t.destination = (char *)destination;
      t.cost = (char *)cost;
      t.days = (char *)days;
      if (insert_trip (&t, rowid))
        {
          wclear (win);
          mvwprintw (win, y / 2,
                     (x - strlen ("We've saved the trip for you.")) / 2,
                     "We've saved the trip for you.");
        }
      else
        {
          wclear (win);
          mvwprintw (win, y / 2,
                     (x - strlen ("We're unable to save the trip for you."))
                         / 2,
                     "We're unabel to save the trip for you.");
        }
      wrefresh (win);
    }

  int ch;
  while ((ch = wgetch (win)) != KEY_BACKSPACE)
    {
    }

  wclear (stdscr);
  delwin (win);
}
