#ifndef SAVER_H
#define SAVER_H

#include "../Map/Map.h"

// Сохранение данных карты и возврат по запросу стейта

/**
 * @brief возврат состояния карты по запросу
 * @param map_data структура карта
*/
GameMap load(GameMap map_data);

/**
 * @brief сохранение состояния карты
 * @param map_data структура карта
*/
void save(GameMap map_data);

#endif