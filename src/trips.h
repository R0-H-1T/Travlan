#ifndef TRIP_H
#define TRIP_H
#include <stdint.h>

typedef struct TRIP
{
  const char *destination;
  const char *cost;
  const char *days;
} TRIP;

void summarize (int64_t rowid);

#endif
