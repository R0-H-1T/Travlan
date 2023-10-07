#ifndef TRIP_H
#define TRIP_H

struct trip
{
  char *destination;
  unsigned int *days;
};

typedef struct trip TRIP;
#endif
