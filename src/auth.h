#ifndef AUTH_H
#define AUTH_H

#include <ncurses.h>
#include <sqlite3.h>

// Function declarations
int signin (const char *username, const char *password, WINDOW *win);
int signup (const char *username, const char *password, WINDOW *win);
int get_credentials_prompt (int action);

#endif // AUTH_H
