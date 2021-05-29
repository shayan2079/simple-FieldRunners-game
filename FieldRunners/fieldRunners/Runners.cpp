#include "Runners.hpp"
#include <chrono>
#include <iostream>

#define RUNNERS_HEALTH 250
#define RUNNERS_SPEED 50
#define RUNNERS_DEATH_PRIZE 6
#define RUNNERS_SURVIVATION_DAMAGE 1
#define RUNNERS_PERIOD 20
#define RUNNERS_NUMBER_OF_RIGHT_STEPS 12
#define RUNNERS_NUMBER_OF_UP_STEPS 12
#define RUNNERS_NUMBER_OF_DOWN_STEPS 12
#define GLUE_EFFICACY_TIME 2000
#define GLUE_EFFICACY_UPGRADE 500
#define START_HEALTH_BAR 50
#define UP "up"
#define LEFT "left"
#define RIGHT "right"
#define DOWN "down"
#define RUNNERS_FIX_STEPS_PERIOD 50
#define HEALTH_BAR_WIDTH 4
#define ARRIVE "arrive"
#define GLUE_SHOT_KIND "Glue"
#define RUNNERS_UP_LENGTH 24
#define RUNNERS_UP_WIDTH 32
#define RUNNERS_RIGHT_LENGTH 30
#define RUNNERS_RIGHT_WIDTH 30
#define RUNNERS_DOWN_LENGTH 24
#define RUNNERS_DOWN_WIDTH 26
#define RUNNERS_LEFT_LENGTH 30
#define RUNNERS_LEFT_WIDTH 30

using namespace std;

Runners::Runners(const int wave)
        :Enemies(RUNNERS_HEALTH, RUNNERS_SPEED, RUNNERS_DEATH_PRIZE, RUNNERS_SURVIVATION_DAMAGE, RUNNERS_NUMBER_OF_RIGHT_STEPS, RUNNERS_NUMBER_OF_UP_STEPS, RUNNERS_NUMBER_OF_DOWN_STEPS, wave, RUNNERS_PERIOD, RUNNERS_FIX_STEPS_PERIOD)
{}

void Runners::slowEnemy(const int &tower_damage, const int& tower_level)
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

void Runners::decreaseEnemyHealth(const int &tower_damage, const string &shot_kind, const int& tower_level)
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

int Runners::findDirectionIndex()
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

void Runners::drawEnemy(Window *window)
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
      window->draw_img("MapImages/Enemies/Runners/right/" + to_string(steps[i]) + ".png", Rectangle(coordinate.x - RUNNERS_LEFT_LENGTH / 2, coordinate.y - RUNNERS_LEFT_WIDTH / 2, RUNNERS_LEFT_LENGTH, RUNNERS_LEFT_WIDTH), NULL_RECT , 0, 1);
      if (health_bar > 0)
      {
        window->draw_rect(Rectangle(coordinate.x - START_HEALTH_BAR / 2, coordinate.y - RUNNERS_LEFT_WIDTH / 2 - HEALTH_BAR_WIDTH, health_bar, HEALTH_BAR_WIDTH), RED);
      }
    }
    else if (draw_direction == UP)
    {
      window->draw_img("MapImages/Enemies/Runners/" + draw_direction +"/" + to_string(steps[i]) + ".png", Rectangle(coordinate.x - RUNNERS_UP_LENGTH / 2, coordinate.y - RUNNERS_UP_WIDTH / 2, RUNNERS_UP_LENGTH, RUNNERS_UP_WIDTH));
      if (health_bar > 0)
      {
        window->draw_rect(Rectangle(coordinate.x - START_HEALTH_BAR / 2, coordinate.y - RUNNERS_UP_WIDTH / 2 - HEALTH_BAR_WIDTH, health_bar, HEALTH_BAR_WIDTH), RED);
      }
    }
    else if (draw_direction == DOWN)
    {
      window->draw_img("MapImages/Enemies/Runners/" + draw_direction +"/" + to_string(steps[i]) + ".png", Rectangle(coordinate.x - RUNNERS_DOWN_LENGTH / 2, coordinate.y - RUNNERS_DOWN_WIDTH / 2, RUNNERS_DOWN_LENGTH, RUNNERS_DOWN_WIDTH));
      if (health_bar > 0)
      {
        window->draw_rect(Rectangle(coordinate.x - START_HEALTH_BAR / 2, coordinate.y - RUNNERS_DOWN_WIDTH / 2 - HEALTH_BAR_WIDTH, health_bar, HEALTH_BAR_WIDTH), RED);
      }
    }
    else
    {
      window->draw_img("MapImages/Enemies/Runners/right/" + to_string(steps[i]) + ".png", Rectangle(coordinate.x - RUNNERS_RIGHT_LENGTH / 2, coordinate.y - RUNNERS_RIGHT_WIDTH / 2, RUNNERS_RIGHT_LENGTH, RUNNERS_RIGHT_WIDTH));
      if (health_bar > 0)
      {
        window->draw_rect(Rectangle(coordinate.x - START_HEALTH_BAR / 2, coordinate.y - RUNNERS_RIGHT_WIDTH / 2 - HEALTH_BAR_WIDTH, health_bar, HEALTH_BAR_WIDTH), RED);
      }
    }

  }
}

void Runners::updateSteps()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  int i = this->findDirectionIndex();
  if (current_time - draw_period >= fix_step_period)
  {
    steps[i] = (steps[i] + 1) % number_of_steps[i];
    draw_period = current_time;
  }
}

int Runners::isGoodTime()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - start_time >= enemies_period ||  draw_direction == "")
  {
    start_time = current_time;
    return 1;
  }
  return 0;
}

void Runners::setCoordinate(const Point movement, const string direction)
{
  start_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  coordinate.x += movement.x;
  coordinate.y += movement.y;
  draw_direction = direction;
}
