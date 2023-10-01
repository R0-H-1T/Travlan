#ifndef DATABASE_H
#define DATABASE_H

#include <ncurses.h>
#include <sqlite3.h>
#include <stdbool.h>

typedef struct
{
  sqlite3 *db;
} Database;

bool init_database (Database *db);
bool create_users_table (Database *db);
bool insert_user (Database *db, const char *username,
                  const char *hashed_password);
bool close_database (Database *db);

#endif
