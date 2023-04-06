#ifndef MAP_H
#define MAP_H

// сия библиотека описывает глобальную карту, где происходит действие игры

#include "Cell/Cell.h"
#include "../enemy_system/units.h"
#include "../enemy_system/items.h"

/**
 * @brief структура настроек для генерации карты
 * @param size_x размер карты по оси оХ
 * @param size_y размер карты по оси оУ
*/
typedef struct
{
	int size_x;				// размер карты по оси oX
	int size_y;				// размер карты по оси oY
}
MapSettings;

/**
 * @brief структура игровой карты
 * @param level номер этажа
 * @param size_x ширина карты
 * @param size_y высота карты
 * @param data двумерный массив клеток, являющий собой карту
 * @param mobs_list список всех мобов, что есть на карте
 * @param items_list список всех предметов, что есть карте
*/
typedef struct
{
	int level;			// номер этажа
	
	int size_x;			// ширина карты
	int size_y;			// высота карты
	Cell **data; 		// двумерный массив клеток, являющий собой карту
	
	Unit *mobs_list;	// список всех мобов, что есть на карте
	Item *items_list;	// список всех предметов
} GameMap;

/**
 * @brief генерирует ландашфт игровой карты (на основе алгоритма игры "жизнь")
 * @param game_map указатель на карту, ландшафт которой будем генерировать
 * @return код ошибки
 */
int generate_maps_landscape(GameMap *game_map);

/**
 * @brief размещает монстров и предметы на игровой карте
 * @param game_map игровая карта (с готовым ландшафтом)
 * @param items массив предметов
 * @param items_num кол-во предметов
 * @param units массив мобов
 * @param units_num кол-во мобов
 * @return код ошибки
 */
int place_objects_on_map(GameMap *game_map, Item *items, int items_num, Unit *units, int units_num);

/**
 * @brief обновляет состояние игровой карты
 * @param game_map игровая карта
 * @return код ошибки
 */
int renew_map_state(GameMap *game_map);

/**
 * @brief проверяет, окончилась ли игра на игровой карте
 * @param game_map игровая карта
 * @param is_finished сюда вернётся 1, если игра закончилась, и 0 в противном случае
 * @return код ошибки
 */
int game_is_finished(GameMap *game_map, char *is_finished);

/**
 * @brief возвращает указатель на игровую карту
 * @param game_map игровая карта
 * @return код ошибки
 */
GameMap* get_map_state(GameMap *game_map);

/**
 * @brief копирует одну игровую карту в другую
 * @param game_map игровая карта -- в которую будем копировать
 * @param source откуда будем копировать
 * @return код ошибки
 */
int set_map_state(GameMap *game_map, GameMap *source);

/**
 * @brief создаёт карту, выделяет память и задаёт некоторые стартовые параметры (см. в settings)
 * @param game_map указатель на игровую карту, которую надо инициализировать
 * @param settings параметры создаваемой карты
 * @return код ошибки
 */
int init_map(GameMap *game_map, MapSettings settings);

/**
 * @brief чистит память, которую занимает карта
 * @param game_map указатель на игровую карту, за которой надо почистить память
 * @return код ошибки
*/
int delete_map(GameMap *game_map);

#endif
