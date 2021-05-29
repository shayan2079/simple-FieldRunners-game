#include "Shot.hpp"
#include "chrono"
#include "cmath"

using namespace std;

#define SHOT_RADIUS 5
#define SHOT_PERIOD 5
#define SHOT_SPEED 5
#define ALIVE "alive"
#define MISSILE_SHOT_KIND "Missile"

Shot::Shot (Enemies* enemy, const Point &first_coordinate, const int tower_damage, const int tower_damage_range, const string shot_kind, const int level)
{
  start_time = 0;
  locked_enemy = enemy;
  coordinate = first_coordinate;
  damage = tower_damage;
  damage_range = tower_damage_range;
  kind = shot_kind;
  tower_level = level;
}

int Shot::isGoodTime()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - start_time >= SHOT_PERIOD)
  {
    start_time = current_time;
    return 1;
  }
  return 0;
}

void Shot::shootEnemy()
{
  if (this->isGoodTime())
  {
    Point enemy_coordinate = locked_enemy->getEnemiesCoordinate();
    float angle = atan2 ((enemy_coordinate.y - coordinate.y), (enemy_coordinate.x - coordinate.x));
    coordinate = Point ((coordinate.x + SHOT_SPEED * cos(angle)), coordinate.y + SHOT_SPEED * sin(angle));
  }
}

int Shot::isEnemyMatch(const string enemy_situation)
{
  if (locked_enemy->isEnemyMatch(enemy_situation))
    return 1;
  return 0;
}

int Shot::isClash(const vector<Enemies*> &enemies)
{
  Point enemy_coordinate = locked_enemy->getEnemiesCoordinate();
  if (((enemy_coordinate.x < coordinate.x + SHOT_RADIUS && enemy_coordinate.x > coordinate.x - SHOT_RADIUS) && (enemy_coordinate.y < coordinate.y + SHOT_RADIUS && enemy_coordinate.y > coordinate.y - SHOT_RADIUS)))
  {
    if (damage_range > 0)
    {
      for (size_t i = 0; i < enemies.size(); i++)
        if (enemies[i]->isEnemyMatch(ALIVE) && enemies[i]->getDistance(enemy_coordinate) <= damage_range * damage_range)
          enemies[i]->decreaseEnemyHealth(damage, kind, tower_level);
    }
    else
      locked_enemy->decreaseEnemyHealth(damage, kind, tower_level);
    return 1;
  }
  return 0;
}

void Shot::drawShot(Window *window)
{
  window->fill_circle(coordinate, SHOT_RADIUS, BLACK);
}
