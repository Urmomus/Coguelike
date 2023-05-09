#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include "units.h"


// Тут имена мобов в зависимости от их типов

const char *PLAYER_NAMES[50] = {
    "Елистрат", "Моцарелл", "Ратобор", "Пармезан", "Ведармот",
    "Ведахрап", "Светосвал", "Ватерпежекосма", "Кукуцаполь", "Фемистоклей",
    "Нисерха", "Дуклид", "Добромысл", "Полихроний", "Электрополоний", "Грибабас"
};

// Я хочу есть
const char *SLIME_NAMES[50] = {
    "Боба", "Герри", "Вжух", "Беззуб", "Барон де Улит", "Желатинис", "Агарыч", "Желфикс",
    "Студень", "Бланманж", "Паннакот", "Холодец", "Зефирус", "Кисель"
};

const char *GOBLIN_NAMES[50] = {
    "Куконя", "Челибоб", "Красавчик", "Ржаволюгс", "Веслобумс", "Быстротукс",
    "Карамутикс", "Грун", "Ольеггс"
};

const char *SKELETON_NAMES[50] = {
    "Костик", "Ветчина", "Ефстрат Гемоглобинович", "Кеша", "Жмурик", "Бон Джови",
    "Бона Лиза", "Черепица", "Погремуш", "Мукомол", "Бон Апарт", "Бонд", "Костя",
    "Константин"
};

const char *GHOST_NAMES[50] = {
    "Жертва сессии", "Бабадук", "Каспер", "Бууу", "Кринч", "Домовой", "Слепошар",
    "Онотоле", "Гномыч", "Карапет", "Разумистыч", "Могилич", "Викернес"

};

// Смотри не вляпайся (Тут буквально захардкожены длины массивов имён под каждый тип мобов)
// TODO: переделать под структуру
const int NUMBER_OF_NAMES[] = {
    16,
    14,
    9,
    14,
    13
};

const char **MONSTER_NAMES[50] = {
    PLAYER_NAMES,
    SLIME_NAMES,
    GOBLIN_NAMES,
    SKELETON_NAMES,
    GHOST_NAMES
};

void _generate_unit_type(Unit *monster)
{
    monster->unit_type = 1 + rand() % (UNIT_TYPE_LENGTH - 1);
}

void _generate_monster_name(Unit *monster)
{
    const char **possible_names = MONSTER_NAMES[monster->unit_type];
    int possible_names_size = NUMBER_OF_NAMES[monster->unit_type];
    char *new_name = (char *)possible_names[rand() % (possible_names_size - 1)];

    monster->name = new_name;
}

void _generate_monster_dmg(Unit *monster)
{
    monster->dmg = monster->lvl * 2 + (rand() % (1 + monster->lvl * 2) - monster->lvl);
}

void _generate_monster_hp(Unit *monster)
{
    monster->hp = monster->lvl * 3 + (rand() % (1 + monster->lvl * 2) - monster->lvl);
}

void _generate_monster_defense(Unit *monster)
{
    monster->defense = (int)(monster->lvl / 2 + rand() % monster->lvl);
}

void _generate_monster_miss_chance(Unit *monster)
{
    monster->miss_chance = (double) 1 / (monster->lvl * 2);
}

ExceptionStatus _generate_monster(Unit *monster, int level)
{
    monster->kills = 0;
    monster->lvl = level;
    _generate_monster_miss_chance(monster);
    _generate_unit_type(monster);
    _generate_monster_defense(monster);
    _generate_monster_dmg(monster);
    _generate_monster_hp(monster);
    _generate_monster_name(monster);
    monster->x = -1;
    monster->y = -1;
    // monster->inventory = NULL; TODO: ЧЁТ СДЕЛАЙ
    // monster->equipped_slots = NULL;
}

ExceptionStatus _apply_effect(Unit *unit, Effects effects)
{
    Effect effect = effects.effect_list[0];
    switch (effect.type)
    {
    case POISON:
        take_damage(unit, effect.scale);
        break;

    case HEAL:
        unit->hp += effect.scale;
        break;

    case HP_UP:
        unit->hp += effect.scale;
        break;

    case DMG_UP:
        unit->dmg += effect.scale;

    case DEFENSE_UP:
        unit->defense += effect.scale;

    default:
        break;
    }
}


ExceptionStatus generate_monsters(Unit *monsters, int size, int level)
{
    if (level <= 0)
        return INVALID_LEVEL;

    if (size <= 0)
        return INVALID_SIZE;

    for (int i = 0; i < size; i++)
        _generate_monster(monsters + i, level);

    return OK;
}

ExceptionStatus generate_player(Unit *player, char *player_name)
{
    EquippedSlots slots = {0};
    Item *items = {0};
    Inventory inventory = {items, 0, 256};

    player->unit_type = PLAYER;
    player->kills = 0;
    player->lvl = 1;
    player->name = player_name;
    player->defense = 0;
    player->hp = 10;
    player->dmg = 2;
    player->miss_chance = 0.1;
    // player->equipped_slots = &slots; TODO: ТУТЧЁТОСДЕЛАЙ
    // player->inventory = &inventory;

    return OK;
}

ExceptionStatus attack(Unit *attacker, Unit *defender)
{
    int total_damage = attacker->dmg;

    total_damage -= defender->defense;

    if (total_damage <= 0)
        total_damage = 1;

    if (rand() % 100 + 1 < 100 * attacker->miss_chance)
        total_damage = 0;

    take_damage(defender, total_damage);

    return OK;
}

ExceptionStatus take_damage(Unit *unit, int damage)
{
    if (damage < 0)
        return INVALID_DAMAGE;

    unit->hp -= damage;
}

ExceptionStatus use(Unit *unit, Item *item)
{
    if (item->type != CONSUMABLE)
        return NOT_CONSUMABLE;

    item->uses--;
    return OK;
}

ExceptionStatus add_to_inventory(Unit *unit, Item item)
{
    unit->inventory.current_size += 1;
    unit->inventory.items[unit->inventory.current_size - 1] = item;
}

ExceptionStatus delete_from_inventory(Unit *unit, int item_index)
{
    unit->inventory.current_size -= 1;
    for (int i = unit->inventory.current_size - 1; i > item_index; i++)
    {
        unit->inventory.items[i] = unit->inventory.items[i + 1];
    }
}

ExceptionStatus equip(Unit *unit, Item *item)
{
    switch (item->type)
    {
    case HEAD:
        unit->equipped_slots.head = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    case BODY:
        unit->equipped_slots.body = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    case RIGHT_HAND:
        unit->equipped_slots.right_hand = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    case LEFT_HAND:
        unit->equipped_slots.left_hand = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    case LEGS:
        unit->equipped_slots.legs = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    default:
        return NOT_EQUIPPABLE;
        break;
    }
}
