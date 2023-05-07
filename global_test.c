#include "test_Map.h"
#include <stdio.h>

int main(void)
{
    char **message;
    int err_code = test_Map(message);

    // если тест библиотеки прошёл успешно -- заканчиваем программу
    if (err_code == 0)
        return 0;

    // в противном случае -- выводим сообщение об ошибке и возвращаем ошибку 1
    printf("%s", *message);
    return 1;
};