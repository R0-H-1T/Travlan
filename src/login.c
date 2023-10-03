#include "login.h"

#include <ncurses.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#include "bcrypt.h"

int
do_login (int action)
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

  mvwprintw (win, y / 2, (x - strlen ("Enter Username: ")) / 2,
             "Enter Username: ");

  echo ();
  wgetstr (win, username);
  noecho ();

  mvwprintw (win, (y / 2) + 1, (x - strlen ("Enter Password: ")) / 2,
             "Enter Password: ");

  // don't emit password to console
  // echo ();
  wgetstr (win, password);
  // noecho ();
  mvwprintw (win, 3, 0, "%s", password);
  wrefresh (win);

  action == 0 ? validate_login (username, password, stdscr)
              : register_user (username, password, stdscr);
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
validate_login (const char *username, const char *password, WINDOW *win)
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
              mvprintw (0, 0, "LOGIN SUCCESSFUL");
              wrefresh (win);
              sqlite3_finalize (stmt);
              sqlite3_close (db);
              return 1; // Login successful
            }
          else
            {
              mvwprintw (win, 0, 0, "Invalid username or password");
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
register_user (const char *username, const char *password, WINDOW *win)
{
  char hashed_password[BCRYPT_HASHSIZE];
  char salt[BCRYPT_HASHSIZE];

  if (bcrypt_gensalt (0, salt) != 0
      && bcrypt_hashpw (password, salt, hashed_password) != 0)
    {
      mvwprintw (win, 0, 0, "Error: Failed to hash password");
      wrefresh (win);
      return -1;
    }
  // todo
  mvwprintw (win, 0, 0, "hash: %s", hashed_password);
  mvwprintw (win, 1, 0, "salt: %s", salt);
  wrefresh (win);

  sqlite3 *db;
  sqlite3_open ("travlan.db", &db);

  char *sql = "INSERT INTO users (username, password, salt) VALUES (?, ?, ?);";
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

  rc = sqlite3_bind_text (stmt, 1, username, strlen (username), SQLITE_STATIC);
  if (rc != SQLITE_OK)
    {
      mvwprintw (win, 0, 0, "Error: Failed to bind parameter");
      wrefresh (win);
      // free (hashed_password);
      sqlite3_finalize (stmt);
      sqlite3_close (db);
      return -1;
    }

  rc = sqlite3_bind_text (stmt, 2, hashed_password, strlen (hashed_password),
                          SQLITE_STATIC);
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

  mvwprintw (win, LINES - 1, 0, "User registered successfully");
  wrefresh (win);
  sqlite3_finalize (stmt);
  sqlite3_close (db);
  return 0;
}
