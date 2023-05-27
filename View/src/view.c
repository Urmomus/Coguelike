#include "view.h"
#include <curses.h>

char free_cell = '.';
char wall_cell = '#';
char goblin_cell = 'g';
char ghost_cell = 'G';
char slime_cell = 's';
char skeleton_cell = 'S';
char player_cell = '@';
char item_cell = 'i';
char portal_cell = '@';

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
}

void print_map(GameMap *game_map)
{
    clear();
    char cell;
    ColorPair cell_color;

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
    printw("%d\n", game_map->units_list[PLAYER_INDEX].defense);
    attroff(COLOR_PAIR(SHOW_DEFENSE_PAIR));

    printw("Inventory:\n\n");
    // перебираем весь инвертарь player'a

    for (int i = 0; i < game_map->units_list[PLAYER_INDEX].inventory.current_size; ++i)
    {
        if (i == selected_item_index)
            attron(COLOR_PAIR(SELECTED_ITEM_PAIR));
        bool is_eq;
        // printw("Item %d\n", i);
        printw("%d: %s", i, game_map->units_list[PLAYER_INDEX].inventory.items[i].name);
        is_equipped(game_map->units_list + PLAYER_INDEX, i, &is_eq);
        if (is_eq)
            printw(" (e)");
        printw("\n\n");
        if (i == selected_item_index)
            attroff(COLOR_PAIR(SELECTED_ITEM_PAIR));
    };
};
