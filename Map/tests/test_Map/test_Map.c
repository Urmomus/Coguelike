#include "Map.h"
#include "test_Map.h"
#include "ErrorCodes.h"
#include <stdlib.h>
#include <stdio.h>

// СООБЩЕНИЯ ОБ ОШИБКАХ

// ошибки для init_map
char* INIT_MAP_ERROR_WITH_SIZE = "Ошибка в init_map: создалась карта некорректного размера!\n";
char* INIT_MAP_ERROR_WITH_NULL = "Ошибка в init_map: функция отработала для нулевого указателя!\n";
char* INIT_MAP_ERROR_WITH_LEVEL = "Ошибка в init_map: создался некорректный уровень!\n";
char *INIT_MAP_CANT_GENERATE = "Ошибка в init_map: карта не создалася, хотя должна была бы!\n";
char *INIT_MAP_ERROR_ALREADY_EXISTS = "Ошибка в init_map: карта была повторно инициализирована!\n";
char *INIT_MAP_DIDNT_GENERATE = "Ошибка в init_map: карта не инициализируется, это лишь иллюзия!\n";
char *INIT_MAP_ERROR_WITH_SETTINGS = "Ошибка в init_map: при инициализации настройки скопировались коряво!\n";

// ошибки для game_is_finished
char* GAME_IS_FINISHED_ERROR_WITH_NULL = "Ошибка в game_is_finished: функция отработала для нулевого указателя!\n";


// ошибки для get_map_state
char* GET_MAP_STATE_ERROR_WITH_NULL = "Ошибка в get_map_state: функция отработала для нулевого указателя!\n";


// ошибки для set_map_state
char* SET_MAP_STATE_ERROR_WITH_NULL = "Ошибка в set_map_state: функция отработала для нулевого указателя!\n";


// ошибки для delete_map
char* DELETE_MAP_ERROR_MAP_NOT_DELETED = "Ошибка в delete_map: карта не удалилась!\n";
char* DELETE_MAP_ERROR_WITH_NULL = "Ошибка в delete_map: функция отработала для нулевого указателя!\n";


// приватные функции

int _test_game_is_finished(char **message);
int _test_get_map_state(char **message);
int _test_set_map_state(char **message);
int _test_init_map(char **message);
int _test_delete_map(char **message);
int _test_generate_map_landscape(char **message);

int _test_generate_map_landscape(char **message)
{
    return 0;
};

/***********
/* @brief тест на функцию game_is_finished
/* @return 0, если корректно, 1, если ошибка
*/
int _test_game_is_finished(char **message)
{
    return 0;
};

/***********
/* @brief тест на функцию get_map_state
/* @return 0, если корректно, 1, если ошибка
*/
int _test_get_map_state(char **message)
{
    return 0;
};

/***********
/* @brief тест на функцию set_map_state
/* @return 0, если корректно, 1, если ошибка
*/
int _test_set_map_state(char **message)
{
    return 0;
};

/***********
/* @brief тест на функцию init_map
/* @return 0, если корректно, 1, если ошибка
*/
int _test_init_map(char **message)
{
    MapSettings map_1_settings;
 
     // проверяем, что если передать в init_map передать NULL, то произойдёт ошибка
    if (init_map(NULL, map_1_settings) != EMPTY_POINTER)
    {
        *message = INIT_MAP_ERROR_WITH_NULL;
        return 1;
    };

    GameMap map_1;

    // теперь проверяем, что карта не инициализируется, если в ней уже есть какая-то информация
    map_1.data = malloc(777);
    if (init_map(&map_1, map_1_settings) != MAP_ALREADY_EXISTS)
    {
        *message = INIT_MAP_ERROR_ALREADY_EXISTS;
        return 1;
    };
    free(map_1.data);
    map_1.data = NULL;

    // проверяем, что при попытке создать карту некорректного размера происходит ошибка
    map_1_settings.size_x = 0;
    map_1_settings.size_y = 5;

    // проверяем, что карта не создастся, ежели размер некорректен
    if (init_map(&map_1, map_1_settings) != INVALID_MAP_SIZE)
    {
        *message = INIT_MAP_ERROR_WITH_SIZE;
        return 1;
    };

    map_1_settings.size_x = 5;
    map_1_settings.size_y = -1;

    // то же самое, но по другой координате
    if (init_map(&map_1, map_1_settings) != INVALID_MAP_SIZE)
    {
        *message = INIT_MAP_ERROR_WITH_SIZE;
        return 1;
    };

    // задаём карте корректные настройки генерации
    map_1_settings.size_x = 50;
    map_1_settings.size_y = 50;

    // теперь проверяем, что карта не создаётся, если некорректен номер уровня
    map_1_settings.level = 0;   // уровни нумеруются с единицы

    if (init_map(&map_1, map_1_settings) != INVALID_LEVEL)
    {
        *message = INIT_MAP_ERROR_WITH_LEVEL;
        return 1;
    };

    // теперь проверяем, что карта действительно создаётся
    map_1_settings.level = 5;   // корректный уровень
    
    // проверяем, что в случае корректных настроек карта создаётся
    if (init_map(&map_1, map_1_settings) != OK)
    {
        *message = INIT_MAP_CANT_GENERATE;
        return 1;
    };

    // проверяем, что в карте появилась какая-то информация
    if (map_1.data == NULL)
    {
        *message = INIT_MAP_DIDNT_GENERATE;
        return 1;
    };

    // проверяем, что настройки корректно скопировались в карту
    if (map_1.size_x != map_1_settings.size_x || map_1.size_y != map_1_settings.size_y || map_1.level != map_1_settings.level)
    {
        *message = INIT_MAP_ERROR_WITH_SETTINGS;
        return 1;
    }

    // т.к. ф-я delete_map ещё не оттестирована, то мы не имеем права её запускать
    // посему -- почистим память вручную, благо это не так сложно
    for (int y = 0; y < map_1.size_y; ++y)
        free(map_1.data[y]);
    free(map_1.data);

    // все тесты прошли благополучно, можем возвращать 0
    return 0;

};

/***********
/* @brief тест на функцию delete_map
/* @return 0, если корректно, 1, если ошибка
*/
int _test_delete_map(char **message)
{
    // проверяем, что корректно созданная карта корректно удаляется 
    /*
    MapSettings map_1_settings;
    GameMap map_1;

    map_1_settings.size_x = 5;
    map_1_settings.size_y = 5;

    init_map(&map_1, map_1_settings);
    delete_map(&map_1);

    if (map_1.data != NULL)
    {
        *message = DELETE_MAP_ERROR_MAP_NOT_DELETED;
        return 1;
    };

    // проверяем, что функция корректно обрабатывает случай, когда на вход не пришло корректной карты
    if (delete_map(NULL) != EMPTY_POINTER)
    {
        *message = DELETE_MAP_ERROR_WITH_NULL;
        return 1;
    };
    */
    return 0;
};

/***********
/* @brief тестирует все функции библиотеки Map
/* @return 0, если корректно, 1, если ошибка
*/
int test_Map()
{
    char *message;
    if (_test_init_map(&message) == 1)  // в процессе
    {
        printf("%s", message);
        return 1;
    };
    if (_test_delete_map(&message) == 1)
    {
        printf("%s", message);
        return 1;
    };
    if (_test_game_is_finished(&message) == 1)
    {
        printf("%s", message);
        return 1;
    };
    if (_test_get_map_state(&message) == 1)
    {
        printf("%s", message);
        return 1;
    };
    if (_test_set_map_state(&message) == 1)
    {
        printf("%s", message);
        return 1;
    };
    if (_test_generate_map_landscape(&message) == 1)
    {
        printf("%s", message);
        return 1;
    };
    return 0;
};
