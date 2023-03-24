// сия библиотека описывает <клетку игрового поля>

//#include "Units.h"
//#include "Items.h"

// описывает возможные типы клеток

#include "Object.h"

enum CellType
{
	FREE_CELL, // проходимая клетка
	WALL_CELL, // непроходимая клетка (стена)
};

// класс, описывающий одну клетку (Cell) игрового поля
typedef struct
{
	char type; 		// тип клетки. принимает значения из CellType
	Object* object;	// моб / предмет, стоящий на клетке
} Cell;
