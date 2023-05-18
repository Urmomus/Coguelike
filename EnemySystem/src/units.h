#ifndef UNITS_H
#define UNITS_H

#include <stdbool.h>
#include "items.h"
#include "exceptions.h"


/**
 * @brief тип персонажа
 * @enum PLAYER игрок
 * @enum GOBLIN гоблин
 * ...
*/
typedef enum
{
    PLAYER,
    GOBLIN
    // ...
} UnitType;


/**
 * @brief структура персонажа
 * @param unit_type тип персонажа
 * @param effects эффекты, наложенные на персонажа
 * @param inventory инвентарь
 * @param equipped_slots экипированные слоты
 * @param miss_chance шанс промаха
 * @param name имя
 * @param lvl уровень
 * @param kills число убийств
 * @param hp здоровье
 * @param dmg атака
 * @param defense защита
 */
typedef struct
{
    UnitType unit_type;
    Effect *effects;
    Item *inventory;
    Slot *equipped_slots;
    float miss_chance;
    char *name;
    int x, y;
    int lvl;
    int kills;
    int hp;
    int dmg;
    int defense;
} Unit;


// ExceptionStatus pick_up(Unit *unit, Cell cell);

/**
 * @brief функция атаки одного моба на другого
 * @param attacker атакующий моб
 * @param defender защищающийся моб
*/
ExceptionStatus attack(Unit *attacker, Unit *defender);

/**
 * @brief функция генерации противников
 * @param monsters неинициализированные противники
 * @param level текущий уровень подземелья
 * @return возвращает код ошибки
 */
ExceptionStatus generate_monsters(Unit *monsters[], int size, int level);

/**
 * @brief функция генерации предметов
 * @param items неинициализированные предметы
 * @param level текущий уровень подземелья
 * @return возвращает код ошибки
 */
ExceptionStatus generate_loot(Item *Items[], int size, int level);

ExceptionStatus take_damage(Unit *unit, int damage);
ExceptionStatus use(Unit *unit, Item *item);
ExceptionStatus equip(Unit *unit, Item item);

// /**
//  * @brief move_monster функция передвигает противника по направлению к игроку
//  * @param monster передвигаемый противник
//  * @param player игрок
//  * @return возвращает код ошибки
//  */
// ExceptionStatus move_monster(Unit *monster, Unit *player);

#endif
