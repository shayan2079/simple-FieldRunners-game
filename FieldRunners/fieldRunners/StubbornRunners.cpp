#include "StubbornRunners.hpp"
#include <chrono>


using namespace std;

#define START_HEALTH_BAR 50
#define UP "up"
#define LEFT "left"
#define RIGHT "right"
#define DOWN "down"
#define HEALTH_BAR_WIDTH 4
#define ARRIVE "arrive"
#define GLUE_SHOT_KIND "Glue"
#define GATLING_SHOT_KIND "Gatling"
#define STUBBORN_RUNNERS_LENGTH 30
#define STUBBORN_RUNNERS_WIDTH 30
#define MAX_STEPS 10
#define STUBBORN_RUNNERS_SPEED 3

int StubbornRunners::isGoodTime()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - start_time >= enemies_period ||  draw_direction == "")
  {
    start_time = current_time;
    return 1;
  }
  return 0;
}

void StubbornRunners::decreaseEnemyHealth(const int &tower_damage, const string &shot_kind, const int& tower_level)
{
  if (shot_kind != GLUE_SHOT_KIND && shot_kind != GATLING_SHOT_KIND)
  {
    if (health_bar - START_HEALTH_BAR * tower_damage / health <= 0)
    {
      health_bar = 0;
      return;
    }
    health_bar -= START_HEALTH_BAR * tower_damage / health;
  }
}

int StubbornRunners::findDirectionIndex()
{
  if (draw_direction == DOWN)
    return 1;
  if (draw_direction == RIGHT)
    return 0;
  if (draw_direction == LEFT)
    return 2;
  if (draw_direction == UP)
    return 3;
}

void StubbornRunners::drawEnemy(Window *window)
{
  if (situation != ARRIVE)
  {
    auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    int i = this->findDirectionIndex();
    if (draw_direction == LEFT)
    {
      window->draw_img("MapImages/Enemies/StubbornRunners/right/" + to_string(steps[i]) + ".png", Rectangle(coordinate.x - STUBBORN_RUNNERS_LENGTH / 2, coordinate.y - STUBBORN_RUNNERS_WIDTH / 2, STUBBORN_RUNNERS_LENGTH, STUBBORN_RUNNERS_WIDTH), NULL_RECT , 0, 1);
    }
    else
    {
      window->draw_img("MapImages/Enemies/StubbornRunners/" + draw_direction +"/" + to_string(steps[i]) + ".png", Rectangle(coordinate.x - STUBBORN_RUNNERS_LENGTH / 2, coordinate.y - STUBBORN_RUNNERS_WIDTH / 2, STUBBORN_RUNNERS_LENGTH, STUBBORN_RUNNERS_WIDTH));
    }
    if (health_bar > 0)
    {
      window->draw_rect(Rectangle(coordinate.x - START_HEALTH_BAR / 2, coordinate.y - STUBBORN_RUNNERS_WIDTH / 2 - HEALTH_BAR_WIDTH, health_bar, HEALTH_BAR_WIDTH), RED);
    }

  }
}

void StubbornRunners::updateSteps()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  int i = this->findDirectionIndex();
  if (current_time - draw_period >= fix_step_period)
  {
    steps[i] = (steps[i] + 1) % number_of_steps[i];
    draw_period = current_time;
  }
}

void StubbornRunners::setCoordinate(const Point movement, const string direction)
{
  start_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (draw_direction == "")
  {
    coordinate.x += movement.x;
    coordinate.y += movement.y;
  }
  else
  {
    coordinate.x += (movement.x * STUBBORN_RUNNERS_SPEED);
    coordinate.y += (movement.y * STUBBORN_RUNNERS_SPEED);
  }
  draw_direction = direction;
}
