#include <stdlib.h>
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

int test_units(char **message)
{
    if (test_attack(message))
        return 1;
    if (test_generate_monsters(message))
        return 1;
    if (test_generate_loot(message))
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
