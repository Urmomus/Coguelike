#ifndef ERRORCODES_H
#define ERRORCODES_H

enum ErrorCodes
{
    NO_ERRORS, 					// нет ошибок
    EMPTY_POINTER,              // вместо game_map в функцию передали NULL
    INVALID_MAP_SIZE,           // попытались использовать карту некорректного (<= 0) размера.
    MAP_ALREADY_EXISTS,         // попытка повторно инициализировать карту
    MAP_ALREADY_DELETED,        // карта либо уже удалялась, либо не была инициализирована
	SIZE_ERROR,					// размеры карт при копировании не совпадают
	MONSTER_OR_ITEMS_LEN_ERROR, // ошибка при копировании карты: не совпадает число монстров / предметов
	INVALID_DIRECTION,			// некорректный символ направления (должен быть: 'u', 'd', 'l', 'r', 's')
	MOVE_IS_IMPOSSIBLE,			// нельзя переместить юнита (в стену или за край карты)
	CELL_IS_BUSY,				// на клетке, куда пытаются привязать юнита, уже что-то / кто-то есть
	INVALID_INDEX,				// ошибка индексации при обращении к units_list или к items_list
	UNIT_IS_DIED,				// игрок мёртв
};

#endif