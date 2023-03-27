// сия библиотека описывает глобальную карту, где происходит действие игры

#include "Cell.h"
//#include "Items.h"
//#include "Units.h"

typedef struct
{
	int size_x;				// размер карты по оси oX
	int size_y;				// размер карты по оси oY
}
MapSettings;

typedef struct
{
	int level;			// номер этажа
	
	int size_x;			// ширина карты
	int size_y;			// высота карты
	Cell **data; 		// двумерный массив клеток, являющий собой карту
	
	//Unit *mobs_list;	// список всех мобов, что есть на карте
	//Item *items_list;	// список всех предметов
} GameMap;

/**
 * @brief генерирует ландашфт игровой карты (на основе алгоритма игры "жизнь")
 * @param game_map указатель на карту, ландшафт которой будем генерировать
 * @return код ошибки
 */
int generate_maps_landscape(GameMap *game_map);

int place_objects_on_map(GameMap *game_map);

int renew_map_state(GameMap *game_map);

int game_is_finished(GameMap *game_map);

int get_map_state(GameMap *game_map);

int set_map_state(GameMap *game_map);

int init_map(GameMap *game_map, MapSettings settings);

int delete_map(GameMap *game_map);
