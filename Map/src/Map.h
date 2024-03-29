#ifndef MAP_H
#define MAP_H

// сия библиотека описывает глобальную карту, где происходит действие игры

#include "Cell.h"
#include "units.h"
#include "items.h"

// структуры

/**
 * @brief структура настроек для генерации карты
 * @param size_x размер карты по оси оХ
 * @param size_y размер карты по оси оУ
*/
typedef struct
{
	int size_x;				// размер карты по оси oX
	int size_y;				// размер карты по оси oY
	int level;				// уровень (порядковый номер этажа, на котором находится игрок)
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
	
	int units_num;		// кол-во мобов, что есть на карте
	Unit *units_list;	// список всех мобов, что есть на карте

	int items_num;		// кол-во предметов, что есть на карте
	Item *items_list;	// список всех предметов
} GameMap;

// функции

/**
 * @brief генерирует ландашфт игровой карты (на основе алгоритма игры "жизнь")
 * @param game_map указатель на карту, ландшафт которой будем генерировать
 * @return код ошибки
 */
int generate_maps_landscape(GameMap *game_map);

/**
 * @brief генерирует и размещает монстров и предметы на игровой карте
 * @param game_map игровая карта (с готовым ландшафтом)
 * @return код ошибки
 */
int generate_maps_content(GameMap *game_map);

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

/***
	@brief перемещает игрока на одну клетку в указанном направлении
	@param game_map карта, где находится игрок
	@param dir направление, куда перемещаться: 'l' -- влево, 'r' -- вправо, 'u' -- вверх, 'd' -- вниз, 's' -- стоять.
	@return код ошибки
*/
int move_player(GameMap *game_map, char dir);

/***
	@brief определяет, может ли юнит увидеть игрока
	@param game_map игровая карта, где происходит действие
	@param ind индекс монстра
	@param ans 1, если может, и 0, если не может
	@return код ошибки
	TODO: сделать приватной (вынести в Map.c)
*/
int _can_see_player(GameMap *game_map, int ind, char *ans);

/*******
	@brief передвигает монстров на карте в сторону игрока, если игрок находится в их поле зрения
	@param game_map карта, где происходит действие
*/
void move_monsters(GameMap *game_map);

#endif
