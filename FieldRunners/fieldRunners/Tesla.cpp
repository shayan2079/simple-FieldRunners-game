#include "Tesla.hpp"
#include "Towers.hpp"
#include "../src/rsdl.hpp"
#include <iostream>
#include <chrono>

#define TESLA_DAMAGE 700
#define TESLA_DAMAGE_RANGE 0
#define TESLA_UPGRADE_DAMAGE 1000
#define TESLA_CONSTRUCTION_COST 120
#define TESLA_UPGRADE_COST 100
#define TESLA_DELAY_PER_ATTACK 1500
#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define TESLA_SHOT_KIND "Tesla"
#define ALIVE "alive"
#define ARRIVE "arrive"
#define NORMAL_SITUATION "normal"
#define SHOOTING_SITUATION "shooting"
#define SHOW_SHOOTING_SITUATION_TIME 100

using namespace std;

Tesla::Tesla(const Point coordinate)
        :Towers(coordinate, TESLA_DAMAGE, TESLA_CONSTRUCTION_COST, TESLA_UPGRADE_COST, TESLA_DELAY_PER_ATTACK, TESLA_DAMAGE_RANGE, TESLA_UPGRADE_DAMAGE)
{}

void Tesla::getDirection()
{
}

void Tesla::drawTower(Window *window)
{
  if (shot != NULL)
    shot->drawShot(window);
  window->draw_img("MapImages/Towers/Tesla/" + to_string(level) + "/" + situation + "/1.png", Rectangle(coordinate.x - BLOCK_LENGTH / 2, coordinate.y - BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH));
}

void Tesla::shootEnemy(Window *window)
{
  window->play_sound_effect("Sounds/shooting.mp3");
  Shot* new_shot = new Shot(locked_enemy, coordinate, damage_per_attack, damage_range, TESLA, level);
  shot = new_shot;
}

void Tesla::checkStopShooting(const vector<Enemies*> &enemies, Window *window)
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
  if (locked_enemy == NULL)
  {
    situation = NORMAL_SITUATION;
    return;
  }
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - fire_passed_time >= SHOW_SHOOTING_SITUATION_TIME || !locked_enemy->isEnemyMatch(ALIVE))
  {
    situation = NORMAL_SITUATION;
  }
}

void Tesla::checkShootingConditions(Window *window)
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - fire_passed_time >= delay_per_attack && locked_enemy != NULL)
  {
    this->shootEnemy(window);
    situation = SHOOTING_SITUATION;
    fire_passed_time = current_time;
  }
}
