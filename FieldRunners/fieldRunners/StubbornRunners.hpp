#ifndef STUBBORN_RUNNERS

#define STUBBORN_RUNNERS "STUBBORN_RUNNERS"

#include "Enemies.hpp"
#include <string>
#include "../src/rsdl.hpp"

#define STUBBORN_RUNNERS_HEALTH 400
#define STUBBORN_RUNNER_SPEED 30
#define STUBBORN_RUNNERS_DEATH_PRIZE 10
#define STUBBORN_RUNNERS_SURVIVATION_DAMAGE 4
#define STUBBORN_RUNNERS_NUMBER_OF_RIGHT_STEPS 15
#define STUBBORN_RUNNERS_NUMBER_OF_UP_STEPS 15
#define STUBBORN_RUNNERS_NUMBER_OF_DOWN_STEPS 16
#define STUBBORN_RUNNERS_PERIOD 100
#define STUBBORN_RUNNERS_FIX_STEPS_PERIOD 50

class StubbornRunners : public Enemies
{
public:
  StubbornRunners (const int wave)
    :Enemies(STUBBORN_RUNNERS_HEALTH, STUBBORN_RUNNER_SPEED, STUBBORN_RUNNERS_DEATH_PRIZE, STUBBORN_RUNNERS_SURVIVATION_DAMAGE, STUBBORN_RUNNERS_NUMBER_OF_RIGHT_STEPS, STUBBORN_RUNNERS_NUMBER_OF_UP_STEPS, STUBBORN_RUNNERS_NUMBER_OF_DOWN_STEPS, wave, STUBBORN_RUNNERS_PERIOD, STUBBORN_RUNNERS_FIX_STEPS_PERIOD)
  {
  }
  int isGoodTime();
  void setCoordinate(const Point movement, const std::string direction);
  void drawEnemy(Window *window);
  int findDirectionIndex();
  void updateSteps();
  void decreaseEnemyHealth(const int &tower_damage, const std::string &shot_kind, const int& tower_level);
private:
};

#endif
