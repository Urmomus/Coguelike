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
