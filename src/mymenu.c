#include "mymenu.h"
#include "auth.h"
#include "recommendations.h"
#include "util.h"
#include <menu.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#define QUIT_ON 1
#define QUIT_OFF 0

char *choices[] = {
  "Login",        "Register", "Trip Recommendations", "Add Trip",
  "Trip Summary", "Quit",     (char *)NULL,
};

int QUIT = QUIT_OFF;
int USER_ID = -1;

void
mount_main_menu ()
{
  QUIT = QUIT_OFF;
  WINDOW *menu_win;
  MENU *menu;
  ITEM **menu_items;

  int c, n_choices;

  /* Create items */
  n_choices = ARRAY_SIZE (choices);
  menu_items = (ITEM **)calloc (n_choices, sizeof (ITEM *));
  for (int i = 0; i < n_choices; ++i)
    {
      menu_items[i] = new_item (choices[i], (const char *)NULL);
      set_item_userptr (menu_items[i], func);
    }
  /* Crate menu */
  menu = new_menu ((ITEM **)menu_items);

  /* Create the window to be associated with the menu */
  menu_win = newwin (10, 40, (LINES - 10) / 2, (COLS - 40) / 2);
  keypad (menu_win, TRUE);

  /* Set main window and sub window */
  set_menu_win (menu, menu_win);
  set_menu_sub (menu, derwin (menu_win, 6, 38, 3, 1));

  /* Set menu mark to the string " > " */
  set_menu_mark (menu, " > ");

  /* Print a border around the main window and print a title */
  box (menu_win, 0, 0);
  print_in_middle (menu_win, 1, 0, 40, "Welcome to Travlan");
  mvwaddch (menu_win, 2, 0, ACS_LTEE);
  mvwhline (menu_win, 2, 1, ACS_HLINE, 38);
  mvwaddch (menu_win, 2, 39, ACS_RTEE);

  const char const *help[]
      = { "Press Up or Down arrow keys to navigate the menu",
          "Press <ENTER> to select an option",
          "Press <BACSPACE> to return back to this menu" };

  /*
   * This might look ugly and daunting
   * but all we are really doing is alinging strings
   * on the horizontal center of the stdscr
   * whilst keeping them left justified
   * instead of centering each one
   * For example,
   *              AaBbCc
   *                Bb
   * We don't want the help to look like above, but instead like down below:
   *
   *              AaBbCc
   *              Bb
   */
  int stdscr_y_max, stdscr_x_max;
  getmaxyx (stdscr, stdscr_y_max, stdscr_x_max);
  unsigned int left_justify_x
      = ((stdscr_x_max - strlen (help[0])) / 2)
                > ((stdscr_x_max - strlen (help[1])) / 2)
            ? (((stdscr_x_max - strlen (help[0])) / 2)
                       > ((stdscr_x_max - strlen (help[2])) / 2)
                   ? (stdscr_x_max - strlen (help[0])) / 2
                   : (stdscr_x_max - strlen (help[2])) / 2)
            : (stdscr_x_max - strlen (help[1])) / 2;

  for (size_t i = 0; i < ARRAY_SIZE (help); i++)
    {
      mvaddstr (i + 1, left_justify_x, help[i]);
    }

  /* Post the menu */
  post_menu (menu);
  /* Paint help to stdscr */
  refresh ();
  wrefresh (menu_win);

  set_escdelay (0);
  /* ASCII 27 for Esc */
  // while ((c = wgetch (menu_win)) != KEY_ESC || QUIT == 0)
  while (QUIT != QUIT_ON)
    {
      c = wgetch (menu_win);
      switch (c)
        {
        case KEY_DOWN:
          menu_driver (menu, REQ_DOWN_ITEM);
          break;
        case KEY_UP:
          menu_driver (menu, REQ_UP_ITEM);
          break;
        /* ASCII 10 for Enter */
        case KEY_CR:
          {
            ITEM *cur;
            // Function pointer needs a signature here too
            void (*f) (WINDOW *, MENU *, ITEM **, ITEM *);

            cur = current_item (menu);
            f = item_userptr (cur);
            f (menu_win, menu, menu_items, cur);
            pos_menu_cursor (menu);
          }
          break;
        default:
          continue;
        }
      wrefresh (menu_win);
    }
  unpost_menu (menu);
  free_menu (menu);
  for (int i = 0; i < item_count (menu); ++i)
    free_item (menu_items[i]);
  endwin ();
}

void
func (WINDOW *win, MENU *menu, ITEM **menu_items, ITEM *item)
{
  QUIT = QUIT_ON;
  switch (item_index (item))
    {
    case 0:
      // LOGIN
      {
        // unmount_main_menu (win, menu, menu_items);
        // QUIT = QUIT_ON;
        wclear (stdscr);
        wclear (stdscr);
        get_credentials_prompt (0);
        mount_main_menu ();
      }
      break;
    case 1:
      // unmount_main_menu (win, menu, menu_items);
      // QUIT = QUIT_ON;
      wclear (stdscr);
      get_credentials_prompt (1);
      mount_main_menu ();
      break;
    case 2:
      // unmount_main_menu (win, menu, menu_items);
      // QUIT = QUIT_OFF;
      wclear (stdscr);
      recommendations_menu ();
      mount_main_menu ();
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      QUIT = QUIT_ON;
      // unmount_main_menu (win, menu, menu_items);
      break;
    default:
      break;
    }
}

// void
// unmount_main_menu (WINDOW *win, MENU *menu, ITEM **menu_items)
// {
//   /* Unpost and free all the memory taken up */
//   unpost_menu (menu);
//   // refresh ();
//   // wrefresh (win);
//   for (int i = 0; i < item_count (menu); ++i)
//     free_item (menu_items[i]);
//   free_menu (menu);
//   // wclear (win);
//   delwin (win);
//   // clear ();
//   endwin ();
// }

// TODO: remove
void
unmount_main_menu (WINDOW *win, MENU *menu, ITEM **menu_items)
{
  // wrefresh (win);
  // unpost_menu (menu);
  // free_menu (menu);
  // for (int i = 0; i < item_count (menu); ++i)
  //   free_item (menu_items[i]);
  // endwin ();
  unpost_menu (menu);
  free_menu (menu);
  for (int i = 0; i < item_count (menu); ++i)
    free_item (menu_items[i]);
  endwin ();
}
