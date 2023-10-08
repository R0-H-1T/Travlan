#include "database.h"
// #include "trips.h"
// #include <csv.c>
#include "trips.h"
#include<string.h>
#include<stdint.h>
#include<stdlib.h>

bool init_database()
{
  sqlite3 *db;
  int rc = sqlite3_open("travlan.db", &db);
  if (rc)
  {
    // Print error message directly to ncurses window
    mvprintw(0, 0, "Can't open database: %s\n", sqlite3_errmsg(db));
    close_database(db);
    return false;
  }
  else
  {
    create_users_table(db);
    create_trips_table(db);
  }

  close_database(db);
  return rc;
}

bool create_trips_table(sqlite3 *db)
{
  char *sql = "CREATE TABLE IF NOT EXISTS trips ("
              "id INTEGER PRIMARY KEY,"
              "user_id INTEGER NOT NULL,"
              "destination TEXT NOT NULL,"
              "cost INTEGER NOT NULL,"
              "days INTEGER NOT NULL,"
              "FOREIGN KEY (user_id)"
              "REFERENCES users(id));";

  int rc = sqlite3_exec(db, sql, 0, 0, 0);
  if (rc != SQLITE_OK)
  {
    mvprintw(0, 0, "SQL error: %s\n", sqlite3_errmsg(db));
    return false;
  }
  return true;
}

bool create_users_table(sqlite3 *db)
{
  char *sql = "CREATE TABLE IF NOT EXISTS users ("
              "id INTEGER PRIMARY KEY,"
              "username TEXT NOT NULL,"
              "password TEXT NOT NULL,"
              "salt TEXT NOT NULL);";

  int rc = sqlite3_exec(db, sql, 0, 0, 0);
  if (rc != SQLITE_OK)
  {
    // Print error message directly to ncurses window
    mvprintw(0, 0, "SQL error: %s\n", sqlite3_errmsg(db));
    return false;
  }
  return true;
}

bool insert_user(sqlite3 *db, const char *username, const char *hashed_password,
                 const char *salt)
{
  char *sql = "INSERT INTO users (username, password, salt) VALUES (?, ?, ?);";
  sqlite3_stmt *stmt;

  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

  if (rc != SQLITE_OK)
  {
    // Print error message directly to ncurses window
    mvprintw(0, 0, "Error: Failed to prepare statement\n");
    return false;
  }

  return true;
}



bool insert_trip(TRIP *t, int64_t rowid)
{

  sqlite3 *db;
    sqlite3_open("travlan.db", &db);

    if (sqlite3_exec(db, SQL3_PRAGMA_FK, 0, 0, 0) != SQLITE_OK)
    {
        sqlite3_close(db);
        return false;
    }

    int64_t days = atoi(t->days);
    int64_t cost = atoi(t->cost);


    const char *sql = "INSERT INTO trips (user_id, destination, cost, days) "
                      "VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
        // printf("Error: Failed to prepare statement");
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_int64(stmt, 1, rowid) != SQLITE_OK)
    {
        // printf("Error: Failed to bind parameter");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_text(stmt, 2, t->destination, strlen(t->destination), SQLITE_STATIC) != SQLITE_OK)
    {
        // printf("Error: Failed to bind parameter");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_int64(stmt, 3, cost) != SQLITE_OK)
    {
        // printf("Error: Failed to bind parameter");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_bind_int64(stmt, 4, days) != SQLITE_OK)
    {
        // printf("Error: Failed to bind parameter");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        // printf("Error: Execution failed");
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    // printf(" Trips data inserted into database");
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return true;
}



bool close_database(sqlite3 *db)
{
  int rc = sqlite3_close(db);
  if (rc == SQLITE_BUSY)
  {
    mvprintw(0, 0, "Error: Failed to close database\n");
    return false;
  }
  if (rc != SQLITE_OK)
  {
    // Print error message directly to ncurses window
    mvprintw(1, 0, "Error: Failed to close database\n");
    return false;
  }
  return true;
}

void
test (int64_t rowid, TRIP *t)
{
  printf ("* %s\t%s\t%s", t->destination, t->days, t->cost);
  printf ("user is %ld", rowid);
}
