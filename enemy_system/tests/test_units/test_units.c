#include <stdlib.h>
#include <string.h>
#include "units.h"
#include "test_units.h"

char ATTACK_NO_DEFENSE[] = "attack: Неверное поведение при атаке монстра без защиты";
char ATTACK_WITH_DEFENSE[] = "attack: Неверное поведение при атаке монстра с защитой (защита меньше атаки)";
char ATTACK_WITH_EQUAL_DEFENSE[] = "attack: Неверное поведение при атаке монстра с защитой (защита равна атаке)";

char GENERATE_MONSTERS_INVALID_HP[] = "generate_monsters: Некорректно инициализировано здоровье";
char GENERATE_MONSTERS_INVALID_DMG[] = "generate_monsters: Некорректно инициализирован урон";
char GENERATE_MONSTERS_INVALID_DEFENCE[] = "generate_monsters: Некорректно инициализирована защита";
char GENERATE_MONSTERS_INVALID_CHANCE[] = "generate_monsters: Некорректно инициализирован шанс промаха";
char GENERATE_MONSTERS_INVALID_TYPE[] = "generate_monsters: Некорректно инициализирован тип противника";
char GENERATE_MONSTERS_INVALID_INITIAL_LEVEL[] = "generate_monsters: Некорректно инициализирован уровень противника";
char GENERATE_MONSTERS_INVALID_KILLS[] = "generate_monsters: Некорректно инициализировано число убийств противника";
char GENERATE_MONSTERS_INVALID_CODE[] = "generate_monsters: Функция выдаёт код ошибки при корректных входных данных";
char GENERATE_MONSTERS_INVALID_SIZE[] = "generate_monsters: Функция работает при некорректном размере";
char GENERATE_MONSTERS_INVALID_LEVEL[] = "generate_monsters: Функция работает при некорректном уровне";

char GENERATE_LOOT_INVALID_TYPE[] = "generate_loot: Некорректно инициализирован тип";
char GENERATE_LOOT_INVALID_USES[] = "generate_loot: Некорректно инициализировано число использований";
char GENERATE_LOOT_INVALID_SCALE[] = "generate_loot: Некорректно инициализирована степень эффекта";
char GENERATE_LOOT_INVALID_CODE[] = "generate_loot: Функция выдаёт код ошибки при корректных входных данных";
char GENERATE_LOOT_INVALID_SIZE[] = "generate_loot: Функция работает при некорректном размере";
char GENERATE_LOOT_INVALID_LEVEL[] = "generate_loot: Функция работает при некорректном уровне";

char USE_INVALID_NUMBER_OF_USES[] = "use: число использований предмета не уменьшилось";
char USE_EFFECT_NOT_APPLIED[] = "use: эффект зелья не применился";
char USE_ITEM_NOT_DELETED[] = "use: зелье не удалено из инвентаря после полного использования";
char USE_INVALID_CODE[] = "use: Функция выдаёт код ошибки при корректных входных данных";
char USE_NOT_CONSUMABLE_WRONG_BEHAVIOUR[] = "use: Функция выдаёт неверный код возврата при неверно переданном типе предмета";
char USE_INVALID_ITEM_INDEX_WRONG_BEHAVIOUR[] = "use: Функция выдаёт неверный код возврата при выходе за размеры инвентаря";

char ADD_TO_INVENTORY_INVALID_CODE[] = "add_to_inventory: Функция выдаёт код ошибки при корректных входных данных";
char ADD_TO_INVENTORY_INVALID_INVENTORY_SIZE[] = "add_to_inventory: Некорректный размер инвентаря при добавлении предмета";
char ADD_TO_INVENTORY_INVALID_ITEMS_ADDED[] = "add_to_inventory: Были добавлены неверные предметы";

char DELETE_FROM_INVENTORY_INVALID_SIZE[] = "delete_from_inventory: Некорректный размер инвентаря при удалении предмета";
char DELETE_FROM_INVENTORY_INVALID_INDEX[] = "delete_from_inventory: Функция не возвращает ошибку при некорректном значении индекса предмета";
char DELETE_FROM_INVENTORY_MIDDLE_DELETION[] = "delete_from_inventory: При удалении из середины инвентаря функция отрабатывает неверно";
char DELETE_FROM_INVENTORY_INVALID_CODE[] = "delete_from_inventory: Функция выдаёт код ошибки при корректных входных данных";

char EQUIP_FROM_INVENTORY_INVALID_CODE[] = "equip_from_inventory: Функция выдаёт код ошибки при корректных входных данных";
char EQUIP_FROM_INVENTORY_ITEM_NOT_EQUIPPED[] = "equip_from_inventory: Предмет не был экипирован";
char EQUIP_FROM_INVENTORY_NO_EFFECTS[] = "equip_from_inventory: Не сработал эффект предмета";
char EQUIP_FROM_INVENTORY_DELETION_OF_EQUIPPED_ITEM[] = "equip_from_inventory: Экипированный предмет удалён из инвентаря";
char EQUIP_FROM_INVENTORY_EQUIPPED_TO_USED_SLOT[] = "equip_from_inventory: Предмет экипирован в занятый слот";
char EQUIP_FROM_INVENTORY_EQUIPPED_UNEQUIPPABLE[] = "equip_from_inventory: Экипирован неэкипируемый предмет";

char UNEQUIP_INVALID_CODE[] = "unequip: Функция выдаёт код ошибки при корректных входных данных";
char UNEQUIP_ITEM_EQUIPPED[] = "unequip: Предмет остался экипированным";
char UNEQUIP_EFFECTS_STILL_APPLIED[] = "unequip: Эффекты от предмета остались на персонаже";
char UNEQUIP_UNEQUIPPED_EMPTY_SLOT[] = "unequip: При попытке снять несуществующий предмет не возникла ошибка";

int test_units(char **message)
{
    if (test_attack(message))
        return 1;
    if (test_generate_monsters(message))
        return 1;
    if (test_generate_loot(message))
        return 1;
    if (test_add_to_inventory(message))
        return 1;
    if (test_use(message))
        return 1;
    if (test_delete_from_inventory(message))
        return 1;
    if (test_equip_from_inventory(message))
        return 1;
    if (test_unequip(message))
        return 1;
    return 0;
}

int test_attack(char **message)
{
    const int INITIAL_HP = 20;
    const int INITIAL_DMG = 10;
    const int INITIAL_DEFENSE = 3;

    int exit_code;

    Unit *attacker = malloc(sizeof(Unit));
    Unit *defender = malloc(sizeof(Unit));

    attacker->miss_chance = 0;
    attacker->dmg = INITIAL_DMG;
    defender->hp = INITIAL_HP;
    defender->defense = 0;

    exit_code = attack(attacker, defender);

    if (!(defender->hp == INITIAL_HP - INITIAL_DMG && exit_code == OK))
    {
        *message = ATTACK_NO_DEFENSE;
        return 1;
    }

    defender->defense = INITIAL_DEFENSE;
    defender->hp = INITIAL_HP;

    exit_code = attack(attacker, defender);

    if (!(defender->hp == INITIAL_HP - (INITIAL_DMG - INITIAL_DEFENSE) && exit_code == OK))
    {
        *message = ATTACK_WITH_DEFENSE;
        return 1;
    }

    defender->hp = INITIAL_HP;
    defender->defense = INITIAL_DMG;

    exit_code = attack(attacker, defender);

    if (!(defender->hp == INITIAL_HP - 1 && exit_code == OK))
    {
        *message = ATTACK_WITH_EQUAL_DEFENSE;
        return 1;
    }

    return 0;
}

int test_generate_monsters(char **message)
{
    const int NUMBER_OF_MONSTERS = 1;
    const int LEVEL = 1;
    int error_code;
    Unit attacker = {0};
    Unit defender = {0};
    Unit monsters[] = {attacker};

    error_code = generate_monsters(monsters, NUMBER_OF_MONSTERS, LEVEL);

    if (!(monsters[0].hp > 0))
    {
        *message = GENERATE_MONSTERS_INVALID_HP;
        return 1;
    }

    if (!(monsters[0].dmg > 0))
    {
        *message = GENERATE_MONSTERS_INVALID_DMG;
        return 1;
    }

    if (!(monsters[0].defense >= 0))
    {
        *message = GENERATE_MONSTERS_INVALID_DEFENCE;
        return 1;
    }

    if (!(monsters[0].miss_chance > 0))
    {
        *message = GENERATE_MONSTERS_INVALID_CHANCE;
        return 1;
    }

    if (!(monsters[0].unit_type > 0 && monsters[0].unit_type < UNIT_TYPE_LENGTH))
    {
        *message = GENERATE_MONSTERS_INVALID_TYPE;
        return 1;
    }

    if (!(monsters[0].lvl == LEVEL))
    {
        *message = GENERATE_MONSTERS_INVALID_LEVEL;
        return 1;
    }

    if (!(monsters[0].kills == 0))
    {
        *message = GENERATE_MONSTERS_INVALID_KILLS;
        return 1;
    }

    if (!(error_code == OK))
    {
        *message = GENERATE_MONSTERS_INVALID_CODE;
        return 1;
    }

    error_code = generate_monsters(monsters, -1, 2);

    if (!(error_code == INVALID_SIZE))
    {
        *message = GENERATE_MONSTERS_INVALID_SIZE;
        return 1;
    }

    error_code = generate_monsters(monsters, 1, -2);

    if (!(error_code == INVALID_LEVEL))
    {
        *message = GENERATE_MONSTERS_INVALID_LEVEL;
        return 1;
    }

    return 0;
}

int test_generate_loot(char **message)
{
    int LOOT_QUANTITY = 2;
    int LEVEL = 1;
    int error_code;
    Item item_1 = {0};
    Item item_2 = {0};
    Item items[] = {item_1, item_2};

    error_code = generate_loot(items, LOOT_QUANTITY, LEVEL);

    if (!(items[0].type < NUMBER_OF_TYPES))
    {
        *message = GENERATE_LOOT_INVALID_TYPE;
        return 1;
    }
    if (!(items[0].uses == 1))
    {
        *message = GENERATE_LOOT_INVALID_USES;
        return 1;
    }
    if (!(items[0].effects.effect_list[0].scale == 1))
    {
        *message = GENERATE_LOOT_INVALID_SCALE;
        return 1;
    }
    if (!(error_code == OK))
    {
        *message = GENERATE_LOOT_INVALID_CODE;
        return 1;
    }

    error_code = generate_loot(items, -1, 2);

    if (!(error_code == INVALID_SIZE))
    {
        *message = GENERATE_LOOT_INVALID_SIZE;
        return 1;
    }

    error_code = generate_loot(items, 1, 0);

    if (!(error_code == INVALID_LEVEL))
    {
        *message = GENERATE_LOOT_INVALID_LEVEL;
        return 1;
    }

    return 0;
}

int test_add_to_inventory(char **message)
{
    int exception;
    Unit *player = malloc(sizeof(Unit));
    generate_player(player, "TEST");

    Item *item = malloc(sizeof(Item));

    Effect *heal = malloc(sizeof(Effect));
    heal->scale = 10;
    heal->type = HP_UP;

    Effects effects = {
        heal,
        1,
        256};

    item->type = LEGS;
    item->effects = effects;
    item->name = "TEST_ITEM";
    item->uses = 1;

    exception = add_to_inventory(player, *item);

    if (exception != OK)
    {
        *message = ADD_TO_INVENTORY_INVALID_CODE;
        return 1;
    }

    if (player->inventory.current_size != 1)
    {
        *message = ADD_TO_INVENTORY_INVALID_INVENTORY_SIZE;
        return 1;
    }

    for (int i = 0; i < 5; i++)
    {
        add_to_inventory(player, *item);
    }

    if (player->inventory.current_size != 6)
    {
        *message = ADD_TO_INVENTORY_INVALID_INVENTORY_SIZE;
        return 1;
    }

    for (int i = 0; i < player->inventory.current_size; i++)
        if (!(strcmp(get_item_by_index(player, i)->name, "TEST_ITEM") == 0))
            {
                *message = ADD_TO_INVENTORY_INVALID_ITEMS_ADDED;
                return 1;
            }

    return 0;
}

int test_delete_from_inventory(char **message)
{
    int exception;
    Unit *player = malloc(sizeof(Unit));
    generate_player(player, "TEST");

    Item *item = malloc(sizeof(Item));

    Effect *heal = malloc(sizeof(Effect));
    heal->scale = 10;
    heal->type = HP_UP;

    Effects effects = {
        heal,
        1,
        256};

    item->type = LEGS;
    item->effects = effects;
    item->name = "TEST_ITEM";
    item->uses = 1;

    add_to_inventory(player, *item);
    exception = delete_from_inventory(player, 0);

    if (exception != OK)
    {
        *message = DELETE_FROM_INVENTORY_INVALID_CODE;
        return 1;
    }

    if (player->inventory.current_size != 0)
    {
        *message = DELETE_FROM_INVENTORY_INVALID_SIZE;
        return 1;
    }

    exception = delete_from_inventory(player, 0);

    if (exception != INVALID_ITEM_INDEX)
    {
        *message = DELETE_FROM_INVENTORY_INVALID_INDEX;
        return 1;
    }

    for (int i = 0; i < 3; i++)
        add_to_inventory(player, *item);

    player->inventory.items[0].name = "TEST_ITEM_0";
    player->inventory.items[1].name = "TEST_ITEM_1";
    player->inventory.items[2].name = "TEST_ITEM_2";

    exception = delete_from_inventory(player, 1);

    if (!(strcmp(get_item_by_index(player, 0)->name, "TEST_ITEM_0") == 0
       && strcmp(get_item_by_index(player, 1)->name, "TEST_ITEM_2") == 0))
    {
        *message = DELETE_FROM_INVENTORY_MIDDLE_DELETION;
        return 1;
    }

    return 0;
}

int test_use(char **message)
{
    int USES = 2;
    int HEAL = 10;
    int INITIAL_HP = 10;

    Unit *player = malloc(sizeof(Unit));
    Item *potion = malloc(sizeof(Item));
    int exception;

    generate_player(player, "TEST");

    Effect *potion_heal = malloc(sizeof(Effect));
    potion_heal->scale = HEAL;
    potion_heal->type = HP_UP;

    Effects healing = {
        potion_heal,
        1,
        256
    };

    potion->type = CONSUMABLE;
    potion->effects = healing;
    potion->name = "TEST_POTION";
    potion->uses = USES;

    add_to_inventory(player, *potion);
    exception = use(player, 0);

    if (exception != OK)
    {
        *message = USE_INVALID_CODE;
        return 1;
    }

    if (get_item_by_index(player, 0)->uses != USES - 1)
    {
        *message = USE_INVALID_NUMBER_OF_USES;
        return 1;
    }

    if (player->hp != INITIAL_HP + HEAL)
    {
        *message = USE_EFFECT_NOT_APPLIED;
        return 1;
    }

    exception = use(player, 0);

    if (exception != OK)
    {
        *message = USE_INVALID_CODE;
        return 1;
    }

    if (player->inventory.current_size != 0)
    {
        *message = USE_ITEM_NOT_DELETED;
        return 1;
    }

    Item *armor = malloc(sizeof(Item));

    Effect *heal = malloc(sizeof(Effect));
    heal->scale = 10;
    heal->type = HP_UP;

    Effects effects = {
        heal,
        1,
        256};

    armor->type = LEGS;
    armor->effects = effects;
    armor->name = "TEST_ARMOR";
    armor->uses = 1;

    add_to_inventory(player, *armor);
    exception = use(player, 0);

    if (exception != NOT_CONSUMABLE)
    {
        *message = USE_NOT_CONSUMABLE_WRONG_BEHAVIOUR;
        return 1;
    }

    exception = use(player, 123);

    if (exception != INVALID_ITEM_INDEX)
    {
        *message = USE_INVALID_ITEM_INDEX_WRONG_BEHAVIOUR;
        return 1;
    }

    return 0;
}

int test_equip_from_inventory(char **message)
{
    Unit *player = malloc(sizeof(Unit));
    Item *armor = malloc(sizeof(Item));

    int exception;

    generate_player(player, "TEST");

    Effect *heal = malloc(sizeof(Effect));
    heal->scale = 10;
    heal->type = HP_UP;

    Effects effects = {
        heal,
        1,
        256};

    armor->type = LEGS;
    armor->effects = effects;
    armor->name = "TEST_ARMOR";
    armor->uses = 1;

    add_to_inventory(player, *armor);

    exception = equip_from_inventory(player, 0);

    if (exception != OK)
    {
        *message = EQUIP_FROM_INVENTORY_INVALID_CODE;
        return 1;
    }
    if (player->equipped_slots.legs->name != "TEST_ARMOR")
    {
        *message = EQUIP_FROM_INVENTORY_ITEM_NOT_EQUIPPED;
        return 1;
    }
    if (player->hp != 20)
    {
        *message = EQUIP_FROM_INVENTORY_NO_EFFECTS;
        return 1;
    }

    exception = delete_from_inventory(player, 0);

    if (exception != ITEM_IS_EQUIPPED)
    {
        *message = EQUIP_FROM_INVENTORY_DELETION_OF_EQUIPPED_ITEM;
        return 1;
    }

    Item *potion = malloc(sizeof(Item));

    Effect *potion_heal = malloc(sizeof(Effect));
    potion_heal->scale = 10;
    potion_heal->type = HEAL;

    Effects healing = {
        potion_heal,
        1,
        256
    };

    potion->type = CONSUMABLE;
    potion->effects = healing;
    potion->name = "TEST_POTION";
    potion->uses = 1;

    exception = equip_from_inventory(player, 0);

    if (exception != SLOT_IS_USED)
    {
        *message = EQUIP_FROM_INVENTORY_EQUIPPED_TO_USED_SLOT;
        return 1;
    }

    add_to_inventory(player, *potion);

    exception = equip_from_inventory(player, 1);

    if (exception != NOT_EQUIPPABLE)
    {
        *message = EQUIP_FROM_INVENTORY_EQUIPPED_UNEQUIPPABLE;
        return 1;
    }

    return 0;
}

int test_unequip(char **message)
{
    Unit *player = malloc(sizeof(Unit));
    Item *armor = malloc(sizeof(Item));

    int exception;

    generate_player(player, "TEST");

    Effect *heal = malloc(sizeof(Effect));
    heal->scale = 10;
    heal->type = HP_UP;

    Effects effects = {
        heal,
        1,
        256};

    armor->type = LEGS;
    armor->effects = effects;
    armor->name = "TEST_ARMOR";
    armor->uses = 1;

    add_to_inventory(player, *armor);
    equip_from_inventory(player, 0);

    exception = unequip(player, LEGS);

    if (exception == OK)
    {
        *message = UNEQUIP_INVALID_CODE;
        return 1;
    }
    if (player->equipped_slots.legs != NULL)
    {
        *message = UNEQUIP_ITEM_EQUIPPED;
        return 1;
    }
    if (player->hp != 10)
    {
        *message = UNEQUIP_EFFECTS_STILL_APPLIED;
        return 1;
    }

    exception = unequip(player, LEGS);

    if (exception != SLOT_IS_NOT_USED)
    {
        *message = UNEQUIP_UNEQUIPPED_EMPTY_SLOT;
        return 1;
    }

    return 0;
}
