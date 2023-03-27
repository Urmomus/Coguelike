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
