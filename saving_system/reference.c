#include <stdio.h>
#include <stdlib.h>

struct person
{
    char name[16];
    int age;
};

int save(char * filename, struct person *p);
int load(char * filename);

int main(void)
{
    char * filename = "person.dat";
    struct person tom = { "Tom Smith", 21 };

    save(filename, &tom);
    load(filename);

    return 0;
}

// запись структуры в файл
int save(char * filename, struct person *p)
{
    FILE * fp;
    char *c;
    int size = sizeof(struct person); // количество записываемых байтов

    fp = fopen(filename, "wb"); //открываем файл для бинарной записи
    if (!fp)    // если не удалось открыть файл
    {
        printf("Error occured while opening file \n");
        return 1;
    }
    // устанавливаем указатель на начало структуры
    c = (char *)p;
    // посимвольно записываем в файл структуру
    for (int i = 0; i < size; i++)
    {
        putc(*c++, fp);
    }
    fclose(fp);
    return 0;
}

// загрузка из файла структуры
int load(char * filename)
{
    FILE * fp;
    char *c;
    int i; // для считывания одного символа
    // количество считываемых байтов
    int size = sizeof(struct person);
    // выделяем память для считываемой структуры
    struct person * ptr = malloc(size);
    fp = fopen(filename, "rb");     // открываем файл для бинарного чтения
    if (!fp)
    {
        printf("Error occured while opening file \n");
        return 1;
    }

    // устанавливаем указатель на начало блока выделенной памяти
    c = (char *)ptr;
    // считываем посимвольно из файла
    while ((i = getc(fp))!=EOF)
    {
        *c = i;
        c++;
    }

    fclose(fp);
    // вывод на консоль загруженной структуры
    printf("%-20s %5d \n", ptr->name, ptr->age);
    free(ptr);
    return 0;
}