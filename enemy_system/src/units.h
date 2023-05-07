#ifndef UNITS_H
#define UNITS_H

#include <stdbool.h>
#include "items.h"
#include "exceptions.h"

/**
 * @brief тип персонажа
 * @enum PLAYER игрок
 * @enum GOBLIN гоблин
 * @enum UNIT_TYPE_LENGTH наколхозил длину enum'а и доволен
 */
typedef enum
{
    PLAYER,
    SLIME,
    GOBLIN,
    SKELETON,
    GHOST,
    UNIT_TYPE_LENGTH
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
    // Effect *effects;
    Inventory inventory;
    EquippedSlots equipped_slots;
    float miss_chance;
    char *name;
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
ExceptionStatus generate_monsters(Unit *monsters, int size, int level);

/**
 * @brief функция генерации предметов
 * @param items неинициализированные предметы
 * @param level текущий уровень подземелья
 * @return возвращает код ошибки
 */

ExceptionStatus take_damage(Unit *unit, int damage);
ExceptionStatus use(Unit *unit, Item *item);
ExceptionStatus equip(Unit *unit, Item *item);
ExceptionStatus add_to_inventory(Unit *unit, Item item);
ExceptionStatus generate_player(Unit *player, char *player_name);

// /**
//  * @brief move_monster функция передвигает противника по направлению к игроку
//  * @param monster передвигаемый противник
//  * @param player игрок
//  * @return возвращает код ошибки
//  */
// ExceptionStatus move_monster(Unit *monster, Unit *player);

#endif
