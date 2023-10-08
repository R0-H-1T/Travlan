#ifndef AUTH_H
#define AUTH_H

#include <ncurses.h>
#include <sqlite3.h>

// Function declarations
int64_t signin (const char *username, const char *password, WINDOW *win);
int signup (const char *username, const char *password, WINDOW *win);
int64_t get_credentials_prompt (int action);

#endif // AUTH_H
