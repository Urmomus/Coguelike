#include "Map.h"
#include "ErrorCodes.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
#include <math.h>

// если имя функции начинается с нижнего подчёркивания, то она исключительно внутрифайловая,
// то есть, выставлять её прототип в .h я не буду

// структура-контейнер для настроек компиляции
typedef struct
{
	int chance; 			// вероятность (0; 1) , с которой клетка становится стеной (чем выше, тем больше стен на карте)
	int death_limit; 		// рекомендую задавать в 3
	int birth_limit; 		// рекомендую задавать в 4
	int steps;				// сколько раз карта будет обновлять своё состояние (обычно хватает 2-5 раз)
} 
LandsapeSettings;	


// константы да переменные

int PLAYER_INDEX = 0;							// индекс, под которым в массиве units_list расположен игрок. 
const int MAX_LEVEL = 10;
LandsapeSettings _std_settings = {30, 3, 4, 5};		// дефолтные настройки для генерации ландшафта карты

// приватные функции

/****
	@brief проверяет координаты (х, у) на корректность
	@param game_map игровая карта, на которой отмечена точка (х, у)
	@param x координата по х
	@param y координата по у
	@return 1 -- если корректны, 0 -- если некорректны
*/
char _coords_are_correct(GameMap *game_map, int x, int y);

/**
 * @brief считает кол-во стен вокруг клетки
 * @param game_map указатель на карту, где находится клетка
 * @param x координата клетки по х
 * @param y координата клетки по y
 * @return код ошибки
 */
int _cnt_neighbours(GameMap *game_map, int x, int y, int *result);

/***
TODO: написать заголовок
*/
int _copy_map(GameMap *from, GameMap *to);

/**
 * @brief переводит карту в следующее состояние 
 * @param game_map указатель на карту
 * @param death_limit передать то, что поступило в generate_maps_landscape
 * @param birth_limit передать то, что поступило в generate_maps_landscape
 * @return код ошибки 
 */
int _next_state(GameMap *game_map);

/***
	@brief стандартная (т.е. рекурсивная) реализация поиска в ширину
	@param game_map игровая карта, на которой выполняется dfs
	@param used массив, где 0 -- клетка не посещалась, что-то ещё -- посещалась
	@param x х текущей клетки
	@param y y текущей клетки
	@param size_x размер массива по x
	@param size_y размер массива по у
	@param nummer_of_island чем заполнять открытые территории, связанные с (x, y)
	@param size_of_island кол-во клеток, привязанных к данной (передавать строго 0 (ноль)!!!)
*/
void _dfs(GameMap *game_map, char **used, int x, int y, int size_x, int size_y, int nummer_of_island, int *size_of_island);


/***
	@brief могильный ужас, а не функция: подбирает такие размеры прямоугольника, чтобы на карту влезло определённое число прямоугольников
	@param blocks_num число прямоугольников
	@param map_size_x размер карты по х
	@param map_size_y размер карты по у
	@param block_size_x размер прямоугольника по х
	@param block_size_y размер прямоугольника по у
	@param deep -- глубина рекурсии. Изначально равна нулю!
*/
void _cnt_boundaries(int blocks_num, int map_size_x, int map_size_y, int *block_size_x, int *block_size_y, unsigned int deep);

/***
	@brief функция-обобщения, которая может расставлять и монстров, и предметы на карте
	@param game_map карта, где надо размещать
	@param items_num кол-во размещаемых объектов
	@param type: 'u' -- units, 'i' -- items.
*/
void _place_objects_on_map(GameMap *game_map, int objects_num, char type);

/***
	@brief перемещает юнита на одну клетку в указанном направлении
	@param game_map карта, где находится юнит
	@param ind индекс юнита в списке юнитов
	@param dir направление, куда перемещаться: 'l' -- влево, 'r' -- вправо, 'u' -- вверх, 'd' -- вниз, 's' -- стоять.
	@return код ошибки
*/
int _move_unit(GameMap *game_map, int ind, char dir);

/******
	@brief отвязывает юнита от клетки на карте 
	@param game_map карта, где происходит действие
	@param ind индекс юнита в массиве units_list
	@return код ошибки	
*/
int _unbind_unit_from_cell(GameMap *game_map, int ind);

/******
	@brief привязывает юнита к клетке на карте 
	@param game_map карта, где происходит действие
	@param ind индекс юнита в массиве units_list
	@param x координата по х клетки, к которой надо привязать юнита
	@param y координата по y клетки, к которой надо привязать юнита
	@return код ошибки	
*/
int _bind_unit_to_cell(GameMap *game_map, int ind, int x, int y);

/********
	@brief определяет, является ли индекс предмета / монстра корректным
	@param game_map карта, где происходит действие
	@param ind индекс предмета / монстра
	@param type 'u', если юнит, 'i', если предмет
	@return 1, если индекс корректен, и 0 -- в обратном случае
*/
int _index_is_correct(GameMap *game_map, int ind, char type);

/************
	@brief определяет направление, в котором должен двигаться монстр, и возвращает оное направление
	@param game_map игровая карта, где происходит действие
	@param ind индекс монстра, который ориентируется на местности
	@param char символ направления: один из {'l', 'r', 'd', 'u', 's'}
*/
char _cnt_direction_for_move(GameMap *game_map, int ind);

/****
 * @brief обновляет состояние юнита: если юнит умер, то он отвязывается от карты
 * @param game_map игровая карта
 * @param unit юнит, состояние которого обновляем
 * @return код ошибки
*/
int _resresh_is_live(GameMap *game_map, Unit *unit);

/****
 * @brief проверяет, что юнит выставлен на карте (т.е. -- живой и может двигаться)
 * @param game_map игровая карта
 * @param unit юнит
 * @return 0, если не выставлен, 1, если выставлен
*/
int _unit_on_map(GameMap *game_map, Unit *unit);

/********
 * @brief переводит игрока на след. уровень
 * @param game_map игровая карта
 * @return код ошибки
*/
int _next_level(GameMap *game_map);

/*********
 * @brief создаёт копию юнита 
 * @param orig оригинал юнита
 * @param new_unit куда копировать
 * @return копия юнита
*/
void _copy_unit(Unit *orig, Unit *new_unit);

// реализации функций


/****
	@brief проверяет координаты (х, у) на корректность
	@param game_map игровая карта, на которой отмечена точка (х, у)
	@param x координата по х
	@param y координата по у
	@return 1 -- если корректны, 0 -- если некорректны
*/
char _coords_are_correct(GameMap *game_map, int x, int y)
{
	if (x < 0 || x >= game_map -> size_x)
		return 0;
	if (y < 0 || y >= game_map -> size_y)
		return 0;
	
	return 1;
};

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
            
            if(!_coords_are_correct(game_map, neigbour_x, neigbour_y))
			{
                // если сосед -- край карты, то засчитываем
                *result += 1;
                continue;
            };
            
            if (game_map -> data[neigbour_y][neigbour_x].type == WALL_CELL)
            {
                // если сосед -- стенка, то засчитываем
                *result += 1;
			};
		};
    return NO_ERRORS;
};

/***
TODO: написать заголовок
*/
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

/*****
	@brief чистит память, выделенную под карту (включая монстров и предметы, прицепленные к карте)
	@param game_map карта
	@return код ошибки
*/
int delete_map(GameMap *game_map)
{	
	// проверка 0: проверяем, что не навалили нулевых указателей
	if (game_map == NULL)
		return EMPTY_POINTER;

	// проверка 1: проверяем, что есть, что удалять
	if (game_map -> data == NULL)
		return MAP_ALREADY_DELETED;

	// сначала чистим всю память, выделенную под клетки.
	for (int i = 0; i < game_map -> size_y; ++i)
		free(game_map -> data[i]);
	free(game_map -> data);
	
	
	// потом чистим память, выделенную под предметы 
	free(game_map -> items_list);
	
	// и мобов
	free(game_map -> units_list);
	
	// и зануляем все указатели, чтобы показать, что карта очищена
	game_map -> data = NULL;
	game_map -> units_list = NULL;
	game_map -> items_list = NULL;

	return OK;
};

/****
	@brief инициализирует карту начальными значениями
	@param game_map карта
	@param settings настройки, из которых возьмутся начальные значения
	@return код ошибки  
*/
int init_map(GameMap *game_map, MapSettings settings)
{
	// проверка 2: проверяем, что нам не навалили нулевых указателей
	if (game_map == NULL)
		return EMPTY_POINTER;

	// проверка 4: проверяем, что карта ещё не была инициализирована
	if (game_map -> data != NULL || game_map -> units_list != NULL || game_map -> items_list != NULL)
		return MAP_ALREADY_EXISTS;

	// проверка 1: проверяем, что переданные размеры -- корректные
	if (settings.size_x <= 0 || settings.size_y <= 0)
		return INVALID_MAP_SIZE;

	// проверка 3: проверяем, что задали корректный уровень
	if (settings.level <= 0)
		return INVALID_LEVEL;

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
	
	// изначально ни один из юнитов нигде не стоит
	for (int i = 0; i < game_map -> units_num; ++i)
	{
		game_map -> units_list[i].x = game_map -> units_list[i].y = -1;
	};

	// выделяем память под клетки карты
	game_map -> data = malloc(sizeof(Cell*) * game_map -> size_y);
	for (int y = 0; y < game_map -> size_y; ++y)
		game_map -> data[y] = malloc(sizeof(Cell) * game_map -> size_x);
	
	return OK;
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
	
	tmp.units_list = NULL; tmp.items_list = NULL; tmp.data = NULL;

	MapSettings tmp_settings;

	// копируем настройки исходной карты
	tmp_settings.size_x = game_map -> size_x;
	tmp_settings.size_y = game_map -> size_y;
	tmp_settings.level = game_map -> level;

	int err_code = init_map(&tmp, tmp_settings);
	if(err_code)
		return err_code;
	
	err_code = _copy_map(game_map, &tmp);
	if(err_code)
		return err_code;

	
    for (int x = 0; x < game_map -> size_x; ++x)
		for (int y = 0; y < game_map -> size_y; ++y)
		{
			int neighbours;
            _cnt_neighbours(game_map, x, y, &neighbours);
            if(game_map -> data[y][x].type == WALL_CELL)
            {
                if (neighbours < _std_settings.death_limit)
				{	
                    tmp.data[y][x].type = FREE_CELL;
                }
                else
                {
                    tmp.data[y][x].type = WALL_CELL;
				};
            }
            else
            {
                if (neighbours > _std_settings.birth_limit)
                {
                    tmp.data[y][x].type = WALL_CELL;
                }
                else
                {
                    tmp.data[y][x].type = FREE_CELL;
				};
			};
         };
         
    _copy_map(&tmp, game_map);
    delete_map(&tmp);
	return OK;
};

/***
	@brief стандартная (т.е. рекурсивная) реализация поиска в ширину
	@param game_map игровая карта, на которой выполняется dfs
	@param used массив, где 0 -- клетка не посещалась, что-то ещё -- посещалась
	@param x х текущей клетки
	@param y y текущей клетки
	@param size_x размер массива по x
	@param size_y размер массива по у
	@param nummer_of_island чем заполнять открытые территории, связанные с (x, y)
	@param size_of_island кол-во клеток, привязанных к данной (передавать строго 0 (ноль)!!!)
*/
void _dfs(GameMap *game_map, char **used, int x, int y, int size_x, int size_y, int nummer_of_island, int *size_of_island)
{

	if (!_coords_are_correct(game_map, x, y))	// вышли за пределы карты
		return;
	
	if (game_map -> data[y][x].type == WALL_CELL)	// попали в стену
		return;
	if (used[y][x] != 0) 		// эта клетка уже посчитана
		return;

	*size_of_island += 1;
	used[y][x] = nummer_of_island;
	_dfs(game_map, used, x + 1, y, size_x, size_y, nummer_of_island, size_of_island);
	_dfs(game_map, used, x - 1, y, size_x, size_y, nummer_of_island, size_of_island);
	_dfs(game_map, used, x, y + 1, size_x, size_y, nummer_of_island, size_of_island);
	_dfs(game_map, used, x, y - 1, size_x, size_y, nummer_of_island, size_of_island);
};

/**
 * @brief генерирует ландашфт игровой карты (на основе алгоритма игры "жизнь")
 * @param game_map указатель на карту, ландшафт которой будем генерировать
 * @return код ошибки
 */
int generate_maps_landscape(GameMap *game_map)
{
	// проверяем, что на вход пришла карта, а не NULL
	if (game_map == NULL)
		return EMPTY_POINTER;
	// проверяем, что на вход пришла инициализированная карта
	if (game_map -> data == NULL || game_map -> units_list == NULL || game_map -> items_list == NULL)
		return MAP_ALREADY_DELETED;	
	for (int y = 0; y < game_map -> size_y; ++y)
		for (int x = 0; x < game_map -> size_x; ++x)
		{
			int random_value = rand() % (100+1); // приводим к промежутку [0; 100] -- к процентам
			game_map -> data[y][x].type = (random_value <= _std_settings.chance);
			game_map -> data[y][x].unit = NULL; // на этом этапе на клетках нет ничего и никого
			game_map -> data[y][x].item = NULL;	
		};

	for (int i = 0; i < _std_settings.steps; ++i)
	{
		int err_code = _next_state(game_map);
		if (err_code)
			return err_code;
	};


	// TODO: вынести в отдельную функцию замуровывание островов
	// на этом месте карта уже сгенерировалась, но на ней могут быть "островки" -- места, не связанные друг с другом.
	// чтобы это вылечить, делаем так: ищем при помощи bfs (поиска в глубину) самый большой остров и стираем все остальные.

	char **used; // технический массив для bfs (0 -- что клетка не посещалась; 1 -- что клетка принадлежит острову №1 и т.д.).
	used = malloc(sizeof(char*) * game_map -> size_y);
	for (int y = 0; y < game_map -> size_y; ++y)
	{
		used[y] = malloc(sizeof(char) * game_map -> size_x);
		for (int x = 0; x < game_map -> size_x; ++x)
			used[y][x] = 0;
	};

	// размер наибольшего острова и номер наибольшего острова, соответственно
	int max_size_of_island, nummer_of_max_island;
	max_size_of_island = 0;
	nummer_of_max_island = 0;

	// нумеруем все острова и находим наибольший среди них
	for (int y = 0; y < game_map -> size_y; ++y)
		for (int x = 0; x < game_map -> size_x; ++x)
		{
			// рассматриваем только клетки, по которым можно ходить
			if (game_map -> data[y][x].type == WALL_CELL)
				continue;
			
			int size_of_island = 0;
			//  ind -- это порядковый номер клетки, и он всегда уникален
			int ind = y * game_map -> size_x + x;
			_dfs(game_map, used, x, y, game_map -> size_x, game_map -> size_y, ind, &size_of_island);

			if (size_of_island > max_size_of_island)
			{
				max_size_of_island = size_of_island;
				nummer_of_max_island =  y * game_map -> size_x + x;
			};
		};

	for (int y = 0; y < game_map -> size_y; ++y)
		for (int x = 0; x < game_map -> size_x; ++x)
		{
			// замуровываем наглухо все острова, кроме наибольшего.
			if (used[y][x] != nummer_of_max_island)
				game_map -> data[y][x].type = WALL_CELL;
		};


	// чистим used
	for (int y = 0; y < game_map -> size_y; ++y)
		free(used[y]);
	free(used);

	// осталось закинуть на карту "переход на следующий уровень" -- специальную клетку
	int num_of_free_cells = 0;	// кол-во свободных клеток -- на них можно разместить "переход"
	for (int y = 0; y < game_map -> size_y; ++y)
		for (int x = 0; x < game_map -> size_x; ++x)
			num_of_free_cells += (game_map -> data[y][x].type == FREE_CELL);
	
	// генерируем случайное число в пределах [0; num_of_free_cells);
	int place_for_portal = rand() % num_of_free_cells;	// номер свободной клетки, где стоит портал
	int now_cell = 0;		// номер текущей свободной клетки
	
	for (int y = 0; y < game_map -> size_y; ++y)
		for (int x = 0; x < game_map -> size_x; ++x)
		{
			if (game_map -> data[y][x].type != FREE_CELL)
				continue;
			if (now_cell == place_for_portal)
			{
				game_map -> data[y][x].type = FINISH_CELL;
			};
			now_cell += 1;
		};

	return NO_ERRORS;
};

const unsigned int MAX_DEEP_CNT_BOUNDARIES = 5;
/***
	@brief могильный ужас, а не функция: подбирает такие размеры прямоугольника, чтобы на карту влезло определённое число прямоугольников
	@param blocks_num число прямоугольников
	@param map_size_x размер карты по х
	@param map_size_y размер карты по у
	@param block_size_x размер прямоугольника по х
	@param block_size_y размер прямоугольника по у
	@param deep -- глубина рекурсии. Изначально равна нулю!
*/
void _cnt_boundaries(int blocks_num, int map_size_x, int map_size_y, int *block_size_x, int *block_size_y, unsigned int deep)
{
	static int min_diff = (1 << sizeof(int));

	if (deep >= MAX_DEEP_CNT_BOUNDARIES)
		return;

	int a, b;

	for (int i = 1; i * i <= blocks_num; ++i)
	{
		if (blocks_num % i)
			continue;
		a = map_size_x / i;
		b = map_size_y / (blocks_num / i);
		
		int diff = abs(a - b);
		if (diff < min_diff)
		{
			*block_size_x = a;
			*block_size_y = b;	
		};
	};

	_cnt_boundaries(blocks_num+1, map_size_x, map_size_y, block_size_x, block_size_y, deep+1);
};

/***
	@brief функция-обобщения, которая может расставлять и монстров, и предметы на карте
	@param game_map карта, где надо размещать
	@param items_num кол-во размещаемых объектов
	@param type: 'u' -- units, 'i' -- items.
*/
void _place_objects_on_map(GameMap *game_map, int objects_num, char type)
{
	// что здесь происходит?
	// карта разбивается на прямоугольные блоки.
	// внутри каждого такого блока лежит объект.

	int block_size_x, block_size_y;
	_cnt_boundaries(objects_num, game_map -> size_x, game_map -> size_y, &block_size_x, &block_size_y, 0);

	/*
	// эхо-печать
	printf("кол-во объектов: %d\n", objects_num);
	printf("площадь на объект: %d\n", block_size_x * block_size_y);
	printf("размер блока по х: %d\n", block_size_x);
	printf("размер блока по у: %d\n", block_size_y);
	*/

	int blocks_in_line = (game_map -> size_x / block_size_x);		// кол-во блоков в ряду 
	int cells_in_line = block_size_x * blocks_in_line;				// кол-во клеток в оном ряду из блоков

	// перебираем все объекты
	for (int i = 0; i < objects_num; ++i)
	{
		// нижний левый угол: (x_1, y_1)
		// правый верхний угол: (x_2, y_2)

		// высчитываем координаты текущего блока

		int x_1 = (i * block_size_x) % cells_in_line;
		int y_1 = (i / blocks_in_line) * block_size_y;

		// здесь уже гарантируется, что следующий блок можно воткнуть просто
		// впритык слева от предыдущего
		int x_2 = x_1 + block_size_x - 1;	// не забываем вычесть 1, чтобы блоки не накладывались друг на друга
		int y_2 = y_1 + block_size_y - 1;
		
		// эхо-печать
		//printf("блок номер %d\n", i);
		//printf("%d\t%d\n%d\t%d\n", x_1, y_1, x_2, y_2);

		// идея такая: мы считаем количество клеток suitable_cells_cnt, на которые можно разместить предмет, внутри блока.
		// потом берём рандомное число ind, меньшее, чем suitable_cells_cnt, и ставим текущий предмет на свободную клетку
		// блока под номером ind. 

		int suitable_cells_cnt = 0;

		// проходим по блоку
		for (int y = y_1; y <= y_2; ++y)
			for (int x = x_1; x <= x_2; ++x)
			{
				// работаем только с открытым пространством и скипаем стены
				if (game_map -> data[y][x].type == WALL_CELL)
					continue;
				
				// работаем только с пустыми клетками (где нет других предметов / юнитов)
				if (game_map -> data[y][x].unit != NULL || game_map -> data[y][x].item != NULL)
					continue;
				
				suitable_cells_cnt += 1;
			};

		if (suitable_cells_cnt == 0)
			continue; // TODO: стирать объект из списка предметов
		
		//printf("В блоке %d суммарно %d клеток, где можно разместить объект.\n", i, suitable_cells_cnt);

		int ind = rand() % suitable_cells_cnt;
		int now_ind = 0;
		
		// проходим по блоку
		for (int y = y_1; y <= y_2; ++y)
			for (int x = x_1; x <= x_2; ++x)
			{
				// работаем только с открытым пространством и скипаем стены
				if (game_map -> data[y][x].type == WALL_CELL)
					continue;
				
				// работаем только с пустыми клетками (где нет других предметов / юнитов)
				if (game_map -> data[y][x].unit != NULL || game_map -> data[y][x].item != NULL)
					continue;
				
				if (ind == now_ind)
				{
					if (type == 'i')
						game_map -> data[y][x].item = &(game_map -> items_list[i]);
					else
					{
						game_map -> data[y][x].unit = &(game_map -> units_list[i]);
						// также сохраняем юниту координаты, куда его разместили
						game_map -> units_list[i].x = x;
						game_map -> units_list[i].y = y;
					};
					//printf("Объект %d -- на (%d, %d)\n", i, x, y);
				};
				++now_ind;
			};
	};
	
};

/**
 * @brief генерирует и размещает монстров и предметы на игровой карте
 * @param game_map игровая карта (с готовым ландшафтом)
 * @return код ошибки
 */
int generate_maps_content(GameMap *game_map)
{
	// проверяем, что нам не навалилил нулевых указателей
	if (game_map == NULL)
		return EMPTY_POINTER;

	// проверяем, что карта была инициализирована
	if (game_map -> data == NULL || game_map -> units_list == NULL || game_map -> items_list == NULL)
		return MAP_ALREADY_DELETED;
	
	// создаём монстров и предметы (списки)

	generate_monsters(game_map -> units_list, game_map -> units_num, game_map -> level);
	generate_loot(game_map -> items_list, game_map -> items_num, game_map -> level);

	_place_objects_on_map(game_map, game_map -> items_num, 'i');
	_place_objects_on_map(game_map, game_map -> units_num, 'u');

	// ищем первого размещённого моба и делаем его игроком
	for (int i = 0; i < game_map->units_num; ++i)
	{
		if (!_unit_on_map(game_map, game_map -> units_list+i))
			continue;
		PLAYER_INDEX = i;
		break;
	}
	generate_player(game_map -> units_list + PLAYER_INDEX, "Elpatio");
	
	return OK;
};

/***
	@brief перемещает юнита на одну клетку в указанном направлении
	@param game_map карта, где находится юнит
	@param ind индекс юнита в списке юнитов
	@param dir направление, куда перемещаться: 'l' -- влево, 'r' -- вправо, 'u' -- вверх, 'd' -- вниз, 's' -- стоять.
	@return код ошибки
*/
int _move_unit(GameMap *game_map, int ind, char dir)
{	
	if (!_unit_on_map(game_map, game_map -> units_list + ind))
		return UNIT_IS_DIED;

	// проверяем, что направление, куда пытаются переместить юнита, корректное
	if (dir != 'l' && dir != 'r' && dir != 'u' && dir != 'd' && dir != 's')
		return INVALID_DIRECTION;

	if (dir == 's')	// если стоим -- ничего не меняется; так не будем же лишний раз ничего менять
		return NO_ERRORS;

	int x, y;	// текущие координаты юнита
	x = game_map -> units_list[ind].x;
	y = game_map -> units_list[ind].y;

	if (dir == 'l')	// влево
		x = x - 1;

	if (dir == 'r')	// вправо
		x = x + 1;

	if (dir == 'u')	// вверх
		y = y - 1;

	if (dir == 'd')	// вниз
		y = y + 1;

	// если упёрлись в край карты
	if (!_coords_are_correct(game_map, x, y))
		return MOVE_IS_IMPOSSIBLE;
	
	// если упёрлись в стену
	if (game_map -> data[y][x].type == WALL_CELL)
		return MOVE_IS_IMPOSSIBLE;
	
	// ежели мы упёрлись в иного юнита
	if (game_map -> data[y][x].unit != NULL)
	{
		if (game_map -> data[y][x].unit -> unit_type != PLAYER && game_map -> units_list[ind].unit_type != PLAYER)
			return MOVE_IS_IMPOSSIBLE;
		// атакуем-с
		attack(game_map -> units_list + ind, game_map -> data[y][x].unit);
		_resresh_is_live(game_map, game_map -> data[y][x].unit);	// проверяем, не умер ли атакуемый
		return OK;
	};

	// встаём на новую клетку
	int err_code = _bind_unit_to_cell(game_map, ind, x, y);

	if (err_code == OK)
	{
		// если нет предмета -- скипаем
		if (game_map -> data[y][x].item == NULL)
			return OK;

		// если ходил не игрок -- скипаем (т.к. подымать предметы может только игрок)
		if (ind != PLAYER_INDEX)
			return OK;

		// поднимаем предмет	
		add_to_inventory(&game_map -> units_list[ind], *game_map -> data[y][x].item);	// добавляем в инвертарь
		game_map -> data[y][x].item = NULL;	// отвязываем предмет от клетки
	};

	// и возвращаем результаты оного действия
	return err_code;
};

/***
	@brief перемещает игрока на одну клетку в указанном направлении
	@param game_map карта, где находится игрок
	@param dir направление, куда перемещаться: 'l' -- влево, 'r' -- вправо, 'u' -- вверх, 'd' -- вниз, 's' -- стоять.
	@return код ошибки
*/
int move_player(GameMap *game_map, char dir)
{
	// проверяем, что нам не навалили нулевых указателей
	if (game_map == NULL)
		return EMPTY_POINTER;
	
	// проверяем, что не дали неинициализированную карту 
	if (game_map -> data == NULL || game_map -> units_list == NULL || game_map -> items_list == NULL)
		return MAP_ALREADY_DELETED;
	
	int err_code = _move_unit(game_map, PLAYER_INDEX, dir);
	if (err_code != NO_ERRORS)
		return err_code;	// если ошибки -- возвращаем
	
	// если же всё прошло без ошибок, то надо проверить одну животрепещущую ситуацию, а именно -- 
	// не оказался ли игрок на переходе на след. уровень.
	int x = game_map -> units_list[PLAYER_INDEX].x;
	int y = game_map -> units_list[PLAYER_INDEX].y;

	// если мы попали не на клетку перехода -- то по барабану, на какую: возвращаем ОК
	if (game_map -> data[y][x].type != FINISH_CELL)
		return OK;
	
	// здесь мы, получается, встали на клетку перехода на следующий уровень
	_next_level(game_map);
	return OK;
	
};

/******
	@brief отвязывает юнита от клетки на карте 
	@param game_map карта, где происходит действие
	@param ind индекс юнита в массиве units_list
	@return код ошибки	
*/
int _unbind_unit_from_cell(GameMap *game_map, int ind)
{

	// TODO: добавить проверку, что ind корректный

	// получаем координаты клетки, где стоит юнит
	int x = game_map -> units_list[ind].x;
	int y = game_map -> units_list[ind].y;

	// если юнит нигде не стоит, то он и так не привязан к клетке
	if (x < 0 && y < 0)
		return NO_ERRORS;

	// отвязываем его от этой клетки
	game_map -> data[y][x].unit = NULL;

	// теперь юнит нигде не стоит
	game_map -> units_list[ind].x = -1;
	game_map -> units_list[ind].y = -1;

	return NO_ERRORS;
};

/******
	@brief привязывает юнита к клетке на карте 
	@param game_map карта, где происходит действие
	@param ind индекс юнита в массиве units_list
	@param x координата по х клетки, к которой надо привязать юнита
	@param y координата по y клетки, к которой надо привязать юнита
	@return код ошибки	
*/
int _bind_unit_to_cell(GameMap *game_map, int ind, int x, int y)
{
	// проверяем, что клетка, куда привязываем юнита, существует
	if (!_coords_are_correct(game_map, x, y))
		return MOVE_IS_IMPOSSIBLE;

	// проверяем, что клетка, куда привязываем юнита, пустая
	if (game_map -> data[y][x].unit != NULL)
		return CELL_IS_BUSY;
	
	// TODO: добавить проверку на некорректный индекс

	// на всякий случай отвязываем юнита от той клетки, где он якобы стоит
	_unbind_unit_from_cell(game_map, ind);

	// и привязываем к указанной клетке
	game_map -> data[y][x].unit = &(game_map -> units_list[ind]);
	
	
	// и меняем юниту координаты
	game_map -> units_list[ind].y = y;
	game_map -> units_list[ind].x = x;

	return NO_ERRORS;
};


const int _RADIUS_OF_SIGHT = 4;	// радиус взора для монстров
/***
	@brief определяет, может ли юнит увидеть игрока
	@param game_map игровая карта, где происходит действие
	@param ind индекс монстра
	@param ans 1, если может, и 0, если не может
	@return код ошибки
	TODO: сделать приватной (вынести в Map.c)
*/
int _can_see_player(GameMap *game_map, int ind, char *ans)
{
	if (!_index_is_correct(game_map, ind, 'u'))
		return INVALID_INDEX;

	// считаем координаты игрока
	int x1 = game_map -> units_list[PLAYER_INDEX].x;
	int y1 = game_map -> units_list[PLAYER_INDEX].y;

	// считаем координаты юнита
	int x2 = game_map -> units_list[ind].x;
	int y2 = game_map -> units_list[ind].y;

	// считаем расстояние между юнитом и игроком
	float d = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	
	// если расстояние меньше, чем радиус, то 1, иначе 0
	*ans = ((int)d < _RADIUS_OF_SIGHT);

	return NO_ERRORS;
};

/********
	@brief определяет, является ли индекс предмета / монстра корректным
	@param game_map карта, где происходит действие
	@param ind индекс предмета / монстра
	@param type 'u', если юнит, 'i', если предмет
	@return 1, если индекс корректен, и 0 -- в обратном случае
*/
int _index_is_correct(GameMap *game_map, int ind, char type)
{
	int max_val;
	if (type == 'u')
		max_val = game_map -> units_num;
	else
		max_val = game_map -> items_num;
	
	if (ind < 0 || ind >= max_val)
		return 0;
	
	return 1;
};


/*******
	@brief передвигает монстров на карте в сторону игрока, если игрок находится в их поле зрения
	@param game_map карта, где происходит действие
	@return код ошибки
*/
int move_monsters(GameMap *game_map)
{
	// проверяем, что не навалили нулевых указателей
	if (game_map == NULL)
		return EMPTY_POINTER;

	// проверяем, что на вход не поступила неинициализированная карта
	if (game_map -> units_list == NULL || game_map -> items_list == NULL || game_map -> data == NULL)
		return MAP_ALREADY_DELETED;

	// с единицы -- потому что нулевой -- игрок
	for (int ind = PLAYER_INDEX + 1; ind < game_map -> units_num; ++ind)
	{
		// если юнит умер или не был выставлен -- в движении он не участвует
		if (!_unit_on_map(game_map, game_map -> units_list + ind))
			continue;
		
		char not_need_to_continue; 
		_can_see_player(game_map, ind, &not_need_to_continue);
		if (!not_need_to_continue)
			continue;
		
		// TODO: снести нахуй: это для тестов
		//if (ind != 1)
		//	continue;

		//эхо-печать для тестов
		//printf("двигается монстр номер %d\n", ind);
		
		
		char direction = _cnt_direction_for_move(game_map, ind);
		_move_unit(game_map, ind, direction);
	};
	return OK;
};

// структура клетки для реализации BFS
typedef struct
{
	int dist;		// расстояние от монстра
	int bef;		// индекс предыдущей клетки
	// координаты клетки
	int x;
	int y;
}
_Direction;

/************
	@brief определяет направление, в котором должен двигаться монстр, и возвращает оное направление
	@param game_map игровая карта, где происходит действие
	@param ind индекс монстра, который ориентируется на местности
	@return символ направления: один из {'l', 'r', 'd', 'u', 's'}
*/
char _cnt_direction_for_move(GameMap *game_map, int ind)
{
	// сиё чудо работает на алгоритме BFS (поиск в ширину)
	
	// технический массив used для классической реализации BFS'a	
	char **used;

	// инициализируем used нулями
	used = malloc(sizeof(char*) * game_map -> size_y);
	for (int y = 0; y < game_map -> size_y; ++y)
	{
		used[y] = malloc(sizeof(char) * game_map -> size_x);
		for (int x = 0; x < game_map -> size_x; ++x)
			used[y][x] = 0;
	};

	// клетки, по которым попытается пройти монстр (технический массив для классического BFS)
	_Direction *dirs; 
	int dirs_size;	// длина массива dirs, 
	int real_size;	// реальная длина массива dirs (т.к. из-за особенностей
	// языка С память будем выделять с некоторым запасом)
	int now_elem;	// первый необработанный элемент в массиве dirs

	// инициализируем массив dirs
	dirs_size = 4 * _RADIUS_OF_SIGHT * _RADIUS_OF_SIGHT + 10;	// площадь круга, образуемого радиусом движения, + запас.
	real_size = 1;	// изначально такой элемент один: клетка, где стоит сам монстр
	now_elem = 0;	// первый элемент -- и есть первый необработанный
	dirs = malloc(sizeof(_Direction) * dirs_size);
	
	// клетка, где стоит сам монстр
	dirs[now_elem].x = game_map -> units_list[ind].x; // координаты берём у монстра
	dirs[now_elem].y = game_map -> units_list[ind].y;
	dirs[now_elem].dist = 0;	// а расстояние -- 0, ибо монстр на ней
	dirs[now_elem].bef = 0;		// предущей клетки не было -- ставим её саму себе в отцы

	while (now_elem < real_size)
	{
		// вынимаем координаты и расстояние из рассматриваемой клетки
		int now_x = dirs[now_elem].x;
		int now_y = dirs[now_elem].y;
		int now_dist = dirs[now_elem].dist;
		used[now_y][now_x] = 1;	// отмечаем рассматриваемую клетку как посещённую

		// объявляем координаты следующих клеток, на которые попытаемся встать
		int x[4];
		int y[4];

		x[0] = now_x;		y[0] = now_y + 1;
		x[1] = now_x;		y[1] = now_y - 1;
		x[2] = now_x + 1;	y[2] = now_y;
		x[3] = now_x - 1;	y[3] = now_y;

		for (int i = 0; i < 4; ++i)
		{
			// если можно встать на клетку (x{i}, y{i})
			// и на неё ещё не вставали
			// и она в пределах просчитываемого радиуса движения
			// и не упёрлись в стенку
			if 
			(
			_coords_are_correct(game_map, x[i], y[i])
			 &&
			!used[y[i]][x[i]]
			 &&
			(now_dist + 1 <= _RADIUS_OF_SIGHT)
			 &&
			(game_map -> data[y[i]][x[i]].type != WALL_CELL
			)
			)
			{
				++real_size; // размер увеличивается: вставляем в dirs новую клетку

				if (real_size == dirs_size)	// размер массива надо увеличить
				{
					dirs_size *= 2;		// на побольше
					dirs = realloc(dirs, dirs_size * sizeof(_Direction));
				};

				dirs[real_size - 1].x = x[i];
				dirs[real_size - 1].y = y[i];
				dirs[real_size - 1].bef = now_elem;
				dirs[real_size - 1].dist = now_dist + 1;
			};
		};

		// идём рассматривать следующую клетку
		++now_elem;
	};

	/*
	// эхо-печать
	for (int i = 0; i < real_size; ++i)
	{
		printf("%d: (%d, %d) на расстоянии %d, пришли из %d\n", 
			i,
			dirs[i].x,
			dirs[i].y,
			dirs[i].dist,
			dirs[i].bef);
	};
	char tmp;
	scanf("%c", &tmp);
	*/

	// теперь в массиве dirs лежат все клетки в пределах _RADIUS_OF_SIGHT, на которые может прийти монстр.
	// осталось только построить машрут до игрока

	// ищем клетку, на которой стоит игрок (если мы до неё дошли)
	int finish_cell = -1;	// клетка, на которой стоит игрок

	for (int i = 0; i < real_size; ++i)
	{
		if ((dirs[i].x == game_map -> units_list[PLAYER_INDEX].x) && (dirs[i].y == game_map -> units_list[PLAYER_INDEX].y))
		{
			finish_cell = i;
			break;
		};
	};

	// если мы не смогли, даже в теории, дойти до игрока -- то монстр стоит на месте
	if (finish_cell < 0)
	{
		// удаляем used
		for (int y = 0; y < game_map -> size_y; ++y)
			free(used[y]);
		free(used);

		// удаляем dirs
		free(dirs);
		
		// монстр стоит на месте
		return 's';
	};

	// если игрок оказался в клетке, до которой монстр может дойти, -- прокладываем к нему путь путём анализа массива dirs
	int now_cell = finish_cell;	// текущая клетка (идём как бы "с конца": от игрока к монстру)
	int bef_cell = finish_cell;	// предыдущая клетка
	
	char ans = 's';	// направление, которое будем возвращать (на всякий случай инициализируем "стоять")

	while (dirs[now_cell].bef != now_cell)	// пока не придём к клетке, где стоит монстр
	{
		now_cell = dirs[now_cell].bef;	// рассматриваем предыдущую клетку пути 
		// и вычисляем, куда мы сместились относительно предыдущей клетки
		if (dirs[now_cell].x < dirs[bef_cell].x)
			ans = 'l';
		else
		if (dirs[now_cell].x > dirs[bef_cell].x)
			ans = 'r';
		else
		if (dirs[now_cell].y < dirs[bef_cell].y)
			ans = 'u';
		else
		if (dirs[now_cell].y > dirs[bef_cell].y)
			ans = 'd';

		bef_cell = now_cell;	// текущая клетка становится предыдущей для следующей

		//printf("%c <- ", ans);	// эхо-печать
	};

	// итак, мы получили последний шаг от игрока к монстру (он лежит в ans).
	// чтобы он стал первым шагом от монстра к игроку -- надо инвертировать его направление.

	switch (ans)
	{
		case 'l':
		{
			ans = 'r';
			break;
		}
		case 'r':
		{
			ans = 'l';
			break;
		};
		case 'u':
		{
			ans = 'd';
			break;
		};
		case 'd':
		{
			ans = 'u';
			break;
		}
		default:
		{
			ans = 's';		// такого быть не может, но платят нам построчно, поэтому будет)))
			break;
		};
	};

	// не забываем почистить память

	// удаляем used
	for (int y = 0; y < game_map -> size_y; ++y)
		free(used[y]);
	free(used);

	// удаляем dirs
	free(dirs);

	return ans;
};

/****
 * @brief обновляет состояние юнита: если юнит умер, то он отвязывается от карты
 * @param game_map игровая карта
 * @param unit юнит, состояние которого обновляем
 * @return код ошибки
*/
int _resresh_is_live(GameMap *game_map, Unit *unit)
{
	// ищем юнита в списке юнитов
	int ind;	// индекс юнита
	for (ind = 0; ind < game_map -> units_num; ++ind)
	{
		if (game_map -> units_list + ind == unit)
			break;
	};
	// если юнит уже помер -- отвязываем его от клетки
	if (game_map -> units_list[ind].hp <= 0)
		_unbind_unit_from_cell(game_map, ind);
	return OK;
};

/****
 * @brief проверяет, что юнит выставлен на карте (т.е. -- живой и может двигаться)
 * @param game_map игровая карта
 * @param unit юнит
 * @return 0, если не выставлен, 1, если выставлен
*/
int _unit_on_map(GameMap *game_map, Unit *unit)
{
	int x = unit -> x;
	int y = unit -> y;
	return _coords_are_correct(game_map, x, y);
};

/********
 * @brief переводит игрока на след. уровень
 * @param game_map игровая карта
 * @return код ошибки
*/
int _next_level(GameMap *game_map)
{
	//freopen("log.txt", "w", stdout);

	MapSettings next_level_settings;
	next_level_settings.size_x = game_map -> size_x;
	next_level_settings.size_y = game_map -> size_y;
	next_level_settings.level = game_map -> level + 1;	// след. уровень

	printf("LOG OPENED\n");

	// и заново всё пересчитываем для следующего уровня
	
	// сохраняем игрока во временную переменную
	Unit tmp_player;	tmp_player.name = NULL;
	_copy_unit(game_map -> units_list + PLAYER_INDEX, &tmp_player); 

	delete_map(game_map);	// стираем карту

	// и создаём новый уровень
	init_map(game_map, next_level_settings);
	generate_maps_landscape(game_map);
	generate_maps_content(game_map);

	// загружаем игрока из временной переменной
	_copy_unit(&tmp_player, game_map -> units_list + PLAYER_INDEX);

	//fclose(stdout);
};

/**
 * @brief проверяет, окончилась ли игра на игровой карте
 * @param game_map игровая карта
 * @param is_finished сюда вернётся 1, если игра закончилась, и 0 в противном случае
 * @return код ошибки
 */
int game_is_finished(GameMap *game_map, char *is_finished)
{
	// проверяем, что не навалилили нулевых указателей
	if (game_map == NULL || is_finished == NULL)
		return EMPTY_POINTER;
	// проверяем, что не дали неинициализированную карту
	if (game_map -> data == NULL || game_map -> units_list == NULL || game_map -> items_list == NULL)
		return MAP_ALREADY_DELETED;
	
	// предположим, что игра закончилась
	*is_finished = 0;

	// когда это не так?
	// случай 1: игрок мёртв.
	// случай 2: level > max_level

	// случай 1
	if (_unit_on_map(game_map, game_map -> units_list + PLAYER_INDEX) == 0)
		*is_finished = 1;
	//printf("HUI\n");
	// случай 2
	if (game_map -> level > MAX_LEVEL)
		*is_finished = 1;

	return OK;
};

/*********
 * @brief создаёт копию юнита 
 * @param orig оригинал юнита
 * @param new_unit куда копировать
 * @return копия юнита
*/
void _copy_unit(Unit *orig, Unit *new_unit)
{
	
	// координаты мы ни в коем случае не копируем: они никак не зависят от того, где юнит закончил пред. уровенЬ!!
	//new_unit -> x = orig -> x;
	//new_unit -> y = orig -> y;
	
	new_unit -> miss_chance = orig -> miss_chance;
	new_unit -> lvl = orig -> lvl;
	new_unit -> kills = orig -> kills;
	new_unit -> hp = orig -> hp;
	new_unit -> dmg = orig -> dmg;
	new_unit -> defense = orig -> defense;
	new_unit -> name = orig -> name;
	new_unit -> unit_type = orig -> unit_type;
	new_unit -> inventory = orig -> inventory;
	new_unit -> equipped_slots = orig -> equipped_slots;
};
