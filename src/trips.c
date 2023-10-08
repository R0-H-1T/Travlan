#include "trips.h"
#include "database.h"
#include <stdlib.h>

void
summarize (int64_t rowid)
{
  if (rowid == INT64_MIN)
    {
      return;
    }
  TRIP *trips = read_trips (rowid);
  // printf ("trip is %p", trips);
  // read_trips_count (rowid);
  TRIP *temp = trips;
  // printf ("temp is %p", temp);
  int i = 0;
  while (temp->destination != NULL && temp->cost != NULL && temp->days != NULL)
    {
      // printf ("*");
      clrtoeol ();
      mvprintw (1, 0, "%s", temp[0].destination);
      refresh ();
      // printf ("%s $%s %s days", temp->destination, temp->cost, temp->days);
      temp++;
    }
  // free (trips);

  // printf ("%s $%s %s days", trip[0].destination, trip[0].cost,
  // trip[0].days); free (trip); for (size_t i = 0; i < 3; i++)
  //   {
  //     printf ("%s $%s %s days", trip->destination, trip->cost, trip->days);
  //     trip++;
  //   }
}
