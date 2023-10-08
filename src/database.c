#include "database.h"
// #include "trips.h"
// #include <csv.c>
#include "trips.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool
init_database ()
{
  sqlite3 *db;
  int rc = sqlite3_open ("travlan.db", &db);
  if (rc)
    {
      // Print error message directly to ncurses window
      mvprintw (0, 0, "Can't open database: %s\n", sqlite3_errmsg (db));
      close_database (db);
      return false;
    }
  else
    {
      create_users_table (db);
      create_trips_table (db);
    }

  close_database (db);
  return rc;
}

bool
create_trips_table (sqlite3 *db)
{
  char *sql = "CREATE TABLE IF NOT EXISTS trips ("
              "id INTEGER PRIMARY KEY,"
              "user_id INTEGER NOT NULL,"
              "destination TEXT NOT NULL,"
              "cost INTEGER NOT NULL,"
              "days INTEGER NOT NULL,"
              "FOREIGN KEY (user_id)"
              "REFERENCES users(id));";

  int rc = sqlite3_exec (db, sql, 0, 0, 0);
  if (rc != SQLITE_OK)
    {
      mvprintw (0, 0, "SQL error: %s\n", sqlite3_errmsg (db));
      return false;
    }
  return true;
}

bool
create_users_table (sqlite3 *db)
{
  char *sql = "CREATE TABLE IF NOT EXISTS users ("
              "id INTEGER PRIMARY KEY,"
              "username TEXT NOT NULL,"
              "password TEXT NOT NULL,"
              "salt TEXT NOT NULL);";

  int rc = sqlite3_exec (db, sql, 0, 0, 0);
  if (rc != SQLITE_OK)
    {
      // Print error message directly to ncurses window
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

  int rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, 0);

  if (rc != SQLITE_OK)
    {
      // Print error message directly to ncurses window
      mvprintw (0, 0, "Error: Failed to prepare statement\n");
      return false;
    }

  return true;
}

bool
insert_trip (TRIP *t, int64_t rowid)
{

  sqlite3 *db;
  sqlite3_open ("travlan.db", &db);

  if (sqlite3_exec (db, SQL3_PRAGMA_FK, 0, 0, 0) != SQLITE_OK)
    {
      sqlite3_close (db);
      return false;
    }

  int64_t days = atoi (t->days);
  int64_t cost = atoi (t->cost);

  const char *sql = "INSERT INTO trips (user_id, destination, cost, days) "
                    "VALUES (?, ?, ?, ?);";
  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2 (db, sql, -1, &stmt, 0) != SQLITE_OK)
    {
      // printf("Error: Failed to prepare statement");
      sqlite3_close (db);
      return false;
    }

  if (sqlite3_bind_int64 (stmt, 1, rowid) != SQLITE_OK)
    {
      // printf("Error: Failed to bind parameter");
      sqlite3_finalize (stmt);
      sqlite3_close (db);
      return false;
    }

  if (sqlite3_bind_text (stmt, 2, t->destination, strlen (t->destination),
                         SQLITE_STATIC)
      != SQLITE_OK)
    {
      // printf("Error: Failed to bind parameter");
      sqlite3_finalize (stmt);
      sqlite3_close (db);
      return false;
    }

  if (sqlite3_bind_int64 (stmt, 3, cost) != SQLITE_OK)
    {
      // printf("Error: Failed to bind parameter");
      sqlite3_finalize (stmt);
      sqlite3_close (db);
      return false;
    }

  if (sqlite3_bind_int64 (stmt, 4, days) != SQLITE_OK)
    {
      // printf("Error: Failed to bind parameter");
      sqlite3_finalize (stmt);
      sqlite3_close (db);
      return false;
    }

  if (sqlite3_step (stmt) != SQLITE_DONE)
    {
      // printf("Error: Execution failed");
      sqlite3_finalize (stmt);
      sqlite3_close (db);
      return false;
    }

  // printf(" Trips data inserted into database");
  sqlite3_finalize (stmt);
  sqlite3_close (db);

  return true;
}

bool
close_database (sqlite3 *db)
{
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

int
read_trips_count (int64_t rowid)
{
  int rows = -1;
  sqlite3 *db;
  int rc = sqlite3_open (DB, &db);
  if (rc == SQLITE_OK)
    {
      char *sql = "SELECT COUNT(*) FROM trips WHERE user_id = ?;";
      sqlite3_stmt *stmt;

      rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, 0);
      if (rc != SQLITE_OK)
        {
          // err
          printf ("prep err");
        }
      else
        {
          rc = sqlite3_bind_int64 (stmt, 1, rowid);
          if (rc != SQLITE_OK)
            {
              // err
              printf ("bind err");
            }
          else
            {

              rc = sqlite3_step (stmt);
              // int i = 0;
              // while (rc == SQLITE_ROW && rc != SQLITE_DONE)
              //   {
              //     i++;
              //     rc = sqlite3_step (stmt);
              //   }
              // if (i == 0)
              //   {
              //     printf ("step err");
              //   }

              // printf ("***%d****", i);

              if (rc == SQLITE_ROW)
                {
                  //     t.destination = sqlite3_column_text (stmt, 2);
                  //     t.cost = sqlite3_column_text (stmt, 3);
                  //     t.days = sqlite3_column_text (stmt, 4);
                  //     printf ("%s $%s %s days", t.destination, t.cost,
                  // t.days);
                  // printf ("%s", sqlite3_column_text (stmt, 0));
                  // rows = atoi (sqlite3_column_text (stmt, 0));
                  rows = sqlite3_column_int (stmt, 0);
                  // printf ("ROWS: %d", sqlite3_column_int (stmt, 0));
                }
              else
                {
                  // err
                  printf ("read trips count step err");
                }
            }
        }
      sqlite3_finalize (stmt);
    }
  else
    {
      // err
      printf ("open err");
    }
  sqlite3_close (db);
  return rows;
}

TRIP *
read_trips (int64_t rowid)
{
  int rows = read_trips_count (rowid);
  sqlite3 *db;
  int rc = sqlite3_open (DB, &db);
  if (rc == SQLITE_OK)
    {
      char *sql = "SELECT * FROM trips WHERE user_id = ?;";
      sqlite3_stmt *stmt;

      rc = sqlite3_prepare_v2 (db, sql, -1, &stmt, 0);
      if (rc != SQLITE_OK)
        {
          // err
          printf ("prep err");
        }
      else
        {
          rc = sqlite3_bind_int64 (stmt, 1, rowid);
          if (rc != SQLITE_OK)
            {
              // err
              printf ("bind err");
            }
          else
            {
              // TRIP *t;
              // int i = 0;
              // int read_trips_count (rowid);
              TRIP *trips = (TRIP *)calloc (rows + 1, sizeof (TRIP));
              // TRIP *updated_trips;
              rc = sqlite3_step (stmt);
              int i = 0;
              // mvprintw (1, 0, "%p", trips);

              while (rc == SQLITE_ROW)
                {
                  trips[i].destination = sqlite3_column_text (stmt, 2);
                  trips[i].cost = sqlite3_column_text (stmt, 3);
                  trips[i].days = sqlite3_column_text (stmt, 4);
                  clrtoeol ();
                  mvprintw (0, 0, "%s", trips[i].destination);
                  refresh ();
                  rc = sqlite3_step (stmt);
                  i++;
                }

              // for (; i < rows && rc != SQLITE_DONE && rc == SQLITE_ROW; i++)
              //   {

              //     trips[i].destination = sqlite3_column_text (stmt, 2);
              //     trips[i].cost = sqlite3_column_text (stmt, 3);
              //     trips[i].days = sqlite3_column_text (stmt, 4);
              //     // printf ("\n%s %s %s\n", trips[i].destination,
              //     // trips[i].cost, trips[i].days);
              //     refresh ();
              //     rc = sqlite3_step (stmt);
              //   }
              // mvprintw (2, 0, "%p", trips);
              trips[i].destination = (const char *)NULL;
              trips[i].cost = (const char *)NULL;
              trips[i].days = (const char *)NULL;

              // while (rc == SQLITE_ROW && rc != SQLITE_DONE)
              //   {
              //     trips[i].destination = sqlite3_column_text (stmt, 2);
              //     trips[i].cost = sqlite3_column_text (stmt, 3);
              //     trips[i].days = sqlite3_column_text (stmt, 4);
              //     /* printf ("%s%s%s\n", trips[i].destination,
              //     trips[i].cost,
              //             trips[i].days); */
              //     // printf ("from db: %s", sqlite3_column_text (stmt, 3));
              //     // printf ("%s\t", trips[i].destination);
              //     // printf ("\n%s\t", trips[i].cost);
              //     // printf ("%s\n", trips[i].days);
              //     printf ("hello");

              //     i++;
              //     updated_trips = realloc (trips, (i + 1) * sizeof (TRIP));
              //     trips = updated_trips;
              //     rc = sqlite3_step (stmt);
              //   }
              // printf ("%s", trips[0].destination);
              // free (trips);
              if (i == 0)
                {
                  printf ("read trips step err for rowid of %ld", rowid);
                }

              // printf ("***%d****", i);

              // if (rc == SQLITE_ROW)
              //   {
              //     t.destination = sqlite3_column_text (stmt, 2);
              //     t.cost = sqlite3_column_text (stmt, 3);
              //     t.days = sqlite3_column_text (stmt, 4);
              //     printf ("%s $%s %s days", t.destination, t.cost, t.days);
              //   }
              // else
              //   {
              //     // err
              //     printf ("step err");
              //   }
              return trips;
            }
        }
      sqlite3_finalize (stmt);
    }
  else
    {
      // err
      printf ("open err");
    }
  sqlite3_close (db);
}
