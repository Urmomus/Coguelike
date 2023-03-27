#include <stdio.h>
#include <stdlib.h>

#include "Map.h"

int main(void)
{
	GameMap game_map;
	
	MapSettings settings;
	settings.size_x = 100;
	settings.size_y = 100;
	
	// пока нет функций -- зададим вручную
	init_map(&game_map, settings);
	
	int err_code = generate_maps_landscape(&game_map);
	
	char *filename = "test_map.txt";
	FILE *f_out = fopen(filename, "w");
	
	fprintf(f_out, "%d\n%d\n", game_map.size_y, game_map.size_x);
	for (int i = 0; i < game_map.size_y; ++i)
		for (int j = 0; j < game_map.size_x; ++j)
			fprintf(f_out, "%d\n", game_map.data[i][j].type);
	
	fclose(f_out);
	delete_map(&game_map);
	
	return err_code;
};
