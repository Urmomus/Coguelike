all: runTests
	gcc main.c -I ./enemy_system/src -lEnemySystem -L ./enemy_system -o main

runTests:
	make -f TestMakefile
