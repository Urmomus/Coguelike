# Coguelike

## Структурная схема

![](../github/Structure%20scheme.png "Structure scheme")

## Сигнатуры функций

```c
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
    int lvl;
    int kills;
    int hp;
    int dmg;
    int defense;
} Unit;

/**
 * @brief функция генерации предметов
 * @param items неинициализированные предметы
 * @param level текущий уровень подземелья
 * @return возвращает код ошибки
 */
ExceptionStatus generate_loot(Item *Items[], int size, int level);

/**
 * @brief функция генерации противников
 * @param monsters неинициализированные противники
 * @param level текущий уровень подземелья
 * @return возвращает код ошибки
 */
ExceptionStatus generate_monsters(Unit *monsters[], int size, int level);

/**
 * @brief функция атаки одного моба на другого
 * @param attacker атакующий моб
 * @param defender защищающийся моб
*/
ExceptionStatus attack(Unit *attacker, Unit *defender);
```
