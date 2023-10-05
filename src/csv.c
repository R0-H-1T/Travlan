#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *
 *   int main is declared below
 *   need to have a different header file for the structure of travlan
 *   display_destination returns a pointer to a travlan structure
 */

typedef struct travlan
{
    char *destination;
    char *days;
    char *cost;
} trav;

trav *display_destinations()
{
    FILE *csv_file = fopen("../data/trips.csv", "r");

    if (csv_file == NULL)
    {
        perror("Failed to open the CSV file");
        exit(0);
    }

    char line[100]; // Adjust the buffer size as needed
    int count = 0;
    while (fgets(line, sizeof(line), csv_file) != NULL)
    {
        count++;
    }
    rewind(csv_file);
    trav *travArray = (trav *)malloc((count + 1) * sizeof(trav));

    count = 0;
    while (fgets(line, sizeof(line), csv_file) != NULL)
    {
        char *token = strtok(line, ",");
        int step = 0;
        while (token != NULL)
        {
            if (step == 0)
            {
                travArray[count].destination = strdup(token);
            }
            else if (step == 1)
            {
                travArray[count].days = strdup(token);
            }
            else
            {
                travArray[count].cost = strdup(token);
            }
            step++;
            token = strtok(NULL, ",");
        }
        printf("\n");
        count++;
    }
    travArray[count].destination = (char *)NULL;
    travArray[count].days = (char *)NULL;
    travArray[count].cost = (char *)NULL;

    fclose(csv_file);

    return travArray;
}

// int main()
// {
//     trav *t = display_destinations();

//     int i = 0;
//     while (t[i].destination != NULL && t[i].days != NULL && t[i].cost != NULL)
//     {
//         printf("%s %s %s", t[i].destination, t[i].days, t[i].cost);
//         i++;
//     }

//     free(t);
//     return 0;
// }
