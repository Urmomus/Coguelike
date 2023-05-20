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
        break;

    case DEFENSE_UP:
        unit->defense += effect.scale;
        break;

    default:
        return INVALID_EFFECT;
        break;
    }
}

ExceptionStatus _unapply_effect(Unit *unit, Effects effects)
{
    Effect effect = effects.effect_list[0];
    switch (effect.type)
    {
    case HEAL:
        unit->hp -= effect.scale;
        return OK;
        break;

    case HP_UP:
        unit->hp -= effect.scale;
        return OK;
        break;

    case DMG_UP:
        unit->dmg -= effect.scale;
        return OK;
        break;

    case DEFENSE_UP:
        unit->defense -= effect.scale;
        return OK;
        break;

    default:
        return INVALID_EFFECT;
        break;
    }
}

Item *get_item_by_index(Unit *unit, int index)
{
    if (index >= unit->inventory.current_size)
        return NULL;

    return unit->inventory.items + index;
}

Item *get_item_by_slot(Unit *unit, ItemType type)
{
    switch (type)
    {
    case HEAD:
        return unit->equipped_slots.head;
        break;

    case BODY:
        return unit->equipped_slots.body;
        break;

    case RIGHT_HAND:
        return unit->equipped_slots.right_hand;
        break;

    case LEFT_HAND:
        return unit->equipped_slots.left_hand;
        break;

    case LEGS:
        return unit->equipped_slots.legs;
        break;

    default:
        return NULL;
        break;
    }
    return OK;
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
    Item items[256] = {0};
    Inventory inventory = {items, 0, 256};
    EquippedSlots slots = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    };

    player->unit_type = PLAYER;
    player->kills = 0;
    player->lvl = 1;
    player->name = player_name;
    player->defense = 0;
    player->hp = 10;
    player->dmg = 2;
    player->miss_chance = 0.1;
    player->equipped_slots = slots;
    player->inventory = inventory;

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
    return OK;
}

ExceptionStatus use(Unit *unit, int item_index)
{
    int exception;
    Item *item = get_item_by_index(unit, item_index);

    if (item == NULL)
        return INVALID_ITEM_INDEX;

    if (item->type != CONSUMABLE)
        return NOT_CONSUMABLE;

    _apply_effect(unit, item->effects);
    item->uses--;

    if (item->uses <= 0)
        delete_from_inventory(unit, item_index);

    return OK;
}

ExceptionStatus add_to_inventory(Unit *unit, Item item)
{
    unit->inventory.current_size += 1;
    unit->inventory.items[unit->inventory.current_size - 1] = item;
    return OK;
}

ExceptionStatus delete_from_inventory(Unit *unit, int item_index)
{
    bool equipped = false;
    int exception;
    Item *item = get_item_by_index(unit, item_index);

    if (item == NULL)
        return INVALID_ITEM_INDEX;
    if (item->type != CONSUMABLE)
    {
        exception = is_equipped(unit, item_index, &equipped);
        if (exception)
            return exception;
        if (equipped)
            return ITEM_IS_EQUIPPED;
    }

    for (int i = item_index; i < unit->inventory.current_size; i++)
    {
        unit->inventory.items[i] = unit->inventory.items[i + 1];
    }
    unit->inventory.current_size -= 1;

    return OK;
}

ExceptionStatus equip(Unit *unit, Item *item)
{
    switch (item->type)
    {
    case HEAD:
        if (unit->equipped_slots.head != NULL)
            return SLOT_IS_USED;

        unit->equipped_slots.head = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    case BODY:
        if (unit->equipped_slots.body != NULL)
            return SLOT_IS_USED;

        unit->equipped_slots.body = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    case RIGHT_HAND:
        if (unit->equipped_slots.right_hand != NULL)
            return SLOT_IS_USED;

        unit->equipped_slots.right_hand = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    case LEFT_HAND:
        if (unit->equipped_slots.left_hand != NULL)
            return SLOT_IS_USED;

        unit->equipped_slots.left_hand = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    case LEGS:
        if (unit->equipped_slots.legs != NULL)
            return SLOT_IS_USED;

        unit->equipped_slots.legs = item;
        _apply_effect(unit, item->effects);
        return OK;
        break;

    default:
        return NOT_EQUIPPABLE;
        break;
    }
}

ExceptionStatus equip_from_inventory(Unit *unit, int item_index)
{
    Item *item = get_item_by_index(unit, item_index);
    int exception;
    bool equipped = 0;

    if (item == NULL)
        return INVALID_ITEM_INDEX;

    exception = is_equipped(unit, item_index, &equipped);

    if (exception)
        return exception;

    exception = equip(unit, item);
    if (exception)
        return exception;

    return OK;
}

ExceptionStatus unequip(Unit *unit, ItemType item_type)
{
    Item *item = get_item_by_slot(unit, item_type);
    switch (item_type)
    {
    case HEAD:
        if (unit->equipped_slots.head == NULL)
            return SLOT_IS_NOT_USED;

        unit->equipped_slots.head = NULL;
        _unapply_effect(unit, item->effects);
        return OK;
        break;

    case BODY:
        if (unit->equipped_slots.body == NULL)
            return SLOT_IS_NOT_USED;

        unit->equipped_slots.body = NULL;
        _unapply_effect(unit, item->effects);
        return OK;
        break;

    case RIGHT_HAND:
        if (unit->equipped_slots.right_hand == NULL)
            return SLOT_IS_NOT_USED;

        unit->equipped_slots.right_hand = NULL;
        _unapply_effect(unit, item->effects);
        return OK;
        break;

    case LEFT_HAND:
        if (unit->equipped_slots.left_hand == NULL)
            return SLOT_IS_NOT_USED;

        unit->equipped_slots.left_hand = NULL;
        _unapply_effect(unit, item->effects);
        return OK;
        break;

    case LEGS:
        if (unit->equipped_slots.legs == NULL)
            return SLOT_IS_NOT_USED;

        unit->equipped_slots.legs = NULL;
        _unapply_effect(unit, item->effects);
        return OK;
        break;

    default:
        return NOT_EQUIPPABLE;
        break;
    }
}

ExceptionStatus is_equipped(Unit *unit, int item_index, bool *is_equipped)
{
    Item *item;

    item = get_item_by_index(unit, item_index);
    if (item == NULL)
        return INVALID_ITEM_INDEX;

    if (item->type == CONSUMABLE)
        return NOT_EQUIPPABLE;

    switch (item->type)
    {
    case HEAD:
        if (unit->equipped_slots.head == item)
            *is_equipped = true;
        else
            *is_equipped = false;
        return OK;

    case BODY:
        if (unit->equipped_slots.body == item)
                *is_equipped = true;
            else
                *is_equipped = false;
            return OK;

    case RIGHT_HAND:
        if (unit->equipped_slots.right_hand == item)
                *is_equipped = true;
            else
                *is_equipped = false;
            return OK;

    case LEFT_HAND:
        if (unit->equipped_slots.left_hand == item)
                *is_equipped = true;
            else
                *is_equipped = false;
            return OK;

    case LEGS:
        if (unit->equipped_slots.legs == item)
                *is_equipped = true;
            else
                *is_equipped = false;
            return OK;

    default:
        return NOT_EQUIPPABLE;
        break;
    }
}
