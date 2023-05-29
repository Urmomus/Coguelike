all: buildLibs
	gcc -I ./enemy_system/src -I ./Map/src main.c -lMap -lEnemySystem \
	-L./Map/ -L./enemy_system/ -L./saving_system/ -I./saving_system/src/ -lSavingSystem -lm -o ./bin/main

# runTests: buildLibs
# 	make -f TestMakefile


buildLibs: buildMap buildEnemySystem buildSaves

buildMap:
	make -C ./Map/

buildEnemySystem:
	make -C ./enemy_system/

buildSaves:
	make -C ./saving_system/
