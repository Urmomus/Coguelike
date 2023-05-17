#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// Описание кодов ошибок
typedef enum
{
    OK,               // код успешного выполнения
    GENERATION_ERROR, // код ошибки генерации
    NOT_CONSUMABLE,   // предмет не является используемым
    NOT_EQUIPPABLE,   // предмет не является экипируемым
    INVALID_LEVEL,
    INVALID_SIZE,
    INVALID_DAMAGE,
    INVALID_EFFECT,
    SLOT_IS_USED,
    SLOT_IS_NOT_USED,
    INVALID_ITEM_INDEX,
    ITEM_IS_EQUIPPED
} ExceptionStatus;

#endif
