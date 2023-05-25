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
char* DELETE_MAP_DELETED_NON_INITIALIZED_MAP = "Ошибка в delete_map: функция отработала для неинициализированной карты!\n";
char* DELETE_MAP_CANT_DELETE = "Ошибка в delete_map: функция отказалась работать на корректных входных данных!\n";
char* DELETE_MAP_DIDNT_DELETE = "Ошибка в delete_map: функция отработала, но не почистила память!\n";

// ошибки для generate_maps_landscape
char *GENERATE_MAPS_LANDSCAPE_WORKS_WITH_NULL = "Ошибка в generate_maps_landscape: функция отработала для нулевого указателя!\n";
char *GENERATE_MAPS_LANDSCAPE_WORKS_WITH_NON_INITIALIZED_MAP = "Ошибка в generate_maps_landscape: функция отработала для неинициализированной либо удалённой карты!\n";
char *GENERATE_MAPS_LANDSCAPE_CANT_GENERATE_LANDSCAPE = "Ошибка в generate_maps_landscape: функция не отработала, хотя должна была!\n";
char *GENERATE_MAPS_LANDSCAPE_GENERATED_NON_ONE_GRAPH = "Ошибка в generate_maps_landscape: сгенерировалась карта, содержащая несколько несвязных областей!\n";
char *GENERATE_MAPS_LANDSCAPE_GENERATED_MAP_WITHOUT_NEXT_LEVEL_CELL = "Ошибка в generate_maps_landscape: сгенерировалась карта, не содержащая перехода на следующий уровень!\n";

// ошибки для generate_maps_content
char *GENERATE_MAP_CONTENT_ERROR_WITH_NULL_PTR = "Ошибка в generate_maps_content: функция отработала для nullptr!\n";
char *GENERATE_MAP_CONTENT_WORKED_FOR_NON_INITIALIZED_MAP = "Ошибка в generate_maps_content: функция отработала для неинициализированной карты!\n";
char *GENERATE_MAP_CONTENT_CANT_CREATE_CONTENT = "Ошибка в generate_maps_content: функция не отработала, хотя должна была!\n";
char *GENERATE_MAP_CONTENT_MISSED_PLAYERS_PLACEMENT = "Ошибка в generate_maps_content: функция не разместила игрока на карту!\n";

// приватные функции


/*
int _test_get_map_state(char **message);
int _test_set_map_state(char **message);
*/

int _test_game_is_finished(char **message);
int _test_init_map(char **message);                 // готова
int _test_delete_map(char **message);               // готова
int _test_generate_map_landscape(char **message);   // готова
int _test_generate_map_content(char **message);


// реализации функций

int _test_generate_map_content(char **message)
{
    // проверяем, что нулевые указатели отлавливаются
    if (generate_maps_content(NULL) != EMPTY_POINTER)
    {
        *message = GENERATE_MAP_CONTENT_ERROR_WITH_NULL_PTR;
        return 1;
    }

    GameMap game_map;
    game_map.data = NULL;
    game_map.items_list = NULL;
    game_map.units_list = NULL;
    // проверяем, что неинициализированная карта отлавливается
    if (generate_maps_content(&game_map) != MAP_ALREADY_DELETED)
    {
        *message = GENERATE_MAP_CONTENT_WORKED_FOR_NON_INITIALIZED_MAP;
        return 1;
    };

    // инициализируем карту, создаём ландшафт
    MapSettings settings = {40, 40, 3};
    //return 0;
    // проверяем для разных конфигураций
    for (int seed = 0; seed <= 15; ++seed)
        for (int level = 1; level <= 10; ++level)
        {
            //printf("CONF: %d, %d\n", seed, level);
            // даём конфигурацию
            settings.size_x = level * 10;
            settings.size_y = level * 10;
            settings.level = level;
            srand(seed);

            int err = init_map(&game_map, settings);  // функция уже оттещена
            err = generate_maps_landscape(&game_map); // функция уже оттещена

            if (generate_maps_content(&game_map) != OK)
            {
                *message = GENERATE_MAP_CONTENT_CANT_CREATE_CONTENT;
                return 1;
            }

            // проверяем, что игрок всегда выставлен
            bool player_is_placed = false;
            for (int y = 0; y < game_map.size_y; ++y)
                for (int x = 0; x < game_map.size_x; ++x)
                {
                    if (game_map.data[y][x].unit == NULL)
                        continue;
                    if (game_map.data[y][x].unit -> unit_type == PLAYER)
                    {
                        //printf("(%d, %d), %d\n", y, x, game_map.data[y][x].type == WALL_CELL);
                        player_is_placed = true;
                    };
                };

            // игрок не выставлен
            if (!player_is_placed)
            {
                *message = GENERATE_MAP_CONTENT_MISSED_PLAYERS_PLACEMENT;
                return 1;
            };
            // не забываем очистить память
            delete_map(&game_map);  // функция уже оттещена
        };
    

    return 0;
};

/********
 * @brief алгоритм dfs
 * @param game_map карта
 * @param used массив "посещено / не посещено"
 * @param x координата по х
 * @param y координата по y 
*/
void _dfs_(GameMap *game_map, char **used, int x, int y)
{
    if (x < 0 || y < 0 || x >= game_map -> size_x || y >= game_map -> size_y)
        return;
    if (used[y][x])
        return;
    if (game_map->data[y][x].type == WALL_CELL)
        return;
    used[y][x] = true;
    _dfs_(game_map, used, x+1, y);
    _dfs_(game_map, used, x-1, y);
    _dfs_(game_map, used, x, y-1);
    _dfs_(game_map, used, x, y+1);
};

/************
 * @brief считает кол-во отдельных "зон" на карте
 * @param game_map карта
 * @return кол-во отдельных "зон" на карте
*/
int cnt_places(GameMap *game_map)
{
    char **used = malloc(sizeof(char*) * game_map -> size_y);
    // чистим used'ы
    for (int y = 0; y < game_map -> size_y; ++y)
    {    
        used[y] = malloc(sizeof(char) * game_map -> size_x);
        for (int x = 0; x < game_map -> size_x; ++x)
        {
            used[y][x] = 0;
        };
    };
    // считаем кол-во "зон"
    int zones_cnt = 0;
    for (int y = 0; y < game_map -> size_y; ++y)
    {
        for (int x = 0; x < game_map -> size_x; ++x)
        {
            // стены не обрабатываем
            if (game_map -> data[y][x].type == WALL_CELL)
                continue;
            if (!used[y][x])
            {
                zones_cnt += 1;
                _dfs_(game_map, used, x, y);
            };
        };
    };

    // не забываем чистить память!
    for (int y = 0; y < game_map -> size_y; ++y)
        free(used[y]);
    free(used);
    
    return zones_cnt;
};

int _test_generate_map_landscape(char **message)
{
    GameMap game_map;

    // проверяем, что NULL обрабатывается
    if (generate_maps_landscape(NULL) != EMPTY_POINTER)
    {
        *message = GENERATE_MAPS_LANDSCAPE_WORKS_WITH_NULL;
        return 1;
    };

    // проверяем, что неинициализированная карта обрабатывается
    game_map.data = NULL;
    if (generate_maps_landscape(&game_map) != MAP_ALREADY_DELETED)
    {
        *message = GENERATE_MAPS_LANDSCAPE_WORKS_WITH_NON_INITIALIZED_MAP;
        return 1;
    };

    // для корректной инициализации -- все указатели надо занулить
    game_map.units_list = NULL;
    game_map.items_list = NULL;
    game_map.data = NULL;
    MapSettings settings = {50, 50, 1};
    init_map(&game_map, settings);  // эта функция уже оттещена

    // проверяем, что ландшафт генерируется
    if (generate_maps_landscape(&game_map) != OK)
    {
        *message = GENERATE_MAPS_LANDSCAPE_CANT_GENERATE_LANDSCAPE;
        return 1;
    };

    // чистим память
    delete_map(&game_map);  // эта функция уже оттещена

    // теперь мы делаем что: мы создаём разные конфигурации карты и смотрим:
    for (int seed = 0; seed < 15; ++seed)
    {
        srand(seed);
        for (int level = 1; level < 10; ++level)
        {
            settings.size_x = settings.size_y = level * 10; // чтобы размеры карты также изменялись

            init_map(&game_map, settings);  // эта функция уже оттещена
            generate_maps_landscape(&game_map);
            bool has_portal = false;
            for (int y = 0; y < game_map.size_y; ++y)
                for (int x = 0; x < game_map.size_y; ++x)
                    if (game_map.data[y][x].type == FINISH_CELL)
                        has_portal = true;


            // проверяем, что в любой конфигурации карты будет переход на след. уровень
            if (!has_portal)
            {
                *message = GENERATE_MAPS_LANDSCAPE_GENERATED_MAP_WITHOUT_NEXT_LEVEL_CELL;
                return 1;
            };
            // проверяем, что в карте ровно один, полностью связный, граф
            if (cnt_places(&game_map) != 1)
            {
                *message = GENERATE_MAPS_LANDSCAPE_GENERATED_NON_ONE_GRAPH;
                return 1;
            }
            delete_map(&game_map);  // эта функция уже оттещена
        };
    };
    // не могу больше придумать проблем с ландшафтом
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
    map_1.units_list = malloc(777);
    map_1.items_list = malloc(777);

    // три указателя на память
    if (init_map(&map_1, map_1_settings) != MAP_ALREADY_EXISTS)
    {
        *message = INIT_MAP_ERROR_ALREADY_EXISTS;
        return 1;
    };
    free(map_1.data);
    map_1.data = NULL;

    // два указателя на память
    if (init_map(&map_1, map_1_settings) != MAP_ALREADY_EXISTS)
    {
        *message = INIT_MAP_ERROR_ALREADY_EXISTS;
        return 1;
    };
    free(map_1.units_list);
    map_1.units_list = NULL;

    // один указатель на память
    if (init_map(&map_1, map_1_settings) != MAP_ALREADY_EXISTS)
    {
        *message = INIT_MAP_ERROR_ALREADY_EXISTS;
        return 1;
    };
    free(map_1.items_list);
    map_1.items_list = NULL;
    // все указатели почищены, поэтому теперь init_map будет давать другие ошибки

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
    // проверяем, что невозможно удалить нулевой указатель

    if (delete_map(NULL) != EMPTY_POINTER)
    {
        *message = DELETE_MAP_ERROR_WITH_NULL;
        return 1;
    };

    MapSettings map_1_settings;
    GameMap map_1;
    
    map_1.data = NULL;  // моделируем ситуацию, что карту ещё не инициализировали
    map_1.units_list = NULL;
    map_1.items_list = NULL;
    
    // проверяем, что карта не инициализировалась (что есть, что удалять)
    if (delete_map(&map_1) != MAP_ALREADY_DELETED)
    {
        *message = DELETE_MAP_DELETED_NON_INITIALIZED_MAP;
        return 1;
    };

    // теперь инициализируем карту
    map_1_settings.size_x = 40;
    map_1_settings.size_y = 40;
    map_1_settings.level = 5;

    init_map(&map_1, map_1_settings);   // имеем право использовать ф-ю, т.к. она уже оттестирована
    
    // проверяем, что ф-я работает в корректных ситуациях
    if (delete_map(&map_1) != OK)
    {
        *message = DELETE_MAP_CANT_DELETE;
        return 1;
    };
    
    // проверяем, что функция, ежели вернула ОК, действительно всё подчистила
    if (map_1.data != NULL || map_1.units_list != NULL || map_1.items_list != NULL)
    {
        *message = DELETE_MAP_DIDNT_DELETE;
        return 1;
    };

    // все тесты прошли благополучно, можем возвращать 0
    return 0;
};

/***********
/* @brief тестирует все функции библиотеки Map
/* @return 0, если корректно, 1, если ошибка
*/
int test_Map()
{
    char *message;
    if (_test_init_map(&message) == 1)  // 1 / 7
    {
        printf("%s", message);
        return 1;
    };
    if (_test_delete_map(&message) == 1) // 2 / 7
    {
        printf("%s", message);
        return 1;
    };
    if (_test_generate_map_landscape(&message) == 1)    // 3 / 7
    {
        printf("%s", message);
        return 1;
    };
    
    if(_test_generate_map_content(&message) == 1)
    {
        printf("%s", message);
        return 1;
    };

    if (_test_game_is_finished(&message) == 1)
    {
        printf("%s", message);
        return 1;
    };
    /*
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
    */
    return 0;
};
