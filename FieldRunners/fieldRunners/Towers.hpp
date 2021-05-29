#ifndef TOWER

#define TOWER "TOWER"

#include "../src/rsdl.hpp"
#include "Enemies.hpp"
#include "Shot.hpp"

class Towers
{
public:
  Towers(){}
  Towers(const Point tower_coordinate, const int damage, const int tower_construction_cost, const int tower_upgrade_cost, const int tower_delay, const int tower_damage_range, const int tower_upgrade_damage);
  Point getCoordinate(){ return coordinate; }
  void getNewEnemy(Enemies* enemy);
  int isFree();
  void checkDistace();
  int isEnoughMoney(const int &money);
  int isTowerCoordinate(const Point &clicked_coordinate);
  int upgradeTower();
  virtual void checkStopShooting(const std::vector<Enemies*> &enemies, Window *window) = 0;
  virtual void checkShootingConditions(Window *window) = 0;
  virtual void getDirection() = 0;
  virtual void drawTower(Window *window) = 0;
protected:
  Point coordinate;
  int damage_range;
  int upgrade_damage;
  int damage_per_attack;
  int construction_cost;
  int upgrade_cost;
  int delay_per_attack;
  int level;
  double fire_passed_time;
  int direction;
  std::string situation;
  Enemies* locked_enemy;
  Shot* shot;
};

#endif
