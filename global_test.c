#include "test_Map.h"

int main(void)
{
    int err_code;
    err_code = test_Map();

    // если тест библиотеки прошёл с ошибками -- заканчиваем программу
    if (err_code == 1)
        return 1;

    return 0;
};