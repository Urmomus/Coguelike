#include "Map.h"
#include <ncurses.h>

void init_ncurses();
void print_inventory(GameMap *game_map, int selected_item_index);
void print_map(GameMap *game_map);

typedef enum
{
    DEFAULT,
    PLAYER_PAIR,
    GOBLIN_PAIR,
    SLIME_PAIR,
    GHOST_PAIR,
    SKELETON_PAIR,
    PORTAL_PAIR,
    WALL_PAIR,
    ITEM_PAIR,
    FREE_PAIR,
    SELECTED_ITEM_PAIR,
    SHOW_HP_PAIR,
    SHOW_DMG_PAIR,
    SHOW_NAME_PAIR,
    SHOW_DEFENSE_PAIR
} ColorPair;
