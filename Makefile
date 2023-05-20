all: runTests
	gcc main.c -I ./enemy_system/src -lEnemySystem -L ./enemy_system\
 		-I ./Map/src -lMap -L ./Map -o ./bin/main

runTests: buildLibs
	make -f TestMakefile


buildLibs: buildMap

buildMap:
	make -C ./Map/