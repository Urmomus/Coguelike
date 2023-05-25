#include "view.h"
#include <ncurses.h>

void print_unit(Unit *unit)
{
    initscr();
    printw("авыкто?\n%s", unit->name);
    getch();
}
