all: runTests
	gcc main.c -I ./enemy_system/src -lEnemySystem -L ./enemy_system -o ./bin/main

runTests:
	make -f TestMakefile
