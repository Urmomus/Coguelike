#include "Map.h"
#include <time.h>
#include <stdlib.h>

// если имя функции начинается с нижнего подчёркивания, то она исключительно внутрифайловая,
// то есть, выставлять её прототип в .h я не буду

/**
 * @brief считает кол-во стен вокруг клетки
 * @param game_map указатель на карту, где находится клетка
 * @param x координата клетки по х
 * @param y координата клетки по y
 * @return код ошибки
 */
int _cnt_neighbours(GameMap *game_map, int x, int y, int result)
{
	return 0;
};

/**
 * @brief переводит карту в следующее состояние 
 * @param game_map указатель на карту
 * @param death_limit передать то, что поступило в generate_maps_landscape
 * @param birth_limit передать то, что поступило в generate_maps_landscape
 * @return код ошибки 
 */
int _next_state(GameMap *game_map, int death_limit, int birth_limit)
{    
	size = len(Map)
    new_Map = [[FREE] * size for i in range(size)]
    
    for x in range(size):
        for y in range(size):
            num_of_neighbours = cnt_neighbours(x, y, Map)
            if Map[x][y]:
                if num_of_neighbours < dl:
                    new_Map[x][y] = FREE
                else:
                    new_Map[x][y] = WALL
            else:
                if num_of_neighbours > bl:
                    new_Map[x][y] = WALL
                else:
                    new_Map[x][y] = FREE
	return 0;
};

/**
 * @brief генерирует ландашфт игровой карты (на основе алгоритма игры "жизнь")
 * @param game_map указатель на карту, ландшафт которой будем генерировать
 * @param chance вероятность (0; 1) , с которой клетка становится стеной (чем выше, тем больше стен на карте)
 * @param death_limit рекомендую задавать в 3
 * @param birth_limit рекомендую задавать в 4
 * @param steps сколько раз карта будет обновлять своё состояние (обычно хватает 2-5 раз)
 * @return код ошибки
 */
int _generate_maps_landscape(
		GameMap *game_map, 
		int chance, 
		int death_limit, 
		int birth_limit, 
		int steps
	)
{
	srand(time(0));		// подключаем случайные числа
	for (int i = 0; i < game_map -> size_y; ++i)
		for (int j = 0; j < game_map -> size_x; ++j)
		{
			int random_value = rand() % (100+1); // приводим к промежутку [0; 100] -- к процентам
			game_map -> data[i][j].type = (random_value <= chance);
		};
	
	for (int i = 0; i < steps; ++i)
		_next_state(game_map, death_limit, birth_limit);
	
	return 0;
};


/**
 * @brief генерирует ландашфт игровой карты (на основе алгоритма игры "жизнь")
 * @param game_map указатель на карту, ландшафт которой будем генерировать
 * @return код ошибки
 */
int generate_maps_landscape(GameMap *game_map)
{
	// я перегружаю ф-ю _generate_maps_landscape, чтобы пользователь не нащёлкал кривых параметров
	
	// параметры по умолчанию
	int chance = 36; 
	int death_limit=3; 
	int birth_limit=4; 
	int steps=5;
	
	return _generate_maps_landscape(game_map, chance, death_limit, birth_limit, steps);
};
