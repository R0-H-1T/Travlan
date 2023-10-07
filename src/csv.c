#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <cstring.h>

/*
 *
 *   int main is declared below
 *   need to have a different header file for the structure of travlan
 *   display_destination returns a pointer to a travlan structure
 */

typedef struct TRIP
{
  char *destination;
  char *days;
  char *cost;
} TRIP;

TRIP *
load_recommendations_from_csv ()
{
  FILE *csv_file = fopen ("../data/trips.csv", "r");

  if (csv_file == NULL)
    {
      return (TRIP *)NULL;
    }

  char line[100]; // Adjust the buffer size as needed
  int count = 0;
  while (fgets (line, sizeof (line), csv_file) != NULL)
    {
      count++;
    }
  rewind (csv_file);
  TRIP *travArray = (TRIP *)malloc ((count + 1) * sizeof (TRIP));
  count = 0;
  while (fgets (line, sizeof (line), csv_file) != NULL)
    {
      char *token = strtok (line, ",");
      int step = 0;
      while (token != NULL)
        {
          if (step == 0)
            {
              travArray[count].destination = strdup (token);
            }
          else if (step == 1)
            {
              travArray[count].days = strdup (token);
            }
          else
            {
              // fgets() takes the newline as this is the last column
              // get rid of it
              int upto = (int)strcspn ((const char *)token, "\r\n");
              travArray[count].cost = strndup (token, upto);
            }
          step++;
          token = strtok (NULL, ",");
        }
      count++;
    }
  travArray[count].destination = (char *)NULL;
  travArray[count].days = (char *)NULL;
  travArray[count].cost = (char *)NULL;

  fclose (csv_file);

  return travArray;
}

// int
// main ()
// {
//   TRIP *t = load_recommendations_from_csv ();

//   // int i = 0;
//   // while (t[i].destination != NULL && t[i].days != NULL && t[i].cost !=
//   // NULL)
//   //   {
//   //     printf ("%s %s %s", t[i].destination, t[i].days, t[i].cost);
//   //     printf ("%s %s %s", t[i].destination, t[i].days, t[i].cost);
//   //     i++;
//   //   }
//   TRIP *temp = t;
//   while (temp->cost != NULL)
//     {
//       printf ("%s", temp->cost);
//       temp++;
//     }
//   free (t);
//   return 0;
// }
