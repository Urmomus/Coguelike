#include <ncurses.h>

void renew_screen(int do_change)
{
    if (do_change == 1)
    {
        refresh();
    }
}