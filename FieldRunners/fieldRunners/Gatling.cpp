#include "Gatling.hpp"
#include "Towers.hpp"
#include "../src/rsdl.hpp"
#include <iostream>
#include "chrono"

#define GATLING_DAMAGE 35
#define GATLING_UPGRADE_DAMAGE 35
#define GATLING_CONSTRUCTION_COST 55
#define GATLING_DAMAGE_RANG 0
#define GATLING_UPGRADE_COST 40
#define GATLING_DELAY_PER_ATTACK 1000
#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define ALIVE "alive"
#define ARRIVE "arrive"
#define GATLING_SHOT_KIND "Gatling"
#define NORMAL_SITUATION "normal"
#define SHOOTING_SITUATION "shooting"
#define NUMBER_OF_GATLING_DIRECTION 36
#define SHOW_SHOOTING_SITUATION_TIME 100

using namespace std;

Gatling::Gatling(const Point coordinate)
        :Towers(coordinate, GATLING_DAMAGE, GATLING_CONSTRUCTION_COST, GATLING_UPGRADE_COST, GATLING_DELAY_PER_ATTACK, GATLING_DAMAGE_RANG, GATLING_UPGRADE_DAMAGE)
{}

void Gatling::getDirection()
{
  float angle = locked_enemy->getAngle(coordinate);
  angle += 90;
  if (angle < 0)
    angle += 360;
  angle = 360 - angle;
  direction = (int)(angle / 360 * NUMBER_OF_GATLING_DIRECTION);
}

void Gatling::drawTower(Window *window)
{
  if (shot != NULL)
    shot->drawShot(window);
  if (direction > NUMBER_OF_GATLING_DIRECTION / 2)
  {
    window->draw_img("MapImages/Towers/Gatling/" + to_string(level) + "/" + situation + "/" + to_string(NUMBER_OF_GATLING_DIRECTION - direction +  1) + ".png", Rectangle(coordinate.x - BLOCK_LENGTH / 2, coordinate.y - BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH), NULL_RECT, 0, 1);
  }
  else
  {
    window->draw_img("MapImages/Towers/Gatling/" + to_string(level) + "/" + situation + "/" + to_string(direction +  1) + ".png", Rectangle(coordinate.x - BLOCK_LENGTH / 2, coordinate.y - BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH));
  }
}

void Gatling::shootEnemy(Window *window)
{
  window->play_sound_effect("Sounds/shooting.mp3");
  Shot* new_shot = new Shot(locked_enemy, coordinate, damage_per_attack, damage_range, GATLING_SHOT_KIND, level);
  shot = new_shot;
}

void Gatling::checkStopShooting(const vector<Enemies*> &enemies, Window *window)
{
  if (shot != NULL)
  {
    shot->shootEnemy();
    if (shot->isClash(enemies) || shot->isEnemyMatch(ARRIVE))
    {
      delete shot;
      shot = NULL;
    }
  }
  this->fixSituation();
}

void Gatling::fixSituation()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (locked_enemy == NULL || current_time - fire_passed_time >= SHOW_SHOOTING_SITUATION_TIME || !locked_enemy->isEnemyMatch(ALIVE))
  {
    situation = NORMAL_SITUATION;
    return;
  }
}

void Gatling::checkShootingConditions(Window *window)
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - fire_passed_time >= delay_per_attack && locked_enemy != NULL)
  {
    this->shootEnemy(window);
    situation = SHOOTING_SITUATION;
    fire_passed_time = current_time;
  }
}
