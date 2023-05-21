all: runTests
	gcc main.c -I ./enemy_system/src -lEnemySystem -L ./enemy_system \
	-I ./Map/src -lMap -L ./Map \
	-I ./Input/src -lInput -L ./Input \
	-I ./saving_system/src -lSavingSystem -L ./saving_system \
	-I ./View/src -lView -L ./View \
	-o ./bin/main

runTests: buildLibs
	make -f TestMakefile

buildLibs: buildEnemySystem buildInput buildMap buildSavingSystem buildView

buildEnemySystem:
	cd ./enemy_system/ && make

buildInput:
	cd ./Input/ && make

buildMap:
	cd ./Map/ && make

buildSavingSystem:
	cd ./saving_system/ && make

buildView:
	cd ./View/ && make
