#include <stdlib.h>
#include "units.h"

ExceptionStatus pick_up(Unit *unit, Cell, cell)
{
    return OK;
}

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
