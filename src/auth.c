#include "auth.h"

#include <ncurses.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#include "bcrypt.h"

int
get_credentials_prompt (int action)
{
  char username[BCRYPT_HASHSIZE];
  char password[BCRYPT_HASHSIZE];

  int y, x;

  getmaxyx (stdscr, y, x);

  WINDOW *win = newwin (y, x, 0, 0);
  keypad (win, TRUE);
  // werase (win);
  // raw ();
  cbreak ();

  curs_set (1);
  mvwprintw (win, y / 2, (x - strlen ("Enter Username: ")) / 2,
             "Enter Username: ");
  // mvwprintw (win, y / 2, x / 2, "Enter Username: ");

  echo ();
  wgetstr (win, username);
  noecho ();

  mvwprintw (win, (y / 2) + 1, (x - strlen ("Enter Password: ")) / 2,
             "Enter Password: ");
  // mvwprintw (win, (y / 2) + 1, x / 2, "Enter Password: ");

  // don't emit password to console
  // echo ();
  wgetstr (win, password);
  // noecho ();
  curs_set (0);

  action == 0
      ? signin ((const char *)username, (const char *)password, stdscr)
      : signup ((const char *)username, (const char *)password, stdscr);
  int ch;
  while ((ch = wgetch (win)) != KEY_BACKSPACE)
    {
      /* code */
    }
  wclear (stdscr);
  delwin (win);
  return 0; // Add error handling if needed
}

int
signin (const char *username, const char *password, WINDOW *win)
{
  sqlite3 *db;
  int rc = sqlite3_open ("travlan.db", &db);
  if (rc == SQLITE_OK)
    {
      char *sql = "SELECT * FROM users WHERE username = ?;";
      sqlite3_stmt *stmt;
      int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, 0);

      if (rc != SQLITE_OK)
        {
          mvwprintw (win, 0, 0, "Error: Failed to prepare statement");
          wrefresh (win);
          sqlite3_close (db);
          return -1;
        }

      rc = sqlite3_bind_text (stmt, 1, username, strlen (username),
                              SQLITE_STATIC);

      if (rc != SQLITE_OK)
        {
          mvwprintw (win, 0, 0, "Error: Failed to bind parameter");
          wrefresh (win);
          sqlite3_finalize (stmt);
          sqlite3_close (db);
          return -1;
        }

      rc = sqlite3_step (stmt);

      if (rc == SQLITE_ROW)
        {
          const char *hashed_password
              = (const char *)sqlite3_column_text (stmt, 2);
          // TODO
          if (bcrypt_checkpw (password, hashed_password) == 0)
            {
              // mvprintw (getcury (win) + 2, getcurx (win),"Hello, %s. You're
              // now signed in.", username);
              int y, x;
              getmaxyx (stdscr, y, x);
              clear ();
              mvprintw (y / 2, x / 2, "Hello, %s. You're now signed in.",
                        username);

              wrefresh (win);
              sqlite3_finalize (stmt);
              sqlite3_close (db);
              return 1; // Login successful
            }
          else
            {
              int y, x;
              getmaxyx (stdscr, y, x);
              clear ();
              // mvprintw (y / 2, x / 2, "Invalid username or password!");
              mvprintw (y / 2,
                        (x - strlen ("Invalid username or password!")) / 2,
                        "Invalid username or password!");
              wrefresh (win);
            }
        }

      sqlite3_finalize (stmt);
      sqlite3_close (db);
      return 0; // Incorrect username or password
    }
  sqlite3_close (db);
  return -1;
}

int
signup (const char *username, const char *password, WINDOW *win)
{
  char salt[BCRYPT_HASHSIZE];
  char hashed_password[BCRYPT_HASHSIZE];

  const char *my_pass = hashed_password;
  if (bcrypt_gensalt (0, salt) != 0)
    {
      // err
    }
  else
    {
      if (bcrypt_hashpw (password, salt, hashed_password) != 0)
        {
          // err
        }
      else
        {
          sqlite3 *db;
          sqlite3_open ("travlan.db", &db);

          const char *sql = "INSERT INTO users (username, password, salt) "
                            "VALUES (?, ?, ?);";
          sqlite3_stmt *stmt;
          int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, 0);

          if (rc != SQLITE_OK)
            {
              mvwprintw (win, 0, 0, "Error: Failed to prepare statement");
              wrefresh (win);
              // free (hashed_password);
              sqlite3_close (db);
              return -1;
            }

          rc = sqlite3_bind_text (stmt, 1, username, strlen (username),
                                  SQLITE_STATIC);
          if (rc != SQLITE_OK)
            {
              mvwprintw (win, 0, 0, "Error: Failed to bind parameter");
              wrefresh (win);
              // free (hashed_password);
              sqlite3_finalize (stmt);
              sqlite3_close (db);
              return -1;
            }

          rc = sqlite3_bind_text (stmt, 2, hashed_password,
                                  strlen (hashed_password), SQLITE_STATIC);
          if (rc != SQLITE_OK)
            {
              mvwprintw (win, 0, 0, "Error: Failed to bind parameter");
              wrefresh (win);
              sqlite3_finalize (stmt);
              sqlite3_close (db);
              return -1;
            }

          rc = sqlite3_bind_text (stmt, 3, salt, strlen (salt), SQLITE_STATIC);
          if (rc != SQLITE_OK)
            {
              mvwprintw (win, 0, 0, "Error: Failed to bind parameter");
              wrefresh (win);
              sqlite3_finalize (stmt);
              sqlite3_close (db);
              return -1;
            }

          rc = sqlite3_step (stmt);
          if (rc != SQLITE_DONE)
            {
              mvwprintw (win, 0, 0, "Error: Execution failed");
              wrefresh (win);
              sqlite3_finalize (stmt);
              sqlite3_close (db);
              return -1;
            }

          mvwprintw (win, getcury (win), getcurx (win),
                     "Hello, %s. You're now signed up.", username);
          wrefresh (win);
          sqlite3_finalize (stmt);
          sqlite3_close (db);
        }
    }
  return 0;
}
