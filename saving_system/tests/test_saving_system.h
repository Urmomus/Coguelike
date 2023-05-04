#ifndef TEST_SAVING_SYSTEM_H
#define TEST_SAVING_SYSTEM_H

/***********
* @brief тестирует функции библиотеки saving_system
* @return 0, если корректно, 1, если ошибка
*/
int test_saving_system(char **message);

const char* MAP_NOT_EXIST_ERROR = "Ошибка при сохраненении: карта не существует!\n";

#endif