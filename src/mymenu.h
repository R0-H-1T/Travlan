#ifndef MYMENU_H
#define MYMENU_H

#include <menu.h>
#include <ncurses.h>

// Function declarations
void mount_main_menu ();
void unmount_main_menu (WINDOW *win, MENU *menu, ITEM **menu_items);
void func (WINDOW *win, MENU *menu, ITEM **menu_items, ITEM *item);

#endif // MYMENU_H
