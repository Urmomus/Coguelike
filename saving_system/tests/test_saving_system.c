#include <stdlib.h>
#include <string.h>
#include "./saver.h"
#include "test_saving_system.h"


// СООБЩЕНИЯ ОБ ОШИБКАХ
char* MAP_SAVED = "Карта успешно сохранена";
char* MAP_LOADED = "Карта успешно загружена";
char* MAP_NOT_EXIST_SAVE_ERROR = "Ошибка при сохраненении: карта не существует!";
char* MAP_DATA_IS_NOT_CORRECT = "Ошибка при сохраненении: данные не верны!";
char* SAVE_NOT_EXIST_LOAD_ERROR = "Ошибка при загрузке: сохранения не существует";


/**
* @brief позитивный тест функции load
* @return 0 - если корректно, 1 - если ошибка
*/
int test_load(char **message) {
    MapSettings map_1_settings;
    GameMap test_map;

    map_1_settings.size_x = 5;
    map_1_settings.size_y = 5;

    init_map(&test_map, map_1_settings);

    int error_code = 0;

    GameMap loadedMap = load(test_map);

    if(loadedMap.level <= 0)
    {
        *message = MAP_NOT_EXIST_SAVE_ERROR;
    }

//    Проверить что мапа которую сохранили такая же что и создали
    if (isEquals(test_map, loadedMap)){
      error_code = 1;
    }

    if (error_code == 0){
        *message = MAP_LOADED;
        return 0;
    }
    *message = MAP_DATA_IS_NOT_CORRECT;
    return 1;
}

bool isEquals(GameMap source, GameMap assertable)
{
    if (source.level == assertable.level &&
    source.size_x == assertable.size_x &&
    source.size_y == assertable.size_y)
    {
        return true;
    }

    return false;
}

/**
* @brief позитивный тест функции save
* @return 0 - если корректно, 1 - если ошибка
*/
int test_save(char **message)
{
    MapSettings map_1_settings;
    GameMap test_map;

    map_1_settings.size_x = 5;
    map_1_settings.size_y = 5;

    init_map(&test_map, map_1_settings);

    int error_code;

    error_code = save(&test_map);
    if (error_code == 0){
        *message = MAP_SAVED;
        return 0;
    }
    return 1;
}

/**
* @brief тестирует функции saving_system
* @return 0 - если корректно, 1 - если ошибка
*/
int test_saving_system(char **message)
{
    if (test_save(message))
        return 0;
    if (test_load(message))
        return 0;
    return 1;
}