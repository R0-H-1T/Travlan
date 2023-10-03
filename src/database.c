#include "database.h"

bool
// init_database (sqlite3 *db)
init_database ()
{
  sqlite3 *db;
  // int rc = sqlite3_open ("travlan.db", &(db->db));
  int rc = sqlite3_open ("travlan.db", &db);
  if (rc)
    {
      // Print error message directly to ncurses window
      // mvprintw (0, 0, "Can't open database: %s\n", sqlite3_errmsg (db->db));
      mvprintw (0, 0, "Can't open database: %s\n", sqlite3_errmsg (db));
      return false;
    }
  else
    {
      create_users_table (db);
      close_database (db);
    }

  return true;
}

bool
create_users_table (sqlite3 *db)
{
  char *sql = "CREATE TABLE IF NOT EXISTS users ("
              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
              "username TEXT NOT NULL,"
              "password TEXT NOT NULL,"
              "salt TEXT NOT NULL);";

  // int rc = sqlite3_exec (db->db, sql, 0, 0, 0);
  int rc = sqlite3_exec (db, sql, 0, 0, 0);
  if (rc != SQLITE_OK)
    {
      // Print error message directly to ncurses window
      // mvprintw (0, 0, "SQL error: %s\n", sqlite3_errmsg (db->db));
      mvprintw (0, 0, "SQL error: %s\n", sqlite3_errmsg (db));
      return false;
    }
  return true;
}

bool
insert_user (sqlite3 *db, const char *username, const char *hashed_password,
             const char *salt)
{
  char *sql = "INSERT INTO users (username, password, salt) VALUES (?, ?, ?);";
  sqlite3_stmt *stmt;
  // int rc = sqlite3_prepare_v2 (db->db, sql, -1, &stmt, 0);
  int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, 0);

  if (rc != SQLITE_OK)
    {
      // Print error message directly to ncurses window
      mvprintw (0, 0, "Error: Failed to prepare statement\n");
      return false;
    }

  // Rest of the code...

  return true;
}

bool
close_database (sqlite3 *db)
{
  // int rc = sqlite3_close (db->db);
  int rc = sqlite3_close (db);
  if (rc == SQLITE_BUSY)
    {
      mvprintw (0, 0, "Error: Failed to close database\n");
      return false;
    }
  if (rc != SQLITE_OK)
    {
      // Print error message directly to ncurses window
      mvprintw (1, 0, "Error: Failed to close database\n");
      return false;
    }
  return true;
}
