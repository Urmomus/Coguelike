#ifndef ITEMS_H
#define ITEMS_H

#include "exceptions.h"

typedef enum
{
    POISON,
    HEAL,
    DMG_UP,
    HP_UP,
    DEFENSE_UP,
    NUMBER_OF_EFFECTS
    // ...
} EffectType;


typedef struct
{
    EffectType type;
    int scale;
} Effect;


typedef struct
{
    Effect *effect_list;
    int effects_number;
    int max_size;
} Effects;


typedef enum
{
    HEAD,
    RIGHT_HAND,
    LEFT_HAND,
    LEGS,
    BODY,
    CONSUMABLE,
    NUMBER_OF_TYPES
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
    Effects effects; // TODO: Убрать указатель
    char *name;
    int uses;
} Item;


typedef struct
{
    Item *head;
    Item *right_hand;
    Item *left_hand;
    Item *legs;
    Item *body;
} EquippedSlots;

typedef struct
{
    Item *items;
    int current_size;
    int max_size;
} Inventory;

/**
 * @brief функция генерации предметов
 * @param items неинициализированные предметы
 * @param level текущий уровень подземелья
 * @return возвращает код ошибки
 */
ExceptionStatus generate_loot(Item *items, int size, int level);

#endif
