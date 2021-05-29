#ifndef TESLA

#define TESLA "TESLA"

#include "../src/rsdl.hpp"
#include "Towers.hpp"

class Tesla : public Towers
{
public:
  Tesla (const Point coordinate);
  void getDirection();
  void drawTower(Window *window);
  void checkShootingConditions(Window *window);
  void shootEnemy(Window *window);
  void checkStopShooting(const std::vector<Enemies*> &enemies, Window *window);
private:

};



#endif
