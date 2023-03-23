// сия библиотека описывает глобальную карту, где происходит действие игры

#include "Cell.h"
//#include "Items.h"
//#include "Units.h"

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
