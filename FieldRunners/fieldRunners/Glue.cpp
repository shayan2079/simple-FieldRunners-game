#include "Glue.hpp"
#include "Towers.hpp"
#include "../src/rsdl.hpp"
#include <iostream>
#include <chrono>

#define GLUE_DAMAGE 40
#define GLUE_CONSTRUCTION_COST 60
#define GLUE_UPGRADE_COST 45
#define GLUE_UPGRADE_DAMAGE 10
#define GLUE_DELAY_PER_ATTACK 2000
#define GLUE_DAMAGE_RANG 30
#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define NUMBER_OF_GLUE_DIRECTION 36
#define GLUE_SHOT_KIND "Glue"
#define ALIVE "alive"
#define ARRIVE "arrive"

using namespace std;

Glue::Glue(const Point coordinate)
        :Towers(coordinate, GLUE_DAMAGE, GLUE_CONSTRUCTION_COST, GLUE_UPGRADE_COST, GLUE_DELAY_PER_ATTACK, GLUE_DAMAGE_RANG, GLUE_UPGRADE_DAMAGE)
{}

void Glue::getDirection()
{
  float angle = locked_enemy->getAngle(coordinate);
  angle += 90;
  if (angle < 0)
    angle += 360;
  angle = 360 - angle;
  direction = (int)(angle / 360 * NUMBER_OF_GLUE_DIRECTION);
}

void Glue::drawTower(Window *window)
{
  if (shot != NULL)
    shot->drawShot(window);
  if (direction > NUMBER_OF_GLUE_DIRECTION / 2)
  {
    window->draw_img("MapImages/Towers/Glue/" + to_string(level) + "/" + situation + "/" + to_string(NUMBER_OF_GLUE_DIRECTION - direction +  1) + ".png", Rectangle(coordinate.x - BLOCK_LENGTH / 2, coordinate.y - BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH), NULL_RECT, 0, 1);
  }
  else
  {
    window->draw_img("MapImages/Towers/Glue/" + to_string(level) + "/" + situation + "/" + to_string(direction +  1) + ".png", Rectangle(coordinate.x - BLOCK_LENGTH / 2, coordinate.y - BLOCK_WIDTH / 2, BLOCK_LENGTH, BLOCK_WIDTH));
  }
}

void Glue::shootEnemy(Window *window)
{
  window->play_sound_effect("Sounds/shooting.mp3");
  Shot* new_shot = new Shot(locked_enemy, coordinate, damage_per_attack, damage_range, GLUE_SHOT_KIND, level);
  shot = new_shot;
}

void Glue::checkStopShooting(const vector<Enemies*> &enemies, Window *window)
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
}

void Glue::checkShootingConditions(Window *window)
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - fire_passed_time >= delay_per_attack && locked_enemy != NULL)
  {
    this->shootEnemy(window);
    fire_passed_time = current_time;
  }
}
