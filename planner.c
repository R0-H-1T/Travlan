#include <stdio.h>
#include <string.h>

#define MAX_DESTINATIONS 10

int main() {
  char destinationNames[MAX_DESTINATIONS][50];
  float travelCosts[MAX_DESTINATIONS];
  int daysSpent[MAX_DESTINATIONS];

  int numDestinations = 0;
  int choice;

  do {
    printf("\nTravel Planner Menu:\n");
    printf("1. Add Destination\n");
    printf("2. Calculate Summary\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      if (numDestinations < MAX_DESTINATIONS) {
        printf("\nDestination %d:\n", numDestinations + 1);
        printf("Enter name: ");
        scanf("%s", destinationNames[numDestinations]);
        printf("Enter travel cost: ");
        scanf("%f", &travelCosts[numDestinations]);
        printf("Enter number of days: ");
        scanf("%d", &daysSpent[numDestinations]);
        numDestinations++;
      } else {
        printf("Exceeded maximum number of destinations.\n");
      }
      break;
    case 2:
      if (numDestinations > 0) {
        float totalCost = 0;
        int totalDays = 0;
        float maxCost = 0;
        int maxDays = 0;
        char maxCostName[50];
        char maxDaysName[50];
        int aboveAvgCount = 0;

        for (int i = 0; i < numDestinations; i++) {
          totalCost += travelCosts[i];
          totalDays += daysSpent[i];

          if (travelCosts[i] > maxCost) {
            maxCost = travelCosts[i];
            strcpy(maxCostName, destinationNames[i]);
          }

          if (daysSpent[i] > maxDays) {
            maxDays = daysSpent[i];
            strcpy(maxDaysName, destinationNames[i]);
          }
        }

        float avgCostPerDay = totalCost / totalDays;

        for (int i = 0; i < numDestinations; i++) {
          if (travelCosts[i] > avgCostPerDay) {
            aboveAvgCount++;
          }
        }

        printf("\nTrip Summary:\n");
        printf("Total estimated cost: %.2f\n", totalCost);
        printf("Average cost per day: %.2f\n", avgCostPerDay);
        printf("Most expensive destination: %s\n", maxCostName);
        printf("Destination with maximum days: %s\n", maxDaysName);
        printf("Destinations costing above average: %d\n", aboveAvgCount);
      } else {
        printf("No destinations added yet.\n");
      }
      break;
    case 3:
      printf("Exiting the program.\n");
      break;
    default:
      printf("Invalid choice. Please enter a valid option.\n");
    }
  } while (choice != 3);

  return 0;
}
