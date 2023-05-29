#include "Map.h"
#include "SavingSystem.h"
#include <ncurses.h>

void init_ncurses();
void print_death_screen(GameMap *game_map);
void print_win_screen(GameMap *game_map);
void show_menu(int selected);
void save_to_leaderboard(Unit *player);
void print_leaderboard();
void enter_name();
void show_input();
void hide_input();
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
    SHOW_DEFENSE_PAIR,
    DEATH_SCREEN_PAIR,
    WIN_SCREEN_PAIR,
    EQUIPPED_PAIR,
    SCROLL_PAIR
} ColorPair;
