#include <stdio.h>
#include <stdlib.h>

#include "Map.h"

int main(void)
{
	GameMap game_map;
	
	MapSettings settings;
	settings.size_x = 40;
	settings.size_y = 40;
	
	// пока нет функций -- зададим вручную
	init_map(&game_map, settings);
	
	int err_code = generate_maps_landscape(&game_map);
	

	// сохраняем ландшафт карты в "test_map.txt"
	char *filename = "test_map.txt";
	FILE *f_out = fopen(filename, "w");
	
	fprintf(f_out, "%d\n%d\n", game_map.size_y, game_map.size_x);
	for (int i = 0; i < game_map.size_y; ++i)
		for (int j = 0; j < game_map.size_x; ++j)
			fprintf(f_out, "%d\n", game_map.data[i][j].type);

	fclose(f_out);

	// сохраняем объекты на карте в "objects.txt"
	char *filename_2 = "objects.txt";
	f_out = fopen(filename_2, "w");

	for (int y = 0; y < game_map.size_y; ++y)
		for (int x = 0; x < game_map.size_x; ++x)
		{
			// работаем только со случаем, когда на клетке есть предмет
			if (game_map.data[x][y].item != NULL)
				fprintf(f_out, "%d\n%d\n%s\n", x, y, "item");
			if (game_map.data[x][y].unit != NULL)
				fprintf(f_out, "%d\n%d\n%s\n", x, y, "unit");
		}

	fclose(f_out);

	delete_map(&game_map);	// удаляем карту
	return err_code;
};
