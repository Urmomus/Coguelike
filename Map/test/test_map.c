#include "../Map.h"
#include "test_map.h"
#include "ErrorCodes.h"


/***********
/* @brief тест на функцию game_is_finished
/* @return 0, если корректно, 1, если ошибка
*/
int _test_game_is_finished(char **message)
{

};

/***********
/* @brief тест на функцию get_map_state
/* @return 0, если корректно, 1, если ошибка
*/
int _test_get_map_state(char **message)
{

};

/***********
/* @brief тест на функцию set_map_state
/* @return 0, если корректно, 1, если ошибка
*/
int _test_set_map_state(char **message)
{
};

/***********
/* @brief тест на функцию init_map
/* @return 0, если корректно, 1, если ошибка
*/
int _test_init_map(char **message)
{
    // проверяем, что при попытке создать карту некорректного размера происходит ошибка
    MapSettings map_1_settings;
    GameMap map_1;

    map_1_settings.size_x = 0;
    map_1_settings.size_y = 5;

    if (init_map(&map_1, map_1_settings) != INVALID_MAP_SIZE)
    {
        *message = INIT_MAP_ERROR_WITH_SIZE;
        return 1;
    };

    map_1_settings.size_x = 5;
    map_1_settings.size_y = -1;

    if (init_map(&map_1, map_1_settings) != INVALID_MAP_SIZE)
    {
        *message = INIT_MAP_ERROR_WITH_SIZE;
        return 1;
    };

    delete_map(&map_1);
    
    // проверяем, что если передать в init_map передать NULL, то произойдёт ошибка
    if (init_map(NULL, map_1_settings))
    {
        *message = 
        return 1;
    };
    return 0;

};

/***********
/* @brief тест на функцию delete_map
/* @return 0, если корректно, 1, если ошибка
*/
int _test_delete_map(char **message)
{
    // проверяем, что корректно созданная карта корректно удаляется 

    MapSettings map_1_settings;
    GameMap map_1;

    map_1_settings.size_x = 5;
    map_1_settings.size_y = 5;

    init_map(&map_1, map_1_settings);
    delete_map(&map_1);

    if (map_1.data != NULL)
    {
        *message = "Ошибка в delete_map: карта не удалилась!\n";
        return 1;
    };

    // проверяем, что функция корректно обрабатывает случай, когда на вход не пришло корректной карты
    if (delete_map(NULL) != EMPTY_POINTER)
    {
        *message = "Ошибка в delete_map: не нашла ошибку c NULL!\n";
        return 1;
    };

    return 0;
};

/***********
/* @brief тестирует все функции библиотеки Map
/* @return 0, если корректно, 1, если ошибка
*/
int test_Map(char **message)
{
    if (_test_game_is_finished(message) == 1)
        return 1;

    if (_test_get_map_state(message) == 1)
        return 1;
    
    if (_test_set_map_state(message) == 1)
        return 1;
    
    if (_test_init_map(message) == 1)
        return 1;
    
    if (_test_delete_map(message) == 1)
        return 1;
    
    return 0;
};
