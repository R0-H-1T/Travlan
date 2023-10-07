#ifndef UTIL_H
#define UTIL_H

#include <ncurses.h>

#define KEY_ESC 27
#define KEY_CR 10
#define CTRLD 4
#define ARRAY_SIZE(a) (sizeof (a) / sizeof (a[0]))

void print_in_middle (WINDOW *win, int starty, int startx, int width,
                      char *string);

#endif // UTIL_H
