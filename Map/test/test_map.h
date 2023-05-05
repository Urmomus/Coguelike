#ifndef TEST_MAP_H
#define TEST_MAP_H

/***********
/* @brief тестирует все функции библиотеки Map
/* @return 0, если корректно, 1, если ошибка
*/
int test_Map(char **message);

// ошибки для init_map
const char* INIT_MAP_ERROR_WITH_SIZE = "Ошибка в init_map: создалась карта некорректного размера!\n";
const char* INIT_MAP_ERROR_WITH_NULL = "Ошибка в init_map: функция отработала для нулевого указателя!\n";


// ошибки для game_is_finished
const char* GAME_IS_FINISHED_ERROR_WITH_NULL = "Ошибка в game_is_finished: функция отработала для нулевого указателя!\n";


// ошибки для get_map_state
const char* GET_MAP_STATE_ERROR_WITH_NULL = "Ошибка в get_map_state: функция отработала для нулевого указателя!\n";


// ошибки для set_map_state
const char* SET_MAP_STATE_ERROR_WITH_NULL = "Ошибка в set_map_state: функция отработала для нулевого указателя!\n";


// ошибки для delete_map
const char* DELETE_MAP_ERROR_WITH_NULL = "Ошибка в delete_map: функция отработала для нулевого указателя!\n";



#define TEST_MAP_H