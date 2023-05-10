#include "Map.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h> // стереть
#include <math.h>

// если имя функции начинается с нижнего подчёркивания, то она исключительно внутрифайловая,
// то есть, выставлять её прототип в .h я не буду

enum Errors			// возможные ошибки
{
	NO_ERRORS, 					// нет ошибок
	SIZE_ERROR,					// размеры карт при копировании не совпадают
	MONSTER_OR_ITEMS_LEN_ERROR, // ошибка при копировании карты: не совпадает число монстров / предметов
	INVALID_DIRECTION,			// некорректный символ направления (должен быть: 'u', 'd', 'l', 'r')
	MOVE_IS_IMPOSSIBLE,			// нельзя переместить юнита (в стену или за край карты)
	CELL_IS_BUSY,				// на клетке, куда пытаются привязать юнита, уже что-то / кто-то есть
	INVALID_INDEX,				// ошибка индексации при обращении к units_list или к items_list
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


// константы

const int PLAYER_INDEX = 0;							// индекс, под которым в массиве units_list расположен игрок. 
LandsapeSettings _std_settings = {36, 3, 4, 5};		// дефолтные настройки для генерации ландшафта карты

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
	@param dir направление, куда перемещаться: 'l' -- влево, 'r' -- вправо, 'u' -- вверх, 'd' -- вниз.
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

/****
	@brief инициализирует карту начальными значениями
	@param game_map карта
	@param settings настройки, из которых возьмутся начальные значения
	@return код ошибки  
*/
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
	
	// изначально ни один из юнитов нигде не стоит
	for (int i = 0; i < game_map -> units_num; ++i)
	{
		game_map -> units_list[i].x = game_map -> units_list[i].y = -1;
	};

	// выделяем память под клетки карты
	game_map -> data = malloc(sizeof(Cell*) * game_map -> size_y);
	for (int y = 0; y < game_map -> size_y; ++y)
		game_map -> data[y] = malloc(sizeof(Cell) * game_map -> size_x);
	
	return NO_ERRORS;
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
	return NO_ERRORS;
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
	// создаём монстров и предметы (списки)

	//generate_monsters(&game_map -> units_list, game_map -> units_num, game_map -> level);
	//generate_loot(&game_map -> items_list, game_map -> items_num, game_map -> level);

	// пока заколхожу, что все монстры -- гоблины, а игрок -- под индексом PLAYER_INDEX
	for (int i = 0; i < game_map -> units_num; ++i)
		game_map -> units_list[i].unit_type = GOBLIN;
	game_map -> units_list[PLAYER_INDEX].unit_type = PLAYER;

	_place_objects_on_map(game_map, game_map -> items_num, 'i');
	_place_objects_on_map(game_map, game_map -> units_num, 'u');
	return OK;
};

/***
	@brief перемещает юнита на одну клетку в указанном направлении
	@param game_map карта, где находится юнит
	@param ind индекс юнита в списке юнитов
	@param dir направление, куда перемещаться: 'l' -- влево, 'r' -- вправо, 'u' -- вверх, 'd' -- вниз.
	@return код ошибки
*/
int _move_unit(GameMap *game_map, int ind, char dir)
{
	// проверяем, что направление, куда пытаются переместить юнита, корректное
	if (dir != 'l' && dir != 'r' && dir != 'u' && dir != 'd')
		return INVALID_DIRECTION;

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
	
	if (game_map -> data[y][x].unit != NULL)
	{
		// здесь будет атака
		// TODO
	};

	// встаём на новую клетку
	int err_code = _bind_unit_to_cell(game_map, ind, x, y);

	// и возвращаем результаты оного действия
	return err_code;
};

/***
	@brief перемещает игрока на одну клетку в указанном направлении
	@param game_map карта, где находится игрок
	@param dir направление, куда перемещаться: 'l' -- влево, 'r' -- вправо, 'u' -- вверх, 'd' -- вниз.
	@return код ошибки
*/
int move_player(GameMap *game_map, char dir)
{
	// игрок -- это просто юнит под индексом PLAYER_INDEX
	return _move_unit(game_map, PLAYER_INDEX, dir);
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
	game_map -> units_list[ind].x = x;
	
	// и меняем юниту координаты
	game_map -> units_list[ind].y = y;
	
	return NO_ERRORS;
};


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
	const int RADIUS_OF_SIGHT = 4;

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
	*ans = ((int)d < RADIUS_OF_SIGHT);

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
*/
void move_monsters(GameMap *game_map)
{
	// с единицы -- потому что нулевой -- игрок
	for (int ind = 1; ind < game_map -> units_num; ++ind)
	{
		char not_need_to_continue; 
		_can_see_player(game_map, ind, &not_need_to_continue);
		if (!not_need_to_continue)
			continue;
		
		printf("двигается монстр номер %d\n", ind);
	};
};