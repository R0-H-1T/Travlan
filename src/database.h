#ifndef DATABASE_H
#define DATABASE_H

#include <ncurses.h>
#include <sqlite3.h>
#include <stdbool.h>

bool init_database ();
bool create_trips_table (sqlite3 *db);
bool create_users_table (sqlite3 *db);
bool insert_user (sqlite3 *db, const char *username,
                  const char *hashed_password, const char *salt);
bool close_database (sqlite3 *db);

#endif
