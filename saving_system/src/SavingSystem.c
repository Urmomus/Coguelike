#include "SavingSystem.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/****
 * @brief сохраняет таблицу лидеров в файл
 * @param table таблица лидеров
 * @param filename название файла
 * @return код ошибки
*/
int save_table_to_file(TableOfLeaders *table, char *filename)
{
	// открываем файл со статистикой
	FILE *f_out = fopen(filename, "w");

	// сначала сохраняем кол-во чемпионов
	fprintf(f_out, "%d\n", table -> num_of_leaders);
	// а потом -- всех их подряд
	for (int i = 0; i < table -> num_of_leaders; ++i)
	{
		fprintf(f_out, "%s\n", table -> data[i].name);
		fprintf(f_out, "%d\n", table -> data[i].kills);
		fprintf(f_out, "%d\n", table -> data[i].level);
		fprintf(f_out, "%d\n", table -> data[i].items_num);
	};

	// закрываем файл со статистикой
	fclose(f_out);
};

/*****
 * @brief загружает таблицу лидеров из файла
 * @param table таблица лидеров
 * @param filename название файла
 * @return код ошибки
*/
int load_table_from_file(TableOfLeaders *table, char *filename)
{
	// открываем файл со статистикой
	FILE *f_in = fopen(filename, "r");

	// считываем кол-во игроков в таблице и выделяем память
	fscanf(f_in, "%d", &table -> num_of_leaders);
	table -> data = malloc(sizeof(Leader) * table -> num_of_leaders);

	for (int i = 0; i < table -> num_of_leaders; ++i)
	{
		char tmp[256];
		fscanf(f_in, "%s", tmp);

		table -> data[i].name = malloc(strlen(tmp) * sizeof(char));
		strcpy(table -> data[i].name, tmp);
		fscanf(f_in, "%d", &table -> data[i].kills);
		fscanf(f_in, "%d", &table -> data[i].level);
		fscanf(f_in, "%d", &table -> data[i].items_num);
	};

	// закрываем файл со статистикой
	fclose(f_in);
	return 0;
};

/*****
 * @brief очищает память из-под таблицы лидеров
 * @param table таблица лидеров
 * @return код ошибки
*/
int delete_table(TableOfLeaders *table)
{
	for (int i = 0; i < table -> num_of_leaders; ++i)
		free(table -> data[i].name);
	free(table -> data);
	table -> data = NULL;
	return 0;
};

/******
 * @brief добавляет игрока во статистику
 * @param table статистика
 * @param unit игрок
 * @return код ошибки
*/
int add_player_to_table(TableOfLeaders *table, Unit *player)
{
	table -> num_of_leaders += 1;	// увеличиваем кол-во игроков в таблице на одного
	table -> data = realloc(table -> data, table -> num_of_leaders * sizeof(Leader));	// перевыделяем память
	int last = table -> num_of_leaders - 1;	// получаем индекс, где хранится последний игрок

	// добавляем игрока в таблицу, причём последним
	table -> data[last].kills = player -> kills;
	table -> data[last].level= player -> lvl;
	table -> data[last].items_num = player -> inventory.current_size;
	table -> data[last].name = malloc(sizeof(char) * strlen(player -> name));
	strcpy(table -> data[last].name, player -> name);

	return 0;
};
