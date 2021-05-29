#include "Scramblers.hpp"
#include <chrono>
#include <iostream>


using namespace std;

#define GLUE_EFFICACY_TIME 2000
#define GLUE_EFFICACY_UPGRADE 500
#define START_HEALTH_BAR 50
#define UP "up"
#define LEFT "left"
#define RIGHT "right"
#define DOWN "down"
#define HEALTH_BAR_WIDTH 4
#define ARRIVE "arrive"
#define GLUE_SHOT_KIND "Glue"
#define SCRAMBLERS_LENGTH 44
#define SCRAMBLERS_WIDTH 44
#define MAX_STEPS 10

int Scramblers::isGoodTime()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - start_time >= enemies_period ||  draw_direction == "")
  {
    start_time = current_time;
    return 1;
  }
  return 0;
}

int Scramblers::findDirectionIndex()
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

void Scramblers::drawEnemy(Window *window)
{
  if (situation != ARRIVE)
  {
    auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
    int i = this->findDirectionIndex();
    if (current_time - glue_time >= glue_period && enemies_period != enemies_original_period)
    {
      enemies_period = enemies_original_period;
      fix_step_period = fix_step_original_period;
    }
    if (draw_direction == LEFT)
    {
      window->draw_img("MapImages/Enemies/Scramblers/right/" + to_string(steps[i]) + ".png", Rectangle(coordinate.x - SCRAMBLERS_LENGTH / 2, coordinate.y - SCRAMBLERS_LENGTH / 2, SCRAMBLERS_LENGTH, SCRAMBLERS_WIDTH), NULL_RECT , 0, 1);
    }
    else
    {
      window->draw_img("MapImages/Enemies/Scramblers/" + draw_direction +"/" + to_string(steps[i]) + ".png", Rectangle(coordinate.x - SCRAMBLERS_LENGTH / 2, coordinate.y - SCRAMBLERS_LENGTH / 2, SCRAMBLERS_LENGTH, SCRAMBLERS_WIDTH));
    }
    if (health_bar > 0)
    {
      window->draw_rect(Rectangle(coordinate.x - START_HEALTH_BAR / 2, coordinate.y - SCRAMBLERS_LENGTH / 2 - HEALTH_BAR_WIDTH, health_bar, HEALTH_BAR_WIDTH), RED);
    }

  }
}

void Scramblers::updateSteps()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  int i = this->findDirectionIndex();
  if (current_time - draw_period >= fix_step_period)
  {
    steps[i] = (steps[i] + 1) % number_of_steps[i];
    draw_period = current_time;
  }
}

void Scramblers::slowEnemy(const int &tower_damage, const int& tower_level)
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (enemies_period == enemies_original_period * 100 / (100 - tower_damage) || enemies_period == enemies_original_period || (enemies_period != enemies_original_period && current_time - glue_time <=10 && enemies_period < enemies_original_period * 100 / (100 - tower_damage)))
  {
    glue_time = current_time;
    fix_step_period = fix_step_original_period * 100 / (100 - tower_damage);
    glue_period = GLUE_EFFICACY_TIME + tower_level * GLUE_EFFICACY_UPGRADE;
    enemies_period = enemies_original_period * 100 / (100 - tower_damage);
  }
}

void Scramblers::decreaseEnemyHealth(const int &tower_damage, const string &shot_kind, const int& tower_level)
{
  if (shot_kind == GLUE_SHOT_KIND)
  {
    this->slowEnemy(tower_damage, tower_level);
    return;
  }
  if (health_bar - START_HEALTH_BAR * tower_damage / health <= 0)
  {
    health_bar = 0;
    return;
  }
  health_bar -= START_HEALTH_BAR * tower_damage / health;
}

void Scramblers::setCoordinate(const Point movement, const string direction)
{
  start_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (steps_counter % MAX_STEPS != 0)
  {
    coordinate.x += movement.x;
    coordinate.y += movement.y;
  }
  steps_counter = (steps_counter + 1) % MAX_STEPS;
  draw_direction = direction;
}
