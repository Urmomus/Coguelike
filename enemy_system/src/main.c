#include "units.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(0));
    int number_of_monsters = 2;
    Unit attacker = {0};
    Unit defender = {0};

    // generate_player(&attacker, "Тесть");
    // Unit defender = {0};

    Unit monsters[] = {attacker, defender};

    generate_monsters(monsters, 2, 1);

    for (int i = 0; i < number_of_monsters; i++)
    {
        printf("Тип: %d\n", monsters[i].unit_type);
        printf("Имя: %s\n", monsters[i].name);
        printf("Дамаг: %d\n", monsters[i].dmg);
        printf("ХыПы: %d\n", monsters[i].hp);
        printf("Защита: %d\n", monsters[i].defense);
        printf("Шанс промаха: %f\n", monsters[i].miss_chance);
        printf("\n");
    }

    // monsters[1].hp = 10;

    // while (monsters[1].hp > 0)
    // {
    //     attack(&monsters[0], &monsters[1]);
    //     printf("Текущее здоровье: %d\n", monsters[1].hp);
    // }

    int loot_quantity = 2;
    Item item_1 = {0};
    Item item_2 = {0};
    Item items[] = {item_1, item_2};

    generate_loot(items, loot_quantity, 1);

    for (int i = 0; i < loot_quantity; i++)
    {
        printf("%s:\n", items[i].name);
        printf("Использований: %d\n", items[i].uses);
        printf("Степень: %d\n", items[i].effects.effect_list[0].scale);
        printf("Тип: %d\n\n", items[i].effects.effect_list[0].type);
    }

    int code = equip(&monsters[0], &items[0]);
    printf("code %d\n", code);
}
