#include "Map.h"
#include <time.h>
#include <stdlib.h>

// если имя функции начинается с нижнего подчёркивания, то она исключительно внутрифайловая,
// то есть, выставлять её прототип в .h я не буду


// структура-контейнер для настроек компиляции
typedef struct
{
	int chance; 			// вероятность (0; 1) , с которой клетка становится стеной (чем выше, тем больше стен на карте)
	int death_limit; 		// рекомендую задавать в 3
	int birth_limit; 		// рекомендую задавать в 4
	int steps;				// сколько раз карта будет обновлять своё состояние (обычно хватает 2-5 раз)
} Settings;	

Settings _std_settings = {36, 3, 4, 5};

/**
 * @brief считает кол-во стен вокруг клетки
 * @param game_map указатель на карту, где находится клетка
 * @param x координата клетки по х
 * @param y координата клетки по y
 * @return код ошибки
 */
int _cnt_neighbours(GameMap *game_map, int x, int y, int *result)
{
    *result = 0;
    
    for (int i = -1; i <= 1; ++i)
		for (int j = -1; j <= 1; ++j)
		{
		    int neigbour_x = x + j;
            int neigbour_y = y + i;
            
            if (neigbour_x == x && neigbour_y == y)
            {
                //саму клетку не считаем!
                continue;
            };
            
            if ((neigbour_x < 0) || (neigbour_y < 0) || (neigbour_x >= game_map -> size_x) || (neigbour_y >= game_map -> size_y))
            {
                // если сосед -- край карты, то засчитываем
                *result += 1;
                continue;
            };
            
            if (game_map -> data[neigbour_x][neigbour_y].type == WALL_CELL)
            {
                // если сосед -- стенка, то засчитываем
                *result += 1;
			};
		};
    return 0;
};

int _copy_map(GameMap *game_map_1, GameMap *game_map_2)
{
	int size_x = game_map_1 -> size_x;
	int size_y = game_map_1 -> size_y;
	
	game_map_2 -> data = malloc(sizeof(Cell) * size_y);
	for (int i = 0; i < size_y; ++i)
	{
		game_map_2 -> data[i] = malloc(sizeof(Cell) * size_x);
		for (int j = 0; j < size_x; ++j)
			game_map_2 -> data[i][j] = game_map_1 -> data[i][j];
	};
	return 0;
};

int delete_map(GameMap *game_map)
{
	for (int i = 0; i < game_map -> size_y; ++i)
		free(game_map -> data[i]);
	free(game_map -> data);
};

/**
 * @brief переводит карту в следующее состояние 
 * @param game_map указатель на карту
 * @param death_limit передать то, что поступило в generate_maps_landscape
 * @param birth_limit передать то, что поступило в generate_maps_landscape
 * @return код ошибки 
 */
int _next_state(GameMap *game_map)
{    
	GameMap tmp;
	_copy_map(game_map, &tmp);
	
    for (int x = 0; x < game_map -> size_x; ++x)
		for (int y = 0; y < game_map -> size_y; ++y)
		{
			int neighbours;
            _cnt_neighbours(game_map, x, y, &neighbours);
            if(game_map -> data[x][y].type == FREE_CELL)
            {
                if (neighbours < _std_settings.death_limit)
				{	
                    tmp.data[x][y].type = FREE_CELL;
                }
                else
                {
                    tmp.data[x][y].type = WALL_CELL;
				};
            }
            else
            {
                if (neighbours > _std_settings.birth_limit)
                {
                    tmp.data[x][y].type = WALL_CELL;
                }
                else
                {
                    tmp.data[x][y].type = FREE_CELL;
				};
			};
         };
    _copy_map(&tmp, game_map);
    delete_map(&tmp);
	return 0;
};

/**
 * @brief генерирует ландашфт игровой карты (на основе алгоритма игры "жизнь")
 * @param game_map указатель на карту, ландшафт которой будем генерировать
 * @return код ошибки
 */
int generate_maps_landscape(GameMap *game_map)
{
	srand(time(0));		// подключаем случайные числа
	for (int i = 0; i < game_map -> size_y; ++i)
		for (int j = 0; j < game_map -> size_x; ++j)
		{
			int random_value = rand() % (100+1); // приводим к промежутку [0; 100] -- к процентам
			game_map -> data[i][j].type = (random_value <= _std_settings.chance);
		};
	
	for (int i = 0; i < _std_settings.steps; ++i)
		_next_state(game_map);
	
	return 0;
};
