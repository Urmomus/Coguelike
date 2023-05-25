#include <ncurses.h>
#include "renewscreen.h"

int renew_screen(int do_change)
{

    if (do_change == 1)
    {
        refresh();
        return 1;
    }
    else
        return 0;

}