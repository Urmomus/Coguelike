#include <stdio.h>
#include "test_Map.h"

int main(void)
{
    int test_code = test_Map();
    if (test_code != 0)
        return 1;
    printf("Compilation succeed!!\n");
    return 0;
}
