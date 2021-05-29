#include "Enemies.hpp"
#include <sstream>
#include <iostream>
#include <math.h>

#define CENTRE_OF_FIRST_BLOCK_X 150
#define CENTRE_OF_FIRST_BLOCK_Y 205
#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define PI 3.14159265
#define ALIVE "alive"
#define DEAD "dead"
#define START_HEALTH_BAR 50
#define TOWER_RANGE 150

using namespace std;

Enemies::Enemies(const int enemy_health, const int enemy_speed, const int enemy_death_prize, const int enemy_survivation_damage, const int number_of_right_steps, const int number_of_up_steps, const int number_of_down_steps, const int wave, const int period, const int step_period)
{
  health_bar = START_HEALTH_BAR;
  health = (0.9 + 0.1 * wave) * enemy_health;
  speed = enemy_speed;
  death_prize = enemy_death_prize;
  survivation_damage = enemy_survivation_damage;
  coordinate = Point(0, 0);
  path_index = 0;
  draw_period = 0;
  fix_step_period = fix_step_original_period = step_period;
  enemies_original_period = enemies_period = period;
  steps[0] = steps[1] = steps[2] = steps[3] = 0;
  number_of_steps[0] = number_of_steps[2] = number_of_right_steps;
  number_of_steps[3] = number_of_up_steps;
  number_of_steps[1] = number_of_down_steps;
  situation = ALIVE;
}

int Enemies::isEnemyMatch(const string enemy_situation)
{
  if (enemy_situation == situation)
    return 1;
  return 0;
}

int Enemies::isEnemyArrive(const Point &end_coordinate)
{
  if (end_coordinate.x * BLOCK_LENGTH + CENTRE_OF_FIRST_BLOCK_X == coordinate.x && end_coordinate.y * BLOCK_WIDTH + CENTRE_OF_FIRST_BLOCK_Y == coordinate.y)
    return 1;
  return 0;
}

void Enemies::setEnemySituation(const string enemy_situation)
{
  situation = enemy_situation;
}

int Enemies::getPathIndex()
{
  path_index++;
  return path_index - 1;
}

int Enemies::isInCentreOfBlock()
{
  if ((coordinate.x - CENTRE_OF_FIRST_BLOCK_X) % BLOCK_LENGTH == 0 && (coordinate.y - CENTRE_OF_FIRST_BLOCK_Y) % BLOCK_WIDTH == 0)
    return 1;
  return 0;
}

float Enemies::getDistance(const Point &tower_coordinate)
{
  if (situation == ALIVE)
    return ((tower_coordinate.x - coordinate.x)*(tower_coordinate.x - coordinate.x) + (tower_coordinate.y - coordinate.y)*(tower_coordinate.y - coordinate.y));
  return TOWER_RANGE * TOWER_RANGE + 1;
}

float Enemies::getAngle(const Point &tower_coordinate)
{
  return atan2 (coordinate.y - tower_coordinate.y, (tower_coordinate.x - coordinate.x)) * 180 / PI;
}

int Enemies::checkEnemySituation()
{
  if (health_bar <= 0 && situation != DEAD)
  {
    situation = DEAD;
    return death_prize;
  }
  return 0;
}
