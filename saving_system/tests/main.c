#include <stdio.h>
#include "test_saving_system.h"

int main()
{
    char *message;
    int test_code = test_saving_system(&message);
    if (test_code)
    {
        printf("\033[31m\033[1m");
        printf("*** ОШИБКА ***\n");
        printf("%s\n", message);
        printf("\033[0m");
    }
    else
    {
        printf("\033[0;32;32m");
        printf("Тесты выполнены успешно\n");
        printf("\033[0m");
    }
    return 0;

}