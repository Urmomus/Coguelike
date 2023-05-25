#include "/mnt/c/Users/Momeiko/Documents/GitHub/Coguelike/renewscreen/renewscreen/renewscreen.h"
#include "test_renewscreen.h"

/***********
/* @brief тестирует все функции библиотеки renewscreen
/* @return 1, если корректно, 0, если ошибка
*/

int test_renewscreen(int do_change)
{
    int error_code = renewscreen (do_change);

    if (error_code == 1)
        return 0;

    if (error_code == 0)
    {
        return 1;
    }

    return 0;

}