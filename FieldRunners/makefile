CC := g++
CCFLAGS += -l SDL2 -l SDL2_image -l SDL2_ttf -l SDL2_mixer

all: FieldRunners.out

FieldRunners.out: fieldRunners.o rsdl.o Map.o Arrows.o Towers.o Gatling.o Enemies.o Runners.o Scramblers.o StubbornRunners.o SuperTroopers.o Missile.o Glue.o Tesla.o Shot.o
	$(CC) fieldRunners.o rsdl.o Map.o Arrows.o Towers.o Gatling.o Enemies.o Runners.o Scramblers.o StubbornRunners.o SuperTroopers.o Missile.o Glue.o Tesla.o Shot.o $(CCFLAGS) -o FieldRunners.out

fieldRunners.o: src/rsdl.hpp fieldRunners/fieldRunners.cpp fieldRunners/Map.hpp fieldRunners/Arrows.hpp fieldRunners/Towers.hpp fieldRunners/Gatling.hpp fieldRunners/Enemies.hpp fieldRunners/Runners.hpp fieldRunners/SuperTroopers.hpp fieldRunners/StubbornRunners.hpp fieldRunners/Scramblers.hpp fieldRunners/Tesla.hpp fieldRunners/Glue.hpp fieldRunners/Missile.hpp fieldRunners/Shot.hpp
	$(CC) -c fieldRunners/fieldRunners.cpp -o fieldRunners.o

Map.o:  src/rsdl.hpp fieldRunners/Map.cpp fieldRunners/Map.hpp fieldRunners/Arrows.hpp fieldRunners/Towers.hpp fieldRunners/Gatling.hpp fieldRunners/Enemies.hpp fieldRunners/Runners.hpp fieldRunners/SuperTroopers.hpp fieldRunners/StubbornRunners.hpp fieldRunners/Scramblers.hpp fieldRunners/Tesla.hpp fieldRunners/Glue.hpp fieldRunners/Missile.hpp fieldRunners/Shot.hpp
	$(CC) -c fieldRunners/Map.cpp -o Map.o

Arrows.o:  src/rsdl.hpp fieldRunners/Arrows.cpp fieldRunners/Arrows.hpp
	$(CC) -c fieldRunners/Arrows.cpp -o Arrows.o

Towers.o:  src/rsdl.hpp fieldRunners/Towers.cpp fieldRunners/Towers.hpp fieldRunners/Enemies.hpp fieldRunners/Shot.hpp
	$(CC) -c fieldRunners/Towers.cpp -o Towers.o

Gatling.o:  src/rsdl.hpp fieldRunners/Gatling.cpp fieldRunners/Gatling.hpp fieldRunners/Towers.hpp fieldRunners/Shot.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/Gatling.cpp -o Gatling.o

Missile.o:  src/rsdl.hpp fieldRunners/Missile.cpp fieldRunners/Missile.hpp fieldRunners/Towers.hpp fieldRunners/Shot.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/Missile.cpp -o Missile.o

Glue.o:  src/rsdl.hpp fieldRunners/Glue.cpp fieldRunners/Glue.hpp fieldRunners/Towers.hpp fieldRunners/Shot.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/Glue.cpp -o Glue.o

Tesla.o:  src/rsdl.hpp fieldRunners/Tesla.cpp fieldRunners/Tesla.hpp fieldRunners/Towers.hpp fieldRunners/Shot.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/Tesla.cpp -o Tesla.o

Enemies.o:  src/rsdl.hpp fieldRunners/Enemies.cpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/Enemies.cpp -o Enemies.o

Runners.o:  src/rsdl.hpp fieldRunners/Runners.cpp fieldRunners/Runners.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/Runners.cpp -o Runners.o

Scramblers.o:  src/rsdl.hpp fieldRunners/Scramblers.cpp fieldRunners/Scramblers.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/Scramblers.cpp -o Scramblers.o

StubbornRunners.o:  src/rsdl.hpp fieldRunners/StubbornRunners.cpp fieldRunners/StubbornRunners.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/StubbornRunners.cpp -o StubbornRunners.o

SuperTroopers.o:  src/rsdl.hpp fieldRunners/SuperTroopers.cpp fieldRunners/SuperTroopers.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/SuperTroopers.cpp -o SuperTroopers.o

Shot.o: src/rsdl.hpp fieldRunners/Shot.cpp fieldRunners/Shot.hpp fieldRunners/Enemies.hpp
	$(CC) -c fieldRunners/Shot.cpp -o Shot.o

rsdl.o: src/rsdl.hpp src/rsdl.cpp
	$(CC) -c src/rsdl.cpp -o rsdl.o

.PHONY: clean
clean:
	rm -r *.o
