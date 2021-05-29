#ifndef SHOT

#define SHOT "SHOT"

#include "Enemies.hpp"
#include "../src/rsdl.hpp"

class Shot
{
public:
  Shot(Enemies* enemy, const Point &first_coordinate, const int tower_damage, const int tower_damage_range, const std::string shot_kind, const int level);
  Shot(){}
  void shootEnemy();
  void drawShot(Window *window);
  int isGoodTime();
  int isClash(const std::vector<Enemies*> &enemies);
  int isEnemyMatch(const std::string enemy_situation);
private:
  Point coordinate;
  int damage;
  int damage_range;
  int tower_level;
  Enemies* locked_enemy;
  double start_time;
  std::string kind;
};

#endif
