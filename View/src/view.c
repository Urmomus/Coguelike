#include "view.h"
#include <curses.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

char free_cell = '.';
char wall_cell = '#';
char goblin_cell = 'g';
char ghost_cell = 'G';
char slime_cell = 's';
char skeleton_cell = 'S';
char player_cell = '@';
char item_cell = 'i';
char portal_cell = '@';

int VISIBLE_INVENTORY_SIZE = 5;

void init_ncurses()
{
    initscr();
    noecho();
    curs_set(0);
    start_color();

    init_pair(PLAYER_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(GOBLIN_PAIR, COLOR_GREEN, COLOR_BLACK);
    init_pair(SLIME_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(GHOST_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(SKELETON_PAIR, COLOR_CYAN, COLOR_BLACK);
    init_pair(PORTAL_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(WALL_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(ITEM_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(FREE_PAIR, COLOR_WHITE, COLOR_BLACK);
    init_pair(SELECTED_ITEM_PAIR, COLOR_BLACK, COLOR_GREEN);
    init_pair(SHOW_HP_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SHOW_DMG_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SHOW_NAME_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(SHOW_DEFENSE_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(DEATH_SCREEN_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(WIN_SCREEN_PAIR, COLOR_YELLOW, COLOR_BLACK);
    init_pair(EQUIPPED_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(SCROLL_PAIR, COLOR_MAGENTA, COLOR_BLACK);
}

void print_map(GameMap *game_map)
{
    clear();
    char cell;
    ColorPair cell_color;

    attron(COLOR_PAIR(SHOW_NAME_PAIR));
    printw("Level: %d\n", game_map->level);
    attron(COLOR_PAIR(SHOW_NAME_PAIR));

    for (int y = 0; y < game_map->size_y; ++y)
    {
        for (int x = 0; x < game_map->size_x; ++x)
        {
            if (game_map->data[y][x].unit != NULL)
            {
                if (game_map->data[y][x].unit->unit_type == PLAYER)
                {
                    cell = player_cell;
                    cell_color = PLAYER_PAIR;
                }
                else if (game_map->data[y][x].unit->unit_type == GOBLIN)
                {
                    cell = goblin_cell;
                    cell_color = GOBLIN_PAIR;
                }
                else if (game_map->data[y][x].unit->unit_type == GHOST)
                {
                    cell = ghost_cell;
                    cell_color = GHOST_PAIR;
                }
                else if (game_map->data[y][x].unit->unit_type == SKELETON)
                {
                    cell = skeleton_cell;
                    cell_color = SKELETON_PAIR;
                }
                else if (game_map->data[y][x].unit->unit_type == SLIME)
                {
                    cell = slime_cell;
                    cell_color = SLIME_PAIR;
                }
            }
            else if (game_map->data[y][x].item != NULL)
            {
                cell = item_cell;
                cell_color = ITEM_PAIR;
            }
            else if (game_map->data[y][x].type == WALL_CELL)
            {
                cell = wall_cell;
                cell_color = WALL_PAIR;
            }
            else if (game_map->data[y][x].type == FINISH_CELL)
            {
                cell = portal_cell;
                cell_color = PORTAL_PAIR;
            }
            else
            {
                cell = free_cell;
                cell_color = FREE_PAIR;
            }

            attron(COLOR_PAIR(cell_color));
            printw("%c ", cell);
            attroff(COLOR_PAIR(cell_color));
        }
        printw("\n");
    }
}

void print_inventory(GameMap *game_map, int selected_item_index)
{
    clear(); // чистим экран

    attron(COLOR_PAIR(SHOW_NAME_PAIR));
    printw("%s\n", game_map->units_list[PLAYER_INDEX].name);
    attroff(COLOR_PAIR(SHOW_NAME_PAIR));

    printw("HP: ");
    attron(COLOR_PAIR(SHOW_HP_PAIR));
    printw("%d\n", game_map->units_list[PLAYER_INDEX].hp);
    attroff(COLOR_PAIR(SHOW_HP_PAIR));

    printw("DMG: ");
    attron(COLOR_PAIR(SHOW_DMG_PAIR));
    printw("%d\n", game_map->units_list[PLAYER_INDEX].dmg);
    attroff(COLOR_PAIR(SHOW_DMG_PAIR));

    printw("DEFENSE: ");
    attron(COLOR_PAIR(SHOW_DEFENSE_PAIR));
    printw("%d\n\n", game_map->units_list[PLAYER_INDEX].defense);
    attroff(COLOR_PAIR(SHOW_DEFENSE_PAIR));

    printw("Inventory:\n\n");
    // перебираем весь инвертарь player'a

    int start = 0;

    if (selected_item_index >= VISIBLE_INVENTORY_SIZE)
    {
        attron(COLOR_PAIR(SCROLL_PAIR));
        printw("^ ^ ^\n\n");
        start = selected_item_index - VISIBLE_INVENTORY_SIZE + 1;
        attroff(COLOR_PAIR(SCROLL_PAIR));
    }

    for (int i = start; i < start + fmin(game_map->units_list[PLAYER_INDEX].inventory.current_size, VISIBLE_INVENTORY_SIZE); ++i)
    {
        if (i == selected_item_index)
            attron(COLOR_PAIR(SELECTED_ITEM_PAIR));

        bool is_eq;
        is_equipped(game_map->units_list + PLAYER_INDEX, i, &is_eq);
        printw("%d: %s", i + 1, game_map->units_list[PLAYER_INDEX].inventory.items[i].name);

        if (is_eq)
        {
            attron(COLOR_PAIR(EQUIPPED_PAIR));
            printw(" (e)");
            attroff(COLOR_PAIR(EQUIPPED_PAIR));
        }
        printw("\n\n");
        if (i == selected_item_index)
            attroff(COLOR_PAIR(SELECTED_ITEM_PAIR));
    };

    if (game_map->units_list[PLAYER_INDEX].inventory.current_size > VISIBLE_INVENTORY_SIZE && selected_item_index < game_map->units_list[PLAYER_INDEX].inventory.current_size - 1)
    {
        attron(COLOR_PAIR(SCROLL_PAIR));
        printw("v v v\n");
        attroff(COLOR_PAIR(SCROLL_PAIR));
    }
};

void enter_name()
{
    int row, col;
    getmaxyx(stdscr, row, col);
    mvprintw(row / 2, (col - 60) / 2, "Enter name: ");
}

void show_input()
{
    echo();
    curs_set(1);
}

void hide_input()
{
    noecho();
    curs_set(0);
}

void print_death_screen(GameMap *game_map)
{
    clear();
    attron(COLOR_PAIR(DEATH_SCREEN_PAIR));
    int row, col;
    getmaxyx(stdscr, row, col);
    mvprintw((row - 15) / 2, (col - 58) / 2,
             "         _.---,._,'");
    mvprintw((row - 15) / 2 + 1, (col - 58) / 2,

             "    /' _.--.<");
    mvprintw((row - 15) / 2 + 2, (col - 58) / 2,

             "      /'     `'");
    mvprintw((row - 15) / 2 + 3, (col - 58) / 2,

             "    /' _.---._____");
    mvprintw((row - 15) / 2 + 4, (col - 58) / 2,

             "    \\.'   ___, .-'`");
    mvprintw((row - 15) / 2 + 5, (col - 58) / 2,

             "        /'    \\\\             .");
    mvprintw((row - 15) / 2 + 6, (col - 58) / 2,

             "      /'       `-.          -|-");
    mvprintw((row - 15) / 2 + 7, (col - 58) / 2,

             "     |                       |");
    mvprintw((row - 15) / 2 + 8, (col - 58) / 2,

             "     |                   .-'~~~`-.");
    mvprintw((row - 15) / 2 + 9, (col - 58) / 2,

             "     |                 .'         `.");
    mvprintw((row - 15) / 2 + 10, (col - 58) / 2,

             "     |                 |  R  I  P  |");
    mvprintw((row - 15) / 2 + 11, (col - 58) / 2,

             "     |                 |           |");
    mvprintw((row - 15) / 2 + 12, (col - 58) / 2,

             "     |                 |           |");
    mvprintw((row - 15) / 2 + 13, (col - 58) / 2,

             "      \\              \\\\|           |//");
    mvprintw((row - 15) / 2 + 14, (col - 58) / 2,

             "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    attroff(COLOR_PAIR(DEATH_SCREEN_PAIR));
    mvprintw((row + 16) / 2, (col - (16 + strlen(game_map->units_list[PLAYER_INDEX].name))) / 2, "%s died on level %d", game_map->units_list[PLAYER_INDEX].name, game_map->level);
    mvprintw((row + 18) / 2, (col - 27) / 2, "Press any button to continue\n");
}

void print_win_screen(GameMap *game_map)
{
    clear();
    int row, col;
    getmaxyx(stdscr, row, col);
    attron(COLOR_PAIR(WIN_SCREEN_PAIR));
    mvprintw((row - 16) / 2 + 1, (col - 42) / 2,
             "       ____...------------...____");
    mvprintw((row - 16) / 2 + 2, (col - 42) / 2,
             "  _.-\"` /o/__ ____ __ __  __ \\o\\_`\"-._");
    mvprintw((row - 16) / 2 + 3, (col - 42) / 2,
             ".'     / /                    \\ \\     '.");
    mvprintw((row - 16) / 2 + 4, (col - 42) / 2,
             "|=====/o/======================\\o\\=====|");
    mvprintw((row - 16) / 2 + 5, (col - 42) / 2,
             "|____/_/________..____..________\\_\\____|");
    mvprintw((row - 16) / 2 + 6, (col - 42) / 2,
             "/   _/ \\_     <_o#\\__/#o_>     _/ \\_   \\");
    mvprintw((row - 16) / 2 + 7, (col - 42) / 2,
             "\\________________\\####/________________/");
    mvprintw((row - 16) / 2 + 8, (col - 42) / 2,
             " |===\\!/========================\\!/===|");
    mvprintw((row - 16) / 2 + 9, (col - 42) / 2,
             " |   |=|          .---.         |=|   |");
    mvprintw((row - 16) / 2 + 10, (col - 42) / 2,
             " |===|o|=========/     \\========|o|===|");
    mvprintw((row - 16) / 2 + 11, (col - 42) / 2,
             " |   | |         \\() ()/        | |   |");
    mvprintw((row - 16) / 2 + 12, (col - 42) / 2,
             " |===|o|======{'-.) A (.-'}=====|o|===|");
    mvprintw((row - 16) / 2 + 13, (col - 42) / 2,
             " | __/ \\__     '-.\\uuu/.-'    __/ \\__ |");
    mvprintw((row - 16) / 2 + 14, (col - 42) / 2,
             " |==============.'.'^'.'.==============|");
    mvprintw((row - 16) / 2 + 15, (col - 42) / 2,
             " |  _\\o/   __  {.' __  '.} _   _\\o/  _|");
    mvprintw((row - 16) / 2 + 16, (col - 42) / 2,
             " `\"\"\"\"-\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"-\"\"\"\"`\n");
    attroff(COLOR_PAIR(WIN_SCREEN_PAIR));

    mvprintw((row + 18) / 2, (col - (21 + strlen(game_map->units_list[PLAYER_INDEX].name))) / 2, "%s completed all levels", game_map->units_list[PLAYER_INDEX].name);
    mvprintw((row + 18) / 2 + 1, (col - 22) / 2, "You killed %d monsters\n", game_map->units_list[PLAYER_INDEX].kills);
    mvprintw((row + 18) / 2 + 2, (col - 28) / 2, "Press any button to continue\n");
}
