#ifndef DATABASE_H
#define DATABASE_H

#include "trips.h"
#include <ncurses.h>
#include <sqlite3.h>
#include <stdbool.h>

#define DB "travlan.db"
#ifndef SQL3_PRAGMA_FK
#define SQL3_PRAGMA_FK "PRAGMA foreign_keys=1;"
#endif

bool init_database ();
bool create_trips_table (sqlite3 *db);
bool create_users_table (sqlite3 *db);
bool insert_user (sqlite3 *db, const char *username,
                  const char *hashed_password, const char *salt);
int read_trips_count (int64_t);
TRIP *read_trips (int64_t);
bool close_database (sqlite3 *db);

#endif
