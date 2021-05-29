#include "Missile.hpp"
#include "Towers.hpp"
#include "../src/rsdl.hpp"
#include <iostream>
#include <chrono>

#define MISSILE_DAMAGE 75
#define MISSILE_CONSTRUCTION_COST 70
#define MISSILE_UPGRADE_COST 60
#define MISSILE_UPGRADE_DAMAGE 75
#define MISSILE_DELAY_PER_ATTACK 3000
#define MISSILE_DAMAGE_RANG 50
#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define NUMBER_OF_MISSILE_DIRECTION 36
#define MISSILE_SHOT_KIND "Missile"
#define ALIVE "alive"
#define ARRIVE "arrive"

using namespace std;

Missile::Missile(const Point coordinate)
        :Towers(coordinate, MISSILE_DAMAGE, MISSILE_CONSTRUCTION_COST, MISSILE_UPGRADE_COST, MISSILE_DELAY_PER_ATTACK, MISSILE_DAMAGE_RANG, MISSILE_UPGRADE_DAMAGE)
{}

void Missile::getDirection()
{
  float angle = locked_enemy->getAngle(coordinate);
  angle += 90;
  if (angle < 0)
    angle += 360;
  angle = 360 - angle;
  direction = (int)(angle / 360 * 36);
}

void Missile::drawTower(Window *window)
{
  if (shot != NULL)
    shot->drawShot(window);
  if (direction > NUMBER_OF_MISSILE_DIRECTION / 2)
  {
    window->draw_img("MapImages/Towers/Missile/" + to_string(level) + "/" + situation + "/" + to_string(NUMBER_OF_MISSILE_DIRECTION - direction +  1) + ".png", Rectangle(coordinate.x - BLOCK_LENGTH / 2, coordinate.y - BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH), NULL_RECT, 0, 1);
  }
  else
  {
    window->draw_img("MapImages/Towers/Missile/" + to_string(level) + "/" + situation + "/" + to_string(direction +  1) + ".png", Rectangle(coordinate.x - BLOCK_LENGTH / 2, coordinate.y - BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH));
  }
}

void Missile::shootEnemy(Window *window)
{
  window->play_sound_effect("Sounds/shooting.mp3");
  Shot* new_shot = new Shot(locked_enemy, coordinate, damage_per_attack, damage_range, MISSILE_SHOT_KIND, level);
  shot = new_shot;
}

void Missile::checkStopShooting(const vector<Enemies*> &enemies, Window *window)
{
  if (shot != NULL)
  {
    shot->shootEnemy();
    if (shot->isClash(enemies) || shot->isEnemyMatch(ARRIVE))
    {
      window->play_sound_effect("Sounds/Explosion.mp3");
      delete shot;
      shot = NULL;
    }
  }
}

void Missile::checkShootingConditions(Window *window)
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - fire_passed_time >= delay_per_attack && locked_enemy != NULL)
  {
    this->shootEnemy(window);
    fire_passed_time = current_time;
  }
}
