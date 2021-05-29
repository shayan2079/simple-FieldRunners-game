#ifndef SUPER_TROOPERS

#define SUPER_TROOPERS "SUPER_TROOPERS"

#include "Enemies.hpp"
#include <string>
#include "../src/rsdl.hpp"

#define SUPER_TROOPERS_HEALTH 500
#define SUPER_TROOPERS_SPEED 25
#define SUPER_TROOPERS_DEATH_PRIZE 8
#define SUPER_TROOPERS_SURVIVATION_DAMAGE 4
#define SUPER_TROOPERS_NUMBER_OF_RIGHT_STEPS 20
#define SUPER_TROOPERS_NUMBER_OF_UP_STEPS 19
#define SUPER_TROOPERS_NUMBER_OF_DOWN_STEPS 15
#define SUPER_TROOPERS_PERIOD 40
#define SUPER_TROOPERS_FIX_STEPS_PERIOD 60

class SuperTroopers : public Enemies
{
public:
  SuperTroopers (const int wave)
    :Enemies(SUPER_TROOPERS_HEALTH, SUPER_TROOPERS_SPEED, SUPER_TROOPERS_DEATH_PRIZE, SUPER_TROOPERS_SURVIVATION_DAMAGE, SUPER_TROOPERS_NUMBER_OF_RIGHT_STEPS, SUPER_TROOPERS_NUMBER_OF_UP_STEPS, SUPER_TROOPERS_NUMBER_OF_DOWN_STEPS, wave, SUPER_TROOPERS_PERIOD, SUPER_TROOPERS_FIX_STEPS_PERIOD)
  {
  }
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
