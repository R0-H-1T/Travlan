#ifndef _MENU_H
#define _MENU_H

#include <menu.h>
#include <ncurses.h>

// Function declarations
void mount_main_menu ();
void unmount_main_menu (MENU *menu, ITEM **menu_items);
void func (MENU *menu, ITEM **menu_items, ITEM *item);
void print_in_middle (WINDOW *win, int starty, int startx, int width,
                      char *string);
#endif // _MENU_H
