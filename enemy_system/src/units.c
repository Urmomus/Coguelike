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

// int _get_number_of_monsters(int level)
// {
//     return 10;
// }

// ExceptionStatus _generate_monster(int *monster, int level)
// {
//     return OK;
// }

// ExceptionStatus generate_monsters(Unit *monsters, int level)
// {
//     int number_of_monsters;

//     number_of_monsters = _get_number_of_monsters(level);
//     for (int i = 0; i < number_of_monsters; i++)
//     {
//         _generate_monster(&monsters[i], level);
//     }

//     return OK;
// }
