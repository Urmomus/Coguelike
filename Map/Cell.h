// сия библиотека описывает <клетку игрового поля>

//#include "Units.h"
//#include "Items.h"

// описывает возможные типы клеток
enum CellType
{
	FREE_CELL, // проходимая клетка
	WALL_CELL, // непроходимая клетка (стена)
};

enum ObjectTypes
{
	ITEM,		// предмет
	UNIT,		// существо
};

typedef struct 
{
	void *data;		// указатель на предмет / моба
	char type;		// элемент из ObjectTypes
} Object;

// класс, описывающий одну клетку (Cell) игрового поля
typedef struct
{
	char type; 		// тип клетки. принимает значения из CellType
	Object* object;	// моб / предмет, стоящий на клетке
} Cell;
