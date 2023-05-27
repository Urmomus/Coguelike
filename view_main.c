#include "Map.h"
#include "view.h"
#include <stdlib.h>
#include <ncurses.h>

void print_map(GameMap *game_map);

int main()
{
    srand(3);
    init_ncurses();

    int selected_item_index = 0;
    char game_mode = 'g'; // g = game, i = inventory

    GameMap game_map;

    game_map.units_list = NULL;
    game_map.items_list = NULL;
    game_map.data = NULL;

    MapSettings settings;

    settings.size_x = 20;
    settings.size_y = 20;
    settings.level = 1;

    int err_code = init_map(&game_map, settings);
    err_code = generate_maps_landscape(&game_map);
    err_code = generate_maps_content(&game_map);

    int game_is_finished = false;
    for (; !game_is_finished;)
    {
        if (game_mode == 'g')
        {
            selected_item_index = 0;
            print_map(&game_map);
        }

        if (game_mode == 'i')
            print_inventory(&game_map, selected_item_index);

        char cmd;
        cmd = getch();

        if (cmd == 'i')
        {
            // входим / выходим из инвентаря
            if (game_mode == 'g')
                game_mode = 'i';
            else
                game_mode = 'g';
        }

        if (game_mode == 'i')
        {
            switch (cmd)
            {
            case 'w':
            {
                if (selected_item_index - 1 >= 0)
                    --selected_item_index;
                break;
            }
            case 's':
            {
                if (selected_item_index + 1 < game_map.units_list[PLAYER_INDEX].inventory.current_size)
                    ++selected_item_index;
                break;
            }
            case 'e':
            {
                ExceptionStatus exception;
                exception = equip_from_inventory(game_map.units_list + PLAYER_INDEX, selected_item_index);
                if (exception == ITEM_IS_EQUIPPED)
                    unequip(game_map.units_list + PLAYER_INDEX, game_map.units_list[PLAYER_INDEX].inventory.items[selected_item_index].type);
                break;
            };
            case 'u':
            {
                use(game_map.units_list + PLAYER_INDEX, selected_item_index);
                break;
            };
            default:
            {
                continue;
            }
            }
        }

        if (game_mode == 'g')
        {
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
            default:
                {
                    continue;
                };
            };

            move_monsters(&game_map);
        }
    };

    err_code = delete_map(&game_map);
};
