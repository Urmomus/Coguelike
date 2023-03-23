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


typedef enum
{
    SWORD,
    SHIELD,
    HELMET,
    // ...
} ItemType;


typedef struct
{
    ItemType item_type;
    Effect *effects;
    char *name;
    int uses;
} Item;


typedef enum
{
    POISON,
    HEAL
    // ...
} Effect;
