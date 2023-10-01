#include "database.h"

bool
init_database (Database *db)
{
  int rc = sqlite3_open ("travlan.db", &(db->db));
  if (rc)
    {
      // Print error message directly to ncurses window
      mvprintw (0, 0, "Can't open database: %s\n", sqlite3_errmsg (db->db));
      return false;
    }
  return true;
}

bool
create_users_table (Database *db)
{
  char *sql = "CREATE TABLE IF NOT EXISTS users ("
              "id INTEGER PRIMARY KEY AUTOINCREMENT,"
              "username TEXT NOT NULL,"
              "password TEXT NOT NULL);";

  int rc = sqlite3_exec (db->db, sql, 0, 0, 0);
  if (rc != SQLITE_OK)
    {
      // Print error message directly to ncurses window
      mvprintw (0, 0, "SQL error: %s\n", sqlite3_errmsg (db->db));
      return false;
    }
  return true;
}

bool
insert_user (Database *db, const char *username, const char *hashed_password)
{
  char *sql = "INSERT INTO users (username, password) VALUES (?, ?);";
  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2 (db->db, sql, -1, &stmt, 0);

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
close_database (Database *db)
{
  int rc = sqlite3_close (db->db);
  if (rc != SQLITE_OK)
    {
      // Print error message directly to ncurses window
      mvprintw (0, 0, "Error: Failed to close database\n");
      return false;
    }
  return true;
}
