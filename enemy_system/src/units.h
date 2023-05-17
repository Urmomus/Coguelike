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
    Inventory inventory;
    EquippedSlots equipped_slots;
    int x, y;
    float miss_chance;
    char *name;
    int lvl;
    int kills;
    int hp;
    int dmg;
    int defense;
} Unit;

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


/**
 * @brief фунекция использования неэкипируемого предмета (например, зелья)
 * @param unit персонаж, использующий предмет
 * @param item_index индекс предмета в инвентаре
*/
ExceptionStatus use(Unit *unit, int item_index);

/**
 * @brief фунекция деэкипировки предмета
 * @param unit персонаж, деэкипирующий предмет
 * @param item_type тип слота предмета
 */
ExceptionStatus unequip(Unit *unit, ItemType item_type);

/**
 * @brief фунекция деэкипировки предмета
 * @param unit персонаж, экипирующий предмет
 * @param item_index индекс предмета в инвентаре
 */
ExceptionStatus equip_from_inventory(Unit *unit, int item_index);

/**
 * @brief фунекция добавления предмета в инвентарь
 * @param unit персонаж, подбирающий предмет
 * @param item подбираемый предмет
 */
ExceptionStatus add_to_inventory(Unit *unit, Item item);

/**
 * @brief фунекция удаления предмета из инвентаря
 * @param unit персонаж, выбрасывающий предмет
 * @param item_index индекс удаляемого предмета в инвентаре
 */
ExceptionStatus delete_from_inventory(Unit *unit, int item_index);

/**
 * @brief фунекция генерации игрока
 * @param player ссылка на игрока
 * @param player_name имя игрока
 */
ExceptionStatus generate_player(Unit *player, char *player_name);

/**
 * @brief фунекция, проверяющая, экипирован ли предмет
 * @param unit персонаж, в инвентаре которого находится предмет
 * @param item_index индекс проверяемого предмета в инвентаре
 * @param is_equipped указатель на переменную истинности суждения
 */
ExceptionStatus is_equipped(Unit *unit, int item_index, bool *is_equipped);

Item *get_item_by_slot(Unit *unit, ItemType type);
Item *get_item_by_index(Unit *unit, int index);
ExceptionStatus take_damage(Unit *unit, int damage);

#endif
