#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

// Описание кодов ошибок
typedef enum
{
    OK,                 // код успешного выполнения
    GENERATION_ERROR,   // код ошибки генерации
    NOT_CONSUMABLE,     // предмет не является используемым
    NOT_EQUIPPABLE      // предмет не является экипируемым
} ExceptionStatus;

#endif
