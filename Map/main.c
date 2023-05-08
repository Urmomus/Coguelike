#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "Map.h"
#include <time.h>

int main(void)
{
	GameMap game_map;
	
	srand(1);

	MapSettings settings;
	settings.size_x = 50;
	settings.size_y = 50;
	settings.level = 4;

	// пока нет функций -- зададим вручную
	init_map(&game_map, settings);

	int err_code = generate_maps_landscape(&game_map);
	if (err_code)
	{
		printf("код ошибки: %d\n\n", err_code);
		return err_code;
	};
	
	err_code = generate_maps_content(&game_map);
	if(err_code)
	{
		printf("код ошибки: %d\n\n", err_code);
		return err_code;
	};

	// сохраняем ландшафт карты в "test_map.txt"
	char *filename = "test_map.txt";
	FILE *f_out = fopen(filename, "w");
	
	fprintf(f_out, "%d\n%d\n", game_map.size_y, game_map.size_x);
	for (int y = 0; y < game_map.size_y; ++y)
		for (int x = 0; x < game_map.size_x; ++x)
			fprintf(f_out, "%d\n", game_map.data[y][x].type);

	fclose(f_out);
	
	// сохраняем объекты на карте в "objects.txt"
	char *filename_2 = "objects.txt";
	f_out = fopen(filename_2, "w");

	for (int y = 0; y < game_map.size_y; ++y)
		for (int x = 0; x < game_map.size_x; ++x)
		{
			// работаем только со случаем, когда на клетке есть предмет
			if (game_map.data[y][x].item != NULL)
				fprintf(f_out, "%d\n%d\n%s\n", x, y, "item");
			if (game_map.data[y][x].unit != NULL)
				fprintf(f_out, "%d\n%d\n%s\n", x, y, "unit");
		}

	fclose(f_out);
	
	// удаляем карту
	err_code = delete_map(&game_map);
	if (err_code)
	{
		printf("код ошибки: %d\n\n", err_code);
		return err_code;
	};

	return 0;
};
