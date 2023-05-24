#include "units.h"
#include "Map.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char* free_cell = "\033[37;40m.";
char* wall_cell = "\033[37;40m#";
char* mob_cell = "\033[40;35mG";
char* player_cell = "\033[40;31m@";
char* item_cell = "\033[40;33mi";
char* portal_cell = "\033[40;34m@";

void print_map(GameMap *game_map);
void print_inventory(GameMap *game_map);

int main()
{
    srand(5);

    char game_mode = 'g';    // g = game, i = inventory

    GameMap game_map;   
    
    game_map.units_list = NULL;
    game_map.items_list = NULL;
    game_map.data = NULL;
    
    MapSettings settings;

    settings.size_x = 20;
    settings.size_y = 20;
    settings.level = 1;

    int err_code = init_map(&game_map, settings);
    //printf("%d\n", err_code);
    err_code = generate_maps_landscape(&game_map);
    if (err_code)
    {
        printf("ERROR: %d", err_code);
        return 1;
    }
    err_code = generate_maps_content(&game_map);

    int game_is_finished = false;
    for (; !game_is_finished ;)
    {
        if (game_mode == 'g')
            print_map(&game_map);

        if (game_mode == 'i')
            print_inventory(&game_map);

        char cmd;
        cmd = getchar();
        fflush(stdin);
        switch (cmd)
        {
            case 'a':
            {
                move_player(&game_map, 'l');
                break;
            };
            case 'w':
            {
                move_player(&game_map, 'u');
                break;
            };
            case 's':
            {
                move_player(&game_map, 'd');
                break;
            };
            case 'd':
            {
                move_player(&game_map, 'r');
                break;
            };
            case 'c':
            {
                move_player(&game_map, 's');
                break;
            };
            case 'i':
            {
                // входим / выходим из инвентаря
                if (game_mode == 'g')
                    game_mode = 'i';
                else
                    game_mode = 'g';
                break;
            };
            case 'e':
            {
                // экипируем предмет
                scanf("%d", (int*)&cmd);

                equip_from_inventory(game_map.units_list + PLAYER_INDEX, cmd);
                break;
            };
            case 'u':
            {
                // используем предмет
                scanf("%d", (int*)&cmd);

                use(game_map.units_list + PLAYER_INDEX, cmd);
                break;
            };
            default:
            {
                continue;
            };
        };
        //printf("player was moved\n");
        if (game_mode == 'g')
            move_monsters(&game_map);
        //printf("monster was moved\n");
    };
    
    err_code = delete_map(&game_map);
    //printf("%d\n", err_code);
};

void print_map(GameMap *game_map)
{
    system("clear");
    static int iteration = 0;
    for (int y = 0; y < game_map->size_y; ++y)
    {
        for (int x = 0; x < game_map->size_x; ++x)
        {
            char *cell;
            if (game_map->data[y][x].unit != NULL)
            {
                if (game_map->data[y][x].unit->unit_type == PLAYER)
                    cell = player_cell;
                else
                    cell = mob_cell;
            }
            else if (game_map->data[y][x].item != NULL)
                cell = item_cell;
            else if (game_map->data[y][x].type == WALL_CELL)
                cell = wall_cell;
            else if (game_map->data[y][x].type == FINISH_CELL)
                cell = portal_cell;
            else
                cell = free_cell;
            printf("%s ", cell);
        };
        printf("\n");
    };
    int hp = game_map -> units_list[0].hp;
    int lvl = game_map -> units_list[0].lvl;
    printf("player's level: %d\n", lvl);
    printf("player's HP: %d\n", hp);
    printf("player's name: %s\n", game_map -> units_list[PLAYER_INDEX].name);
 };

 void print_inventory(GameMap *game_map)
 {
    system("clear");    // чистим экран
    printf("Inventory:\n\n");
    // перебираем весь инвертарь player'a
    for (int i = 0; i < game_map -> units_list[PLAYER_INDEX].inventory.current_size; ++i)
    {
        bool is_eq;
        printf("Предмет №%d\n", i);
        printf("Название: %s\n", game_map -> units_list[PLAYER_INDEX].inventory.items[i].name);
        is_equipped(game_map -> units_list + PLAYER_INDEX, i, &is_eq);
        if (is_eq)
            printf("Экипирован\n");
        printf("\n\n");
    };
 };