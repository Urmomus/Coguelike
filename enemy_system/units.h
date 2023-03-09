#include <stdbool.h>
#include "items.h"


typedef enum
{
    PLAYER,
    GOBLIN
    // ...
} UnitType;


typedef struct
{
    int x;
    int y;
} Position;


typedef struct
{
    UnitType unit_type;
    Position position;
    Effect *effects;
    Item *inventory;
    Slot *equipped_slots;
    char *name;
    int lvl;
    int kills;
    int hp;
    int dmg;
    int defense;
} Unit;


// void pick_up(Unit *unit, Cell cell);
void attack(Unit *attacker, Unit *defender);
void take_damage(Unit *unit, int damage);
void use(Unit *unit, Item item);
void equip(Unit *unit, Item item);
