#include "Map.h"
#include <time.h>
#include <stdlib.h>

// если имя функции начинается с нижнего подчёркивания, то она исключительно внутрифайловая,
// то есть, выставлять её прототип в .h я не буду

enum Errors			// возможные ошибки
{
	NO_ERRORS, 					// нет ошибок
	SIZE_ERROR,					// размеры карт при копировании не совпадают
	MONSTER_OR_ITEMS_LEN_ERROR, // ошибка при копировании карты: не совпадает число монстров / предметов
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
    return NO_ERRORS;
};

int _copy_map(GameMap *from, GameMap *to)
{
	if ((from -> size_x != to -> size_x) || (from -> size_y != to -> size_y))
		return SIZE_ERROR;
	
	if ((from -> items_num != to -> items_num) || (from -> units_num != from -> units_num))
		return MONSTER_OR_ITEMS_LEN_ERROR;

	for (int i = 0; i < from -> size_y; ++i)
		for (int j = 0; j < from -> size_x; ++j)
			to -> data[i][j] = from -> data[i][j];
	
	// не забываем скопировать уровень
	to -> level = from -> level;

	// не забываем скопировать предметы
	for (int i = 0; i < from -> items_num; ++i)
		to -> items_list[i] = from -> items_list[i];
	
	// не забываем скопировать мобов
	for (int i = 0; i < from -> units_num; ++i)
		to -> units_list[i] = from -> units_list[i];

	return NO_ERRORS;
};

int delete_map(GameMap *game_map)
{	
	// сначала чистим всю память, выделенную под клетки.
	for (int i = 0; i < game_map -> size_y; ++i)
		free(game_map -> data[i]);
	free(game_map -> data);
	
	
	// потом чистим память, выделенную под предметы 
	free(game_map -> items_list);
	
	// и мобов
	free(game_map -> units_list);
	
	return NO_ERRORS;
};

int init_map(GameMap *game_map, MapSettings settings)
{
	// задаём карте указанные размеры
	game_map -> size_x = settings.size_x;
	game_map -> size_y = settings.size_y;
	// и уровень
	game_map -> level = settings.level;

	
	// работаем с монстрами и предметами
	const int MONSTER_COEFF = 2; 	// кол-во монстров уровне = уровень * MONSTER_COEFF + 2 + 1(игрок).
	const int ITEM_COEFF = 3;		// кол-во предметов на уровне = уровень * ITEM_COEFF + 5

	// вычисляем их количество
	game_map -> units_num = game_map -> level * MONSTER_COEFF + 2 + 1;
	game_map -> items_num = game_map -> level * ITEM_COEFF + 5;

	// выделяем память под предметы
	game_map -> items_list = malloc(sizeof(Item) * game_map -> items_num);
	
	// выделяем память под юнитов
	game_map -> units_list = malloc(sizeof(Unit) * game_map -> units_num);

	// выделяем память под клетки карты
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

	// копируем настройки исходной карты
	tmp_settings.size_x = game_map -> size_x;
	tmp_settings.size_y = game_map -> size_y;
	tmp_settings.level = game_map -> level;

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
	return NO_ERRORS;
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
	
	return NO_ERRORS;
};

/**
 * @brief генерирует и размещает монстров и предметы на игровой карте
 * @param game_map игровая карта (с готовым ландшафтом)
 * @return код ошибки
 */
int generate_maps_content(GameMap *game_map)
{
	// создаём монстров и предметы (списки)

	//generate_monsters(&game_map -> units_list, game_map -> units_num, game_map -> level);
	//generate_loot(&game_map -> items_list, game_map -> items_num, game_map -> level);
	
	
	
	return OK;
};