#include "saver.h"
#include "../Map/Cell/Cell.h"

#include <stdio.h>
#include <stdlib.h>

GameMap load(GameMap map_data) {
    char *symbol;    // для считывания отдельных символов
    int m = sizeof(map_data);    // сколько надо считать для получения размера массива
    int n;  // количество структур в массиве

    FILE * fp = fopen("test.txt", "r");
    if (!fp)
    {
        printf("Error occured while opening file\n");
    }
    // выделяем память для хранения количества данных
    int *ptr_count = malloc(m);
    // считываем количество структур
    symbol = (char *)ptr_count;
    // пока не считаем m байт, сохраняем байт в выделенный блок для размера массива
    while (m > 0 && (*symbol = getc(fp)) != EOF)
    {
        symbol++;
        m--;
    }
    //получаем число элементов
    n = *ptr_count;
    free(ptr_count);    // освобождаем память
    // выделяем память для считанного массива структур
    struct person * ptr = malloc(n * sizeof(map_data));
    // устанавливаем указатель на блок памяти, выделенный для массива структур
    symbol = (char *)ptr;
    // считываем посимвольно из файла
    while ((*symbol= getc(fp)) != EOF)
    {
        symbol++;
    }
    // перебор загруженных элементов и вывод на консоль
    printf("\nFound %d people\n\n", n);

    for (int i = 0; i < n; i++)
    {
        printf("%-5d %-10s %5d \n", i + 1, (ptr + i)->name, (ptr + i)->age);
        // или так
        // printf("%-5d %-10s %5d \n", i + 1, ptr[i].name, ptr[i].age);
    }

    free(ptr);
    fclose(fp);
    return res_map;
}

int save(char * filename, GameMap *map_data) {
    FILE * fp;
    char *c;
    int size = sizeof(GameMap); // количество записываемых байтов

    fp = fopen(filename, "wb"); //открываем файл для бинарной записи
    if (!fp)    // если не удалось открыть файл
    {
        printf("Error occured while opening file \n");
        return 1;
    }
    // устанавливаем указатель на начало структуры
    c = (char *)map_data;
    // посимвольно записываем в файл структуру
    for (int i = 0; i < size; i++)
    {
        putc(*c++, fp);
    }
    fclose(fp);
    return 0;
}
