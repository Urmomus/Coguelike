#include "saver.h"
#include "../Map/Cell/Cell.h"

#include <stdio.h>
#include <stdlib.h>

GameMap load(GameMap map_data) {
    char *symbol;    // для считывания отдельных символов
    int map_size = sizeof(map_data);
    int structure_amount;

    FILE * save_file = fopen("save.txt", "r");
    if (!save_file)
    {
        printf("Error occured while opening file\n");
    }

    int *ptr_count = malloc(map_size); // выделяем память для хранения количества данных

    symbol = (char *)ptr_count; // считываем количество структур

    // пока не считаем map_size байт, сохраняем байт в выделенный блок для размера массива
    while (map_size > 0 && (*symbol = (char)getc(save_file)) != EOF)
    {
        symbol++;
        map_size--;
    }

    structure_amount = *ptr_count; //получаем число элементов
    free(ptr_count);       // освобождаем память

    // выделяем память для считанного массива структур
    struct game_map * ptr = malloc(structure_amount * sizeof(map_data));
    // устанавливаем указатель на блок памяти, выделенный для массива структур
    symbol = (char *)ptr;
    // считываем посимвольно из файла
    while ((*symbol= (char)getc(save_file)) != EOF)
    {
        symbol++;
    }
    // перебор загруженных элементов и вывод на консоль
    printf("\nmap state size:%d \n\n", structure_amount);

    for (int i = 0; i < structure_amount; ++i)
    {
        printf("", i + 1, (ptr + i)->size_x, (ptr + i)->age);
    }

    free(ptr);
    fclose(save_file);
    return res_map;
}

void save(GameMap *map_data) {
    FILE * fp;
    char *c;
    int size = sizeof(GameMap); // количество записываемых байтов

    char* filename = "save.txt";
    fp = fopen(filename, "wb"); //открываем файл для бинарной записи
    if (!fp)    // если не удалось открыть файл
    {
        printf("Error occured while opening file \n");
    }
    // устанавливаем указатель на начало структуры
    c = (char *)map_data;
    // посимвольно записываем в файл структуру
    for (int i = 0; i < size; i++)
    {
        putc(*c++, fp);
    }
    fclose(fp);
}
