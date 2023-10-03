// #include "login.h"

#include <ncurses.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#include "bcrypt.h"

int
do_login ()
{
  char *username[16];
  char *password[BCRYPT_HASHSIZE];

  int y, x;

  getmaxyx (stdscr, y, x);

  WINDOW *win = newwin (y, x, 0, 0);
  keypad (win, TRUE);
  werase (win);
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

  // Database *db;
  // char *salt[BCRYPT_HASHSIZE];
  // char *hash[BCRYPT_HASHSIZE];
  // if (bcrypt_gensalt (0, salt) > 0)
  //   {
  //     if (bcrypt_hashpw (password, salt, hash) > 0)
  //       {
  //         if (insert_user (db, username, hash, salt))
  //           {
  //             mvwprintw (
  //                 win, y / 2 + 2,
  //                 (x - strlen ("Logged in. Go back by using <BACKSPACE>
  //                 key"))
  //                     / 2,
  //                 "Logged in.Go back by using<BACKSPACE> key");
  //           }
  //       }
  //   }

  int ch;
  while ((ch = wgetch (win)) != KEY_BACKSPACE)
    {
      /* code */
    }
  delwin (win);
  return 0; // Add error handling if needed
}

int
validate_login (sqlite3 *db, const char *username, const char *password,
                WINDOW *win)
{
  char *sql = "SELECT * FROM users WHERE username = ?;";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, 0);

  if (rc != SQLITE_OK)
    {
      mvwprintw (win, 0, 0, "Error: Failed to prepare statement");
      wrefresh (win);
      return -1;
    }

  rc = sqlite3_bind_text (stmt, 1, username, strlen (username), SQLITE_STATIC);

  if (rc != SQLITE_OK)
    {
      mvwprintw (win, 0, 0, "Error: Failed to bind parameter");
      wrefresh (win);
      sqlite3_finalize (stmt);
      return -1;
    }

  rc = sqlite3_step (stmt);

  if (rc == SQLITE_ROW)
    {
      const char *hashed_password
          = (const char *)sqlite3_column_text (stmt, 1);

      if (bcrypt_checkpw (password, hashed_password))
        {
          sqlite3_finalize (stmt);
          return 1; // Login successful
        }
    }

  mvwprintw (win, 0, 0, "Invalid username or password");
  wrefresh (win);
  sqlite3_finalize (stmt);
  return 0; // Incorrect username or password
}

int
register_user (sqlite3 *db, const char *username, const char *password,
               WINDOW *win)
{
  char *hashed_password;
  char *salt;

  if (bcrypt_gensalt (8, salt) != 0
      && bcrypt_hashpw (password, salt, hashed_password) != 0)
    {
      mvwprintw (win, 0, 0, "Error: Failed to hash password");
      wrefresh (win);
      return -1;
    }

  char *sql = "INSERT INTO users (username, password) VALUES (?, ?);";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, 0);

  if (rc != SQLITE_OK)
    {
      mvwprintw (win, 0, 0, "Error: Failed to prepare statement");
      wrefresh (win);
      free (hashed_password);
      return -1;
    }

  rc = sqlite3_bind_text (stmt, 1, username, strlen (username), SQLITE_STATIC);
  if (rc != SQLITE_OK)
    {
      mvwprintw (win, 0, 0, "Error: Failed to bind parameter");
      wrefresh (win);
      free (hashed_password);
      sqlite3_finalize (stmt);
      return -1;
    }

  rc = sqlite3_bind_text (stmt, 2, hashed_password, strlen (hashed_password),
                          SQLITE_STATIC);
  if (rc != SQLITE_OK)
    {
      mvwprintw (win, 0, 0, "Error: Failed to bind parameter");
      wrefresh (win);
      sqlite3_finalize (stmt);
      return -1;
    }

  rc = sqlite3_step (stmt);
  if (rc != SQLITE_DONE)
    {
      mvwprintw (win, 0, 0, "Error: Execution failed");
      wrefresh (win);
      sqlite3_finalize (stmt);
      return -1;
    }

  mvwprintw (win, 0, 0, "User registered successfully");
  wrefresh (win);
  sqlite3_finalize (stmt);
  return 0;
}
