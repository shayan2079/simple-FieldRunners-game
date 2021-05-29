#ifndef RUNNERS

#define RUNNERS "RENNERS"

#include "Enemies.hpp"
#include <vector>
#include <string>
#include "../src/rsdl.hpp"



class Runners: public Enemies
{
public:
  Runners(const int wave);
  int isGoodTime();
  void setCoordinate(const Point movement, const std::string direction);
  void drawEnemy(Window *window);
  int findDirectionIndex();
  void updateSteps();
  void slowEnemy(const int &tower_damage, const int& tower_level);
  void decreaseEnemyHealth(const int &tower_damage, const std::string &shot_kind, const int& tower_level);
private:
};

#endif
