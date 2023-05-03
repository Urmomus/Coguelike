#ifndef CELL_H
#define CELL_H

#include "../../enemy_system/src/units.h"
#include "../../enemy_system/src/items.h"

// описывает возможные типы клеток
enum CellType
{
	FREE_CELL, // проходимая клетка
	WALL_CELL, // непроходимая клетка (стена)
};

// класс, описывающий одну клетку (Cell) игрового поля
typedef struct
{
	char type; 		// тип клетки. принимает значения из CellType
	Unit *unit;		// моб, стоящий на клетке (NULL, если моба нет)
	Item *item;		// предмет, лежащий на клетке (NULL, если моба нет)
} Cell;

#endif
