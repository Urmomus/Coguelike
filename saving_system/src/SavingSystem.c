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
int save_table_to_file(TableOfLiders *table, char *filename)
{
};

/*****
 * @brief загружает таблицу лидеров из файла
 * @param table таблица лидеров
 * @param filename название файла
 * @return код ошибки
*/
int load_table_from_file(TableOfLiders *table, char *filename)
{
	FILE *f_in = fopen(filename, "r");

	fclose();
};

/*****
 * @brief очищает память из-под таблицы лидеров
 * @param table таблица лидеров
 * @return код ошибки
*/
int delete_table(TableOfLiders *table)
{

};

/******
 * @brief добавляет игрока во статистику
 * @param table статистика
 * @param unit игрок
 * @return код ошибки
*/
int add_player_to_table(TableOfLiders *table, Unit *player)
{

};