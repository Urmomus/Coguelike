#ifndef TEST_SAVING_SYSTEM_H
#define TEST_SAVING_SYSTEM_H

#include <stdbool.h>
#include "../Map/src/Map.h"

/**
* @brief тестирует функцию save
* @return 0, если корректно, 1, если ошибка
*/
int test_save(char **message);

/**
* @brief тестирует функцию save
* @return 0, если корректно, 1, если ошибка
*/
int test_load(char **message);

/**
* @brief сравнивает две структуры карты
* @param source тестовая структура карта
* @param assertable сравниваемая структура карта полученная после загрузки
* @return 0, если корректно, 1, если ошибка
*/
bool isEquals(GameMap source, GameMap assertable);

int test_saving_system(char **message);

#endif