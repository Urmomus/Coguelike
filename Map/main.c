#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "Map.h"
#include <time.h>

void print_map(GameMap *game_map);

int main(void)
{
	// создаём карту
	GameMap game_map;
	
	// подключаем случайные числа
	srand(1);

	// задаём настройки карты
	MapSettings settings;
	settings.size_x = 50;
	settings.size_y = 50;
	settings.level = 4;

	// инициализируем карту
	init_map(&game_map, settings);
	// генерируем поверхность карты
	int err_code = generate_maps_landscape(&game_map);
	if (err_code)
	{
		printf("код ошибки: %d\n\n", err_code);
		return err_code;
	};
	
	// генерируем мобов & предметы
	err_code = generate_maps_content(&game_map);
	if(err_code)
	{
		printf("код ошибки: %d\n\n", err_code);
		return err_code;
	};

	// основной цикл
	while (true)
	{
		system("clear");
		char cmd;
		print_map(&game_map);
		scanf("%c", &cmd);

		// выход из игры
		if (cmd == 'e')
			return 0;
		
		// вверх
		if (cmd == 'w')
			err_code = move_player(&game_map, 'u');

		// влево
		if (cmd == 'a')
			err_code = move_player(&game_map, 'l');
			
		// вниз
		if (cmd == 's')
			err_code = move_player(&game_map, 'd');

		// вправо
		if (cmd == 'd')
			err_code = move_player(&game_map, 'r');

		if(err_code)
		{
			printf("код ошибки: %d\n\n", err_code);
			return err_code;
		};
	};
	


	// удаляем карту
	err_code = delete_map(&game_map);
	if (err_code)
	{
		printf("код ошибки: %d\n\n", err_code);
		return err_code;
	};

	return 0;
};

void print_map(GameMap *game_map)
{
	for (int y = 0; y < game_map -> size_y; ++y)
	{
		for (int x = 0; x < game_map -> size_x; ++x)
		{
			if (game_map -> data[y][x].type == WALL_CELL)
			{
				printf("\033[37;40m# ");
				continue;
			};
			
			if (game_map -> data[y][x].unit == NULL)
				printf("\033[37;40m. ");
			else
				if (game_map -> data[y][x].unit -> unit_type == PLAYER)
					printf("\033[40;31m@ ");
				else
					printf("\033[40;35mG");
		};
		printf("\n");
	};
};