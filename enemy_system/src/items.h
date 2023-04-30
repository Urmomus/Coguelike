#ifndef ITEMS_H
#define ITEMS_H

typedef enum
{
    POISON,
    HEAL
    // ...
} Effect;


typedef enum
{
    EQUIPPABLE,
    CONSUMABLE
} ItemType;


/**
 * @brief предмет
 * @param type тип предмета
 * @param effects эффекты предмета
 * @param name название предмета
 * @param uses число использований
*/
typedef struct
{
    ItemType type;
    Effect *effects;
    char *name;
    int uses;
} Item;


typedef enum
{
    HEAD,
    RIGHT_HAND,
    LEFT_HAND,
    LEGS,
    BODY
} SlotType;


typedef struct
{
    SlotType type;
    Item item;
} Slot;

#endif
