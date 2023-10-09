#include "trips.h"
#include "database.h"

void
summarize (int64_t rowid)
{

  int y;
  int x;

  clear ();
  getmaxyx (stdscr, y, x);

  // WINDOW *win = newwin (y, x, 0, 0);
  int left_justify_x = 0;
  const char *summary_text[]
      = { "Total estimated cost: %.2f", "Average cost per day: %.2f",
          "Most expensive destination: %s",
          "Destination with maximum days: %s",
          "Destinations costing above average: %d" };
  //  (const char *)NULL };
  for (int i = 0; i < ARRAY_SIZE (summary_text); i++)
    {
      if (left_justify_x < strlen (summary_text[i]))
        {
          left_justify_x = strlen (summary_text[i]);
        }
    }
  WINDOW *win = newwin (10, x / 2, (y - 10) / 2, left_justify_x);

  keypad (win, TRUE);
  cbreak ();
  curs_set (0);
  noecho ();

  if (rowid == INT64_MIN)
    {
      mvwprintw (win, y / 2,
                 (x - strlen ("You'll need to login to view trip summary."))
                     / 2,
                 "You'll need to login to view trip summary.");
      wrefresh (win);
    }
  else
    {
      TRIP *t = read_trips (rowid);
      TRIP *temp = t;

      float totalCost = 0;
      int totalDays = 0;

      float maxCost = 0;
      int maxDays = 0;

      int aboveAvgCount = 0;

      int atoi_cost;
      int atoi_days;

      char maxCostName[64];
      char maxDaysName[64];

      while (temp->destination != (const char *)NULL
             && temp->cost != (const char *)NULL
             && temp->days != (const char *)NULL)
        {

          atoi_cost = atoi (temp->cost);
          atoi_days = atoi (temp->days);
          totalCost += atoi_cost;
          totalDays += atoi_days;

          if (atoi_cost > maxCost)
            {
              maxCost = atoi_cost;
              strncpy (maxCostName, temp->destination,
                       strlen (temp->destination));
              maxCostName[strlen (temp->destination)] = '\0';
            }
          if (atoi_days > maxDays)
            {
              maxDays = atoi_days;
              strncpy (maxDaysName, temp->destination,
                       strlen (temp->destination));
              maxDaysName[strlen (temp->destination)] = '\0';
            }
          temp++;
        }

      float avgCostPerDay = totalCost / totalDays;
      temp = t;
      while (temp->destination != (const char *)NULL
             && temp->cost != (const char *)NULL
             && temp->days != (const char *)NULL)
        {
          if (atoi (temp->cost) > avgCostPerDay)
            {
              aboveAvgCount++;
            }
          temp++;
        }

      box (win, 0, 0);
      print_in_middle (win, 1, 0, x / 2, "Trip Summary");
      mvwaddch (win, 2, 0, ACS_LTEE);

      mvwhline (win, 2, 1, ACS_HLINE, x / 2 - 2);
      mvwaddch (win, 2, x / 2 - 1, ACS_RTEE);

      mvwprintw (win, 1 + 1 + 1, 1 + 1, "Total estimated cost: %.2f",
                 totalCost);
      mvwprintw (win, 2 + 1 + 1, 1 + 1, "Average cost per day: %.2f",
                 avgCostPerDay);
      mvwprintw (win, 3 + 1 + 1, 1 + 1, "Most expensive destination: %s",
                 maxCostName);
      mvwprintw (win, 4 + 1 + 1, 1 + 1, "Destination with maximum days: %s",
                 maxDaysName);
      mvwprintw (win, 5 + 1 + 1, 1 + 1,
                 "Destinations costing above average: %d", aboveAvgCount);
      wrefresh (win);
    }
  int ch;
  while ((ch = wgetch (win)) != KEY_BACKSPACE)
    {
      /* code */
    }
  wclear (stdscr);
  delwin (win);
}

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
