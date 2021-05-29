#include "Towers.hpp"
#include "../src/rsdl.hpp"
#include <iostream>
#include <chrono>

using namespace std;

#define CENTRE_OF_FIRST_BLOCK_X 150
#define CENTRE_OF_FIRST_BLOCK_Y 205
#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define ALIVE "alive"
#define DEAD "dead"
#define ARRIVE "arrive"
#define START_SITUATION "normal"
#define START_LEVEL 1
#define MAX_LEVEL 3
#define TOWER_RANGE 150

Towers::Towers(const Point tower_coordinate, const int damage, const int tower_construction_cost, const int tower_upgrade_cost, const int tower_delay, const int tower_damage_range, const int tower_upgrade_damage)
{
  coordinate = tower_coordinate;
  damage_per_attack = damage;
  damage_range = tower_damage_range;
  construction_cost = tower_construction_cost;
  upgrade_cost = tower_upgrade_cost;
  delay_per_attack = tower_delay;
  upgrade_damage = tower_upgrade_damage;
  level = START_LEVEL;
  direction = 0;
  locked_enemy = NULL;
  fire_passed_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();;
  situation = START_SITUATION;
  shot = NULL;
}

int Towers::isEnoughMoney(const int &money)
{
  if (money >= upgrade_cost)
    return 1;
  return 0;
}

int Towers::isTowerCoordinate(const Point &clicked_coordinate)
{
  if (clicked_coordinate.x * BLOCK_LENGTH + CENTRE_OF_FIRST_BLOCK_X == coordinate.x && clicked_coordinate.y * BLOCK_WIDTH + CENTRE_OF_FIRST_BLOCK_Y == coordinate.y)
    return 1;
  return 0;
}

int Towers::upgradeTower()
{
  if (level < MAX_LEVEL)
  {
    level++;
    damage_per_attack += upgrade_damage;
    return upgrade_cost;
  }
  return 0;
}

void Towers::checkDistace()
{
  if ((locked_enemy->getDistance(coordinate)) > TOWER_RANGE * TOWER_RANGE || !locked_enemy->isEnemyMatch(ALIVE))
  {
    locked_enemy = NULL;
  }
}

void Towers::getNewEnemy(Enemies* enemy)
{

  locked_enemy = enemy;
}


int Towers::isFree()
{
  if (locked_enemy == NULL)
    return 1;
  return 0;
}
