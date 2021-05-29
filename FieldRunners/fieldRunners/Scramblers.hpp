#ifndef SCRAMBLERS

#define SCRAMBLERS "SCRAMBLERS"

#include "Enemies.hpp"
#include <string>
#include "../src/rsdl.hpp"

#define SCRAMBLERS_HEALTH 100
#define SCRAMBLERS_SPEED 90
#define SCRAMBLERS_DEATH_PRIZE 4
#define SCRAMBLERS_SURVIVATION_DAMAGE 2
#define SCRAMBLERS_NUMBER_OF_RIGHT_STEPS 4
#define SCRAMBLERS_NUMBER_OF_UP_STEPS 4
#define SCRAMBLERS_NUMBER_OF_DOWN_STEPS 4
#define SCRAMBLERS_PERIOD 10
#define SCRAMBLERS_FIX_STEPS_PERIOD 50

class Scramblers : public Enemies
{
public:
  Scramblers (const int wave)
    :Enemies(SCRAMBLERS_HEALTH, SCRAMBLERS_SPEED, SCRAMBLERS_DEATH_PRIZE, SCRAMBLERS_SURVIVATION_DAMAGE, SCRAMBLERS_NUMBER_OF_RIGHT_STEPS, SCRAMBLERS_NUMBER_OF_UP_STEPS, SCRAMBLERS_NUMBER_OF_DOWN_STEPS, wave, SCRAMBLERS_PERIOD, SCRAMBLERS_FIX_STEPS_PERIOD)
  {
    steps_counter = 1;
  }
  int isGoodTime();
  void setCoordinate(const Point movement, const std::string direction);
  void drawEnemy(Window *window);
  int findDirectionIndex();
  void updateSteps();
  void slowEnemy(const int &tower_damage, const int& tower_level);
  void decreaseEnemyHealth(const int &tower_damage, const std::string &shot_kind, const int& tower_level);
private:
  int steps_counter;
};

#endif
