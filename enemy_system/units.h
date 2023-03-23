#include <stdbool.h>
#include "items.h"
#include "exceptions.h"


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
    float miss_chance;
    char *name;
    int lvl;
    int kills;
    int hp;
    int dmg;
    int defense;
} Unit;


// void pick_up(Unit *unit, Cell cell);
ExceptionStatus attack(Unit *attacker, Unit *defender);
ExceptionStatus take_damage(Unit *unit, int damage);
ExceptionStatus use(Unit *unit, Item *item);
ExceptionStatus equip(Unit *unit, Item item);
