#ifndef GLUE

#define GLUE "GLUE"

#include "../src/rsdl.hpp"
#include "Towers.hpp"

class Glue : public Towers
{
public:
  Glue (const Point coordinate);
  void getDirection();
  void drawTower(Window *window);
  void shootEnemy(Window *window);
  void checkStopShooting(const std::vector<Enemies*> &enemies, Window *window);
  void checkShootingConditions(Window *window);
private:

};



#endif
