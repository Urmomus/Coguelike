#include "Map.h"
#include <time.h>
#include <stdlib.h>

// если имя функции начинается с нижнего подчёркивания, то она исключительно внутрифайловая,
// то есть, выставлять её прототип в .h я не буду

enum Errors			// возможные ошибки
{
	SIZE_ERROR,			// размеры карт при копировании не совпадают
};

// структура-контейнер для настроек компиляции
typedef struct
{
	int chance; 			// вероятность (0; 1) , с которой клетка становится стеной (чем выше, тем больше стен на карте)
	int death_limit; 		// рекомендую задавать в 3
	int birth_limit; 		// рекомендую задавать в 4
	int steps;				// сколько раз карта будет обновлять своё состояние (обычно хватает 2-5 раз)
} 
LandsapeSettings;	

LandsapeSettings _std_settings = {36, 3, 4, 5};

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

int _copy_map(GameMap *from, GameMap *to)
{
	if ((from -> size_x != to -> size_x) || (from -> size_y != to -> size_y))
		return SIZE_ERROR;
	
	for (int i = 0; i < from -> size_y; ++i)
		for (int j = 0; j < from -> size_x; ++j)
			to -> data[i][j] = from -> data[i][j];
	
	return 0;
};

int delete_map(GameMap *game_map)
{	
	for (int i = 0; i < game_map -> size_y; ++i)
		free(game_map -> data[i]);
	free(game_map -> data);
	
	return 0;
};

int init_map(GameMap *game_map, MapSettings settings)
{
	// задаём карте указанные размеры
	game_map -> size_x = settings.size_x;
	game_map -> size_y = settings.size_y;
	
	// изначально на карте нет монстров
	game_map -> units_list = NULL;
	game_map -> units_num = 0;

	// и предметов тоже нет
	game_map -> items_list = NULL;
	game_map -> items_num = 0;

	// выделяем память под размеры карты
	game_map -> data = malloc(sizeof(Cell*) * game_map -> size_y);
	for (int i = 0; i < game_map -> size_y; ++i)
		game_map -> data[i] = malloc(sizeof(Cell) * game_map -> size_x);
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
	
	MapSettings tmp_settings;
	tmp_settings.size_x = game_map -> size_x;
	tmp_settings.size_y = game_map -> size_y;
	init_map(&tmp, tmp_settings);
	
	_copy_map(game_map, &tmp);
	
    for (int x = 0; x < game_map -> size_x; ++x)
		for (int y = 0; y < game_map -> size_y; ++y)
		{
			int neighbours;
            _cnt_neighbours(game_map, x, y, &neighbours);
            if(game_map -> data[x][y].type == WALL_CELL)
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


/**
 * @brief размещает монстров и предметы на игровой карте
 * @param game_map игровая карта (с готовым ландшафтом)
 * @param items массив предметов
 * @param items_num кол-во предметов
 * @param units массив мобов
 * @param units_num кол-во мобов
 * @return код ошибки
 */
int place_objects_on_map(GameMap *game_map, Item *items, int items_num, Unit *units, int units_num)
{
	return 0;
};
