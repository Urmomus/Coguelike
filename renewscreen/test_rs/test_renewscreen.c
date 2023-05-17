/***********
/* @brief тестирует все функции библиотеки renewscreen
/* @return 0, если корректно, 1, если ошибка
*/

#include <stdio.h>

int test_renewscreen(char **message)
{
    if (test_renew(message) == 1);
        return 1;
}

int test_renew(char **message)
{
    int do_change = 1;
    
}