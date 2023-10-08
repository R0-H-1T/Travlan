#ifndef TRIP_H
#define TRIP_H
#include <ncurses.h>
#include <stdint.h>
#include <string.h>

typedef struct TRIP
{
  char *destination;
  char *days;
  char *cost;
} TRIP;

void add_trip (int64_t);

#endif
