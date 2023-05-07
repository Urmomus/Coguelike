#include <stdlib.h>
#include "items.h"
#include "exceptions.h"

ExceptionStatus _generate_consumable(Item *item, int level)
{
    Effect *effect = malloc(sizeof(Effect) * 256);
    Effects effects;

    if (rand() % 100 < 1)
        effect->type = POISON;
    else
        effect->type = HEAL;

    effect->scale = level;

    effects.effect_list = effect;
    effects.effects_number = 1;
    effects.max_size = 256;

    item->type = CONSUMABLE;
    item->uses = 1;
    item->effects = effects;
    item->name = "Неподозрительное зелье";
}

ExceptionStatus _generate_equipable(Item *item, int level)
{
    item->type = rand() % (NUMBER_OF_TYPES - 1);
    Effect *effect = malloc(sizeof(Effect) * 256);
    Effects effects;

    if (item->type == HEAD)
    {
        item->name = "Шелом";
        effect->type = HP_UP;
    }
    if (item->type == BODY)
    {
        item->name = "Нагрудник";
        effect->type = DEFENSE_UP;
    }
    if (item->type == LEFT_HAND)
    {
        item->name = "Щит";
        effect->type = DEFENSE_UP;
    }
    if (item->type == RIGHT_HAND)
    {
        item->name = "Меч";
        effect->type = DMG_UP;
    }
    if (item->type == LEGS)
    {
        item->name = "Штаны";
        effect->type = HP_UP;
    }

    effect->scale = level;

    effects.effect_list = effect;
    effects.effects_number = 1;
    effects.max_size = 256;

    item->uses = 1;
    item->effects = effects;
}

ExceptionStatus _generate_item(Item *item, int level)
{
    if (rand() % (NUMBER_OF_TYPES * 2 - 1) >= NUMBER_OF_TYPES)
        _generate_consumable(item, level);
    else
        _generate_equipable(item, level);

    return OK;
}


ExceptionStatus generate_loot(Item *items, int size, int level)
{
    if (size <= 0)
        return INVALID_SIZE;

    if (level <= 0)
        return INVALID_LEVEL;

    for (int i = 0; i < size; i++)
    {
            _generate_item(&items[i], level);
    }

    return OK;
}
