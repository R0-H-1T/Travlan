#ifndef LOGIN_H
#define LOGIN_H

#include <ncurses.h>
#include <sqlite3.h>

// Function declarations
int validate_login(sqlite3 *db, const char *username, const char *password,
                   WINDOW *win);
int register_user(sqlite3 *db, const char *username, const char *password,
                  WINDOW *win);
int get_login_credentials(WINDOW *win, char *username, char *password);
int get_registration_credentials(WINDOW *win, char *username, char *password);

#endif  // LOGIN_H
