#ifndef MISSILE

#define MISSILE "MISSILE"

#include "../src/rsdl.hpp"
#include "Towers.hpp"

class Missile : public Towers
{
public:
  Missile (const Point coordinate);
  void getDirection();
  void drawTower(Window *window);
  void checkShootingConditions(Window *window);
  void shootEnemy(Window *window);
  void checkStopShooting(const std::vector<Enemies*> &enemies, Window *window);
private:

};



#endif
