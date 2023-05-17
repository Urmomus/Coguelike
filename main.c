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

    // Unit monsters[] = {attacker, defender};

    // generate_monsters(monsters, 2, 1);

    // for (int i = 0; i < number_of_monsters; i++)
    // {
    //     printf("Тип: %d\n", monsters[i].unit_type);
    //     printf("Имя: %s\n", monsters[i].name);
    //     printf("Дамаг: %d\n", monsters[i].dmg);
    //     printf("ХыПы: %d\n", monsters[i].hp);
    //     printf("Защита: %d\n", monsters[i].defense);
    //     printf("Шанс промаха: %f\n", monsters[i].miss_chance);
    //     printf("\n");
    // }

    // monsters[1].hp = 10;

    // while (monsters[1].hp > 0)
    // {
    //     attack(&monsters[0], &monsters[1]);
    //     printf("Текущее здоровье: %d\n", monsters[1].hp);
    // }

    // int loot_quantity = 2;
    // Item item_1 = {0};
    // Item item_2 = {0};
    // Item items[] = {item_1, item_2};

    // generate_loot(items, loot_quantity, 1);

    // for (int i = 0; i < loot_quantity; i++)
    // {
    //     printf("%s:\n", items[i].name);
    //     printf("Использований: %d\n", items[i].uses);
    //     printf("Степень: %d\n", items[i].effects.effect_list[0].scale);
    //     printf("Тип: %d\n\n", items[i].effects.effect_list[0].type);
    // }

    Unit *player = malloc(sizeof(Unit));
    Item *item = malloc(sizeof(Item));

    Effect *heal = malloc(sizeof(Effect));
    heal->scale = 10;
    heal->type = HP_UP;

    Effects effects = {
        heal,
        1,
        256
    };

    item->type = LEGS;
    item->effects = effects;
    item->name = "Ногбе";
    item->uses = 1;

    generate_player(player, "Куконя");
    add_to_inventory(player, *item);
    add_to_inventory(player, *item);

    printf("%s\n", player->inventory.items->name);
    printf("%d\n", player->inventory.current_size);

    for (int i = 0; i < player->inventory.current_size; i++)
    {
        printf("Предмет: %s \n", player->inventory.items[i].name);
        printf("Размер инвентаря: %d\n\n", player->inventory.current_size);
    }

    delete_from_inventory(player, 0);

    for (int i = 0; i < player->inventory.current_size; i++)
    {
        printf("Предмет: %s\n", player->inventory.items[i].name);
        printf("Размер инвентаря: %d\n\n", player->inventory.current_size);
    }

    Item *item_2 = malloc(sizeof(Item));

    Effect *heal_2 = malloc(sizeof(Effect));
    heal->scale = 100;
    heal->type = HP_UP;

    Effects effects_2 = {
        heal,
        1,
        256
    };

    item_2->type = HEAD;
    item_2->effects = effects_2;
    item_2->name = "Шелом";
    item_2->uses = 1;

    add_to_inventory(player, *item_2);

    printf("%d\n", player->inventory.items[0].type);

    printf("%d\n", player->hp);
    equip_from_inventory(player, 0);
    printf("%d\n", player->hp);

    bool smth = false;
    is_equipped(player, 0, &smth);
    printf("%d\n", smth);

    int exception;
    exception = delete_from_inventory(player, 0);

    printf("%d\n", exception);

    unequip(player, LEGS);

    exception = delete_from_inventory(player, 0);
    printf("%d\n", player->hp);

    printf("%d\n", exception);
}
