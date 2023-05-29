all: buildLibs
	gcc -I ./enemy_system/src -I ./Map/src main.c -lMap -lEnemySystem \
	-L./Map/ -L./enemy_system/ -lm -o ./bin/main

# runTests: buildLibs
# 	make -f TestMakefile


buildLibs: buildMap buildEnemySystem

buildMap:
	make -C ./Map/

buildEnemySystem:
	make -C ./enemy_system/
