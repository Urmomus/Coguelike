#ifndef TEST_SAVING_SYSTEM_H
#define TEST_SAVING_SYSTEM_H

/**
* @brief тестирует функцию save
* @return 0, если корректно, 1, если ошибка
*/
int test_save(char **message);

/**
* @brief тестирует функцию save
* @return 0, если корректно, 1, если ошибка
*/
int test_load(char **message);

/**
* @brief тестирует функции библиотеки saving_system
* @return 0, если корректно, 1, если ошибка
*/
int test_saving_system(char **message);

#endif