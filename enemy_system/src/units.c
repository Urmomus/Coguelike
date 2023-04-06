#include <stdlib.h>
#include "units.h"


// ExceptionStatus pick_up(Unit *unit, Cell, cell)
// {
//     return OK;
// }

ExceptionStatus attack(Unit *attacker, Unit *defender)
{
    take_damage(defender, attacker->dmg);
}

ExceptionStatus take_damage(Unit *unit, int damage)
{
    unit->hp -= damage;
}

ExceptionStatus use(Unit *unit, Item *item)
{
    if (item->type != CONSUMABLE)
        return NOT_CONSUMABLE;

    item->uses--;
    return OK;
}

ExceptionStatus equip(Unit *unit, Item item)
{
    if (item.type != EQUIPPABLE)
        return NOT_EQUIPPABLE;

    Slot slot = {item.type, item};
    unit->equipped_slots[item.type] = slot;
}

ExceptionStatus _generate_monster(Unit *monster, int level)
{
    monster->defense = level;
    monster->dmg = level;
    // monster->effects;
    monster->equipped_slots;
    monster->hp = level;
    // monster->inventory;
    monster->kills;
    monster->lvl = level;
    monster->miss_chance = 0;
    monster->name = "kookech";
    monster->unit_type = GOBLIN;
}

ExceptionStatus generate_monsters(Unit *monsters[], int size, int level)
{
    for (int i = 0; i < size; i++)
    {
        monsters[i] = malloc(sizeof(Unit));
        _generate_monster(monsters[i], level);
    }

    return OK;
}
