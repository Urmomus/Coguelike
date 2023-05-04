#include "./saver.h"
#include "test_saving_system.h"
#include "ErrorCodes.h"


/**
* @brief тест на функцию load
* @return 0, если корректно, 1, если ошибка
*/
int _test_load(char **message)
{

}

/**
* @brief тест на функцию save
* @return 0, если корректно, 1, если ошибка
*/
int _test_save(char **message)
{

}

/**
* @brief тестирует все функции библиотеки Map
* @return 0 - если корректно, 1 - если ошибка
*/
int test_Map(char **message)
{
    if (_test_load(message) == 1)
        return 1;

    if (_test_save(message) == 1)
        return 1;

    return 0;
}
