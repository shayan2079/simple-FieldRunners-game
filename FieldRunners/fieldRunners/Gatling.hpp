#ifndef GATLING

#define GATLING "GATLING"

#include "../src/rsdl.hpp"
#include "Towers.hpp"

class Gatling : public Towers
{
public:
  Gatling (const Point coordinate);
  void getDirection();
  void drawTower(Window *window);
  void checkShootingConditions(Window *window);
  void shootEnemy(Window *window);
  void fixSituation();
  void checkStopShooting(const std::vector<Enemies*> &enemies, Window *window);
private:

};



#endif
