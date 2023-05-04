#include 'Map.h'
#include <ncurses.h>

int show_map(map)
{
    initscr();

    printw(map);

    endwin();
}

int show_menu()
{
    initscr();

    printw("Continue\n","Exit\n");
    
    endwin();
}

int show_inventor(items)
{
    intscr();

    //в таблице или списком?
    printw(items)

    endwin()
}