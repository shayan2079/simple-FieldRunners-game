#include "Map.hpp"
#include "../src/rsdl.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <chrono>

using namespace std;

#define MAP_LENGTH 1365
#define MAP_WIDTH 1024
#define CENTRE_OF_FIRST_BLOCK_X 150
#define CENTRE_OF_FIRST_BLOCK_Y 205
#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define FIRST_BLOCK_X 120
#define FIRST_BLOCK_Y 175
#define LAST_BLOCK_X 1260
#define LAST_BLOCK_Y 835
#define GATLING_KEY1 'g'
#define GATLING_KEY2 'G'
#define MISSILE_KEY1 'm'
#define MISSILE_KEY2 'M'
#define GLUE_KEY1 'l'
#define GLUE_KEY2 'L'
#define TESLA_KEY1 't'
#define TESLA_KEY2 'T'
#define UPGRADE_KEY1 'u'
#define UPGRADE_KEY2 'U'
#define ATTACK_RADIUS 150
#define CHOOSE_ENEMIES_PERIOD 500
#define WAVE_PERIOD 3000
#define ALIVE "alive"
#define ARRIVE "arrive"
#define WIN "you win"
#define LOSE "you lose"
#define HEALTH_FONT_SIZE 20
#define MONEY_FONT_SIZE 20
#define PLAYER_START_HEALTH 20
#define TOWER_RANGE 150
#define PLAYER_START_MONEY 200
#define END_FONT_START_SIZE 100
#define NUMBER_OF_ENEMIES_KIND 4
#define HEALTH_X 300
#define HEALTH_Y 70
#define MONEY_X 100
#define MONEY_Y 70
#define GATLING_CONSTRUCTION_COST 55
#define MISSILE_CONSTRUCTION_COST 70
#define GLUE_CONSTRUCTION_COST 60
#define TESLA_CONSTRUCTION_COST 120
#define END_TEXT_PERIOD 6
#define END_TEXT_MAX_SIZE 350
#define END_TEXT_COORDINATE_X 100
#define END_TEXT_COORDINATE_Y 100
#define UP "up"
#define LEFT "left"
#define RIGHT "right"
#define DOWN "down"
#define WAVE_X 500
#define WAVE_Y 70
#define WAVE_FONT_SIZE 20

Map::Map(vector<Point> path)
{
  length = MAP_LENGTH;
  width = MAP_WIDTH;
  first_block_coordinates = Point(FIRST_BLOCK_X, FIRST_BLOCK_Y);
  block_width = BLOCK_WIDTH;
  block_length = BLOCK_LENGTH;
  enemies_path = path;
  arrow = Arrows(path[0], path[(int)path.size() - 1]);
  tower_coordinate = Point(-1, -1);
  player_health = PLAYER_START_HEALTH;
  money = PLAYER_START_MONEY;
  end_wave_time = 0;
  end_font_edit_time = 0;
  end_font_size = END_FONT_START_SIZE;
}

void Map::getEnemiesInformation()
{
  this->getEnemiesPath();
  this->getEnemiesWavesInformation();
}

void Map::getEventsAndUpdateMap(Window *window)
{
  if (player_situation == "")
    this->checkEndConditions();
  this->getEvents(window);
  if (player_situation == "")
    this->updateGame(window);
}

void Map::checkEndConditions()
{
  this->checkLoseCondition();
  if (player_situation == "")
    this->checkWinCondition();
}

void Map::checkLoseCondition()
{
  if (player_health <= 0)
  {
    player_situation = LOSE;
  }
}

void Map::checkWinCondition()
{
  if (wave + 1 == enemies.size())
  {
    for (size_t i = 0; i < choosen_enemies.size(); i++)
      if (choosen_enemies[i]->isEnemyMatch(ALIVE))
        return;
    for (size_t i = 0; i < NUMBER_OF_ENEMIES_KIND; i++)
      if (enemies[wave][i].size() > 0)
        return;
    player_situation = WIN;
  }
}

void Map::updateGame(Window *window)
{
  this->updateTowers(window);
  this->updateEnemies();
}

void Map::updateTowers(Window *window)
{
  this->checkTowersLockedEnemy();
  this->chooseTowersLockedEnemy();
  this->shootEnemies(window);
  this->getTowersAngles();
}

void Map::updateEnemies()
{
  this->checkEndWave();
  this->checkChooseEnemiesTime();
  this->checkEnemiesSituation();
  this->setEnemiesCoordinate();
  this->setEnemiesStep();
}

void Map::checkEndWave()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (choosen_enemies.size() != 0 && this->isWaveEnds() && current_time - end_wave_time >= WAVE_PERIOD && enemies.size() >= wave + 2)
  {
    this->deleteEnemies();
    end_wave_time = 0;
    wave++;
  }
}

void Map::setEnemiesStep()
{
  for (size_t i = 0; i < choosen_enemies.size(); i++)
  {
    choosen_enemies[i]->updateSteps();
  }
}

void Map::checkChooseEnemiesTime()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if ((current_time - draw_enemy_start_time >= CHOOSE_ENEMIES_PERIOD || (choosen_enemies.size() == 0 && end_wave_time == 0)) && this->isEnemyRemain())
  {
    choosen_enemies.push_back(this->chooseEnemies());
    draw_enemy_start_time = current_time;
  }
}

void Map::checkEnemiesSituation()
{
  for (size_t i = 0; i < choosen_enemies.size(); i++)
    if (choosen_enemies[i]->isEnemyMatch(ALIVE))
    {
      money += choosen_enemies[i]->checkEnemySituation();
      if (choosen_enemies[i]->isEnemyArrive(enemies_path[enemies_path.size() - 1]))
      {
        choosen_enemies[i]->setEnemySituation(ARRIVE);
        player_health -= choosen_enemies[i]->getEnemySurvivationDamage();
      }
    }
}

vector<Point> Map::completeDistance(Point first_block, Point second_block, int x_increaser, int y_increaser)
{
  vector<Point> path;
  while (first_block.x + x_increaser != second_block.x || first_block.y + y_increaser != second_block.y)
  {
    first_block.x += x_increaser;
    first_block.y += y_increaser;
    path.push_back(first_block);
  }
  return path;
}

int Map::notAdjacent(const Point &first_block, const Point &second_block)
{
  if (abs((first_block.x - second_block.x)) + abs((first_block.y - second_block.y)) > 1)
    return 1;
  return 0;
}

vector<Point> Map::distinguishDirection(const Point &first_block, const Point &second_block)
{
  int x_increaser, y_increaser;
  if (first_block.x == second_block.x && first_block.y < second_block.y)
  {
    x_increaser = 0;
    y_increaser = 1;
  }
  if (first_block.x == second_block.x && first_block.y > second_block.y)
  {
    x_increaser = 0;
    y_increaser = -1;
  }
  if (first_block.x < second_block.x && first_block.y == second_block.y)
  {
    x_increaser = 1;
    y_increaser = 0;
  }
  if (first_block.x > second_block.x && first_block.y == second_block.y)
  {
    x_increaser = -1;
    y_increaser = 0;
  }
  return completeDistance(first_block, second_block, x_increaser, y_increaser);
}

vector<Point> Map::completePath(const vector<Point> &path)
{
  vector<Point> completed_path;
  completed_path.push_back(path[0]);
  for (size_t i = 0; i < path.size() - 1; i++)
  {
    if (notAdjacent(path[i], path[i + 1]))
    {
      vector<Point> distance_path = distinguishDirection(path[i], path[i + 1]);
      completed_path.insert(completed_path.end(), distance_path.begin(), distance_path.end());
    }
    completed_path.push_back(path[i + 1]);
  }
  return completed_path;
}

void Map::separateCoordinates(stringstream &coordinates)
{
  int x_coordinate, y_coordinate;
  vector<Point> path;
  while (coordinates>>x_coordinate>>y_coordinate)
  {
    path.push_back(Point(x_coordinate, y_coordinate));
  }
  *this = Map(completePath(path));
}

void Map::getEnemiesWavesInformation()
{
  int number_of_runners, number_of_stubborn_runners, number_of_super_troopers, number_of_scramblers;
  wave = 0;
  while (cin>>number_of_runners>>number_of_stubborn_runners>>number_of_super_troopers>>number_of_scramblers)
    this->makeNewEnemies(number_of_runners, number_of_stubborn_runners, number_of_super_troopers, number_of_scramblers);
}

void Map::makeNewEnemies(const int number_of_runners, const int number_of_stubborn_runners, const int number_of_super_troopers, const int number_of_scramblers)
{
  vector<Enemies*> enemy;
  vector< vector<Enemies*> > new_enemies;
  new_enemies.push_back(enemy);
  new_enemies.push_back(enemy);
  new_enemies.push_back(enemy);
  new_enemies.push_back(enemy);
  for (int i = 0; i < number_of_runners; i++)
  {
    Runners *runner = new Runners((int)(enemies.size() + 1));
    new_enemies[0].push_back(runner);
  }
  for (int i = 0; i < number_of_stubborn_runners; i++)
  {
    StubbornRunners *stubborn_runner = new StubbornRunners((int)(enemies.size() + 1));
    new_enemies[1].push_back(stubborn_runner);
  }
  for (int i = 0; i < number_of_super_troopers; i++)
  {
    SuperTroopers *super_trooper = new SuperTroopers((int)(enemies.size() + 1));
    new_enemies[2].push_back(super_trooper);
  }
  for (int i = 0; i < number_of_scramblers; i++)
  {
    Scramblers *scrambler = new Scramblers((int)(enemies.size() + 1));
    new_enemies[3].push_back(scrambler);
  }
  enemies.push_back(new_enemies);
}

void Map::getEnemiesPath()
{
  string first_line;
  stringstream coordinates;
  getline(cin, first_line);
  coordinates<<first_line;
  this->separateCoordinates(coordinates);
}

void Map::findNearestEnemy(const int tower_index, const Point &coordinate)
{
  float distance = TOWER_RANGE * TOWER_RANGE + 1, new_distance;
  int enemy_index;
  for (size_t i = 0; i < choosen_enemies.size(); i++)
    if ((new_distance = choosen_enemies[i]->getDistance(coordinate)) <= distance)
    {
      distance = new_distance;
      enemy_index = i;
    }
  if (distance <= TOWER_RANGE * TOWER_RANGE)
    towers[tower_index]->getNewEnemy(choosen_enemies[enemy_index]);
}

void Map::checkTowersLockedEnemy()
{
  for (size_t i = 0; i < towers.size(); i++)
  {
    if (!towers[i]->isFree())
      towers[i]->checkDistace();
  }
}

void Map::chooseTowersLockedEnemy()
{
  for (size_t i = 0; i < towers.size(); i++)
  {
    Point coordinate = towers[i]->getCoordinate();
    if (towers[i]->isFree())
      this->findNearestEnemy(i, coordinate);
  }
}

void Map::shootEnemies(Window *window)
{
  for (size_t i = 0; i < towers.size(); i++)
  {
    if (!towers[i]->isFree())
      towers[i]->checkShootingConditions(window);
    towers[i]->checkStopShooting(choosen_enemies, window);
  }
}

void Map::getTowersAngles()
{
  for (size_t i = 0; i < towers.size(); i++)
    if (!towers[i]->isFree())
      towers[i]->getDirection();
}

void Map::drawTowers(Window *window)
{
  for (size_t i = 0; i < towers.size(); i++)
  {
    towers[i]->drawTower(window);
  }
}

void Map::deleteAllEnemies()
{
  this->deleteEnemies();
  for (size_t i = 0; i < enemies.size(); i++)
    for (size_t j = 0; j < enemies[i].size(); j++)
      for (size_t k = 0; k < enemies[i][j].size(); k++)
        delete enemies[i][j][k];
  enemies.clear();
}

void Map::deleteTowers()
{
  for (size_t i = 0; i < towers.size(); i++)
    delete towers[i];
  towers.clear();
}

void Map::deleteAll()
{
  this->deleteAllEnemies();
  this->deleteTowers();
}

void Map::drawBackGround(Window *window)
{
  window->draw_img("MapImages/background.png", Rectangle(0, 0, MAP_LENGTH, MAP_WIDTH));
  window->show_text("Wave: " + to_string(wave + 1), Point(WAVE_X, WAVE_Y), BLACK, "FreeSans.ttf", WAVE_FONT_SIZE);
  window->show_text("Health: " + to_string(player_health), Point(HEALTH_X, HEALTH_Y), BLACK, "FreeSans.ttf", HEALTH_FONT_SIZE);
  window->show_text("Money: " + to_string(money), Point(MONEY_X, MONEY_Y), BLACK, "FreeSans.ttf", MONEY_FONT_SIZE);
}

void Map::drawTowerRange(Window *window)
{
  if (tower_coordinate.x != -1)
    window->draw_img("MapImages/attack_radius.png", Rectangle(CENTRE_OF_FIRST_BLOCK_X - ATTACK_RADIUS + BLOCK_LENGTH * tower_coordinate.x, CENTRE_OF_FIRST_BLOCK_Y - ATTACK_RADIUS + BLOCK_WIDTH * tower_coordinate.y, 2 * ATTACK_RADIUS, 2 * ATTACK_RADIUS));
}

void Map::drawGameMap(Window *window)
{
  this->drawBackGround(window);
  this->drawPath(window);
  arrow.drawArrows(window, enemies_path);
  this->drawTowerRange(window);
  this->drawTowers(window);
  this->drawEnemies(window);
}

void Map::drawEndGame(Window *window)
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (end_font_size < END_TEXT_MAX_SIZE && current_time - end_font_edit_time >= END_TEXT_PERIOD)
  {
    end_font_size ++;
    end_font_edit_time = current_time;
  }
  window->show_text(player_situation, Point(END_TEXT_COORDINATE_X, END_TEXT_COORDINATE_Y), BLACK, "FreeSans.ttf", end_font_size);
}

void Map::drawMap(Window *window)
{
  window->clear();
  this->drawGameMap(window);
  if (player_situation != "")
    this->drawEndGame(window);
  window->update_screen();
}

Enemies* Map::chooseEnemies()
{
  int random_number;
  srand(chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count());
  random_number = rand() % NUMBER_OF_ENEMIES_KIND;
  while (enemies[wave][random_number].size() == 0 )
    random_number = rand() % NUMBER_OF_ENEMIES_KIND;
  Enemies* enemy = enemies[wave][random_number][0];
  enemies[wave][random_number].erase(enemies[wave][random_number].begin());
  return enemy;
}

int Map::isEnemyRemain()
{
  for (size_t i = 0; i < NUMBER_OF_ENEMIES_KIND; i++)
  {
    if (enemies[wave][i].size() > 0)
      return 1;
  }
  return 0;
}

int Map::isWaveEnds()
{
  for (size_t i = 0; i < choosen_enemies.size(); i++)
    if (choosen_enemies[i]->isEnemyMatch(ALIVE))
      return 0;
  if (end_wave_time == 0)
    end_wave_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  return 1;
}

void Map::deleteEnemies()
{
  for (size_t i = 0; i < choosen_enemies.size(); i++)
    delete choosen_enemies[i];
  choosen_enemies.clear();
}

void Map::drawEnemies(Window *window)
{
  for (size_t i = choosen_enemies.size() - 1; i != -1; i--)
  {
    if ((choosen_enemies[i]->isEnemyMatch(ALIVE)))
      choosen_enemies[i]->drawEnemy(window);
  }
}

void Map::setEnemiesCoordinate()
{
  for (size_t i = 0; i < choosen_enemies.size(); i++)
    if (choosen_enemies[i]->isEnemyMatch(ALIVE) && choosen_enemies[i]->isGoodTime())
    {
      string enemy_draw_direction = choosen_enemies[i]->getDrawDirection();
      this->setCoordinate(i, enemy_draw_direction);
    }
}

void Map::setCoordinate(const int & i, const string &enemy_draw_direction)
{
  if (enemy_draw_direction == "")
  {
    this->setFirstCoordinate(i);
    return;
  }
  this->setNewCoordinate(i, enemy_draw_direction);
}

Point Map::getBlockCoordinate(const Point &block)
{
  return Point(block.x * BLOCK_LENGTH + CENTRE_OF_FIRST_BLOCK_X, block.y * BLOCK_WIDTH + CENTRE_OF_FIRST_BLOCK_Y);
}

void Map::setFirstCoordinate(const int & i)
{
  if (enemies_path.size() == 1)
  {
    choosen_enemies[i]->setCoordinate(getBlockCoordinate(enemies_path[0]), RIGHT);
    return;
  }
  if (enemies_path[0].x < enemies_path[1].x)
    choosen_enemies[i]->setCoordinate(getBlockCoordinate(enemies_path[0]), RIGHT);
  if (enemies_path[0].x > enemies_path[1].x)
    choosen_enemies[i]->setCoordinate(getBlockCoordinate(enemies_path[0]), LEFT);
  if (enemies_path[0].y < enemies_path[1].y)
    choosen_enemies[i]->setCoordinate(getBlockCoordinate(enemies_path[0]), DOWN);
  if (enemies_path[0].y > enemies_path[1].y)
    choosen_enemies[i]->setCoordinate(getBlockCoordinate(enemies_path[0]), UP);
}

void Map::setNewCoordinate(const int & i, const string &enemy_draw_direction)
{
  if (choosen_enemies[i]->isInCentreOfBlock())
  {
    this->setDirectionAndMoveEnemy(i, enemy_draw_direction);
    return;
  }
  this->moveEnemy(i, enemy_draw_direction);
}

void Map::moveEnemy(const int & i, const string &enemy_draw_direction)
{
  if (enemy_draw_direction == RIGHT)
    choosen_enemies[i]->setCoordinate(Point (1, 0), RIGHT);
  if (enemy_draw_direction == LEFT)
    choosen_enemies[i]->setCoordinate(Point (-1, 0), LEFT);
  if (enemy_draw_direction == UP)
    choosen_enemies[i]->setCoordinate(Point (0, -1), UP);
  if (enemy_draw_direction == DOWN)
    choosen_enemies[i]->setCoordinate(Point (0, 1), DOWN);
}

void Map::setDirectionAndMoveEnemy(const int & i, const string &enemy_draw_direction)
{
  Point enemy_coordinate = choosen_enemies[i]->getCoordinate();
  int index = choosen_enemies[i]->getPathIndex();
  if (enemy_draw_direction == RIGHT)
  {
    if (enemies_path[index].x < enemies_path[index + 1].x)
      choosen_enemies[i]->setCoordinate(Point (1, 0), RIGHT);
    if (enemies_path[index].y < enemies_path[index + 1].y)
      choosen_enemies[i]->setCoordinate(Point (0, 1), DOWN);
    if (enemies_path[index].y > enemies_path[index + 1].y)
      choosen_enemies[i]->setCoordinate(Point (0, -1), UP);
  }
  if (enemy_draw_direction == LEFT)
  {
    if (enemies_path[index].x > enemies_path[index + 1].x)
      choosen_enemies[i]->setCoordinate(Point (-1, 0), LEFT);
    if (enemies_path[index].y < enemies_path[index + 1].y)
      choosen_enemies[i]->setCoordinate(Point (0, 1), DOWN);
    if (enemies_path[index].y > enemies_path[index + 1].y)
      choosen_enemies[i]->setCoordinate(Point (0, -1), UP);
  }
  if (enemy_draw_direction == UP)
  {
    if (enemies_path[index].x < enemies_path[index + 1].x)
      choosen_enemies[i]->setCoordinate(Point (1, 0), RIGHT);
    if (enemies_path[index].y > enemies_path[index + 1].y)
      choosen_enemies[i]->setCoordinate(Point (0, -1), UP);
    if (enemies_path[index].x > enemies_path[index + 1].x)
      choosen_enemies[i]->setCoordinate(Point (-1, 0), LEFT);
  }
  if (enemy_draw_direction == DOWN)
  {
    if (enemies_path[index].x < enemies_path[index + 1].x)
      choosen_enemies[i]->setCoordinate(Point (1, 0), RIGHT);
    if (enemies_path[index].y < enemies_path[index + 1].y)
      choosen_enemies[i]->setCoordinate(Point (0, 1), DOWN);
    if (enemies_path[index].x > enemies_path[index + 1].x)
      choosen_enemies[i]->setCoordinate(Point (-1, 0), LEFT);
  }
}

void Map::drawPath(Window *window)
{
  for (size_t i = 0; i < enemies_path.size(); i++)
  {
    window->draw_img("MapImages/block.png", Rectangle(FIRST_BLOCK_X + BLOCK_LENGTH * enemies_path[i].x, FIRST_BLOCK_Y + BLOCK_WIDTH * enemies_path[i].y, BLOCK_LENGTH, BLOCK_WIDTH));
  }
}

int Map::isInPath(const Point mouse_position)
{
  for (size_t i = 0; i < enemies_path.size(); i++)
  {
    if (enemies_path[i].x == mouse_position.x && enemies_path[i].y == mouse_position.y)
    {
      return 1;
    }
  }
  return 0;
}

int Map::isTowerCoordinate()
{

  for (size_t i = 0; i < towers.size(); i++)
  {
    if(towers[i]->isTowerCoordinate(tower_coordinate))
    {
      return 1;
    }
  }
  return 0;
}

void Map::makeNewGatling()
{
  if (money >= GATLING_CONSTRUCTION_COST)
  {
    money -= GATLING_CONSTRUCTION_COST;
    Gatling *gatling = new Gatling(Point (tower_coordinate.x * BLOCK_LENGTH + CENTRE_OF_FIRST_BLOCK_X, tower_coordinate.y * BLOCK_WIDTH + CENTRE_OF_FIRST_BLOCK_Y));
    towers.push_back(gatling);
  }
}

void Map::makeNewMissile()
{
  if (money >= MISSILE_CONSTRUCTION_COST)
  {
    money -= MISSILE_CONSTRUCTION_COST;
    Missile *missile = new Missile(Point (tower_coordinate.x * BLOCK_LENGTH + CENTRE_OF_FIRST_BLOCK_X, tower_coordinate.y * BLOCK_WIDTH + CENTRE_OF_FIRST_BLOCK_Y));
    towers.push_back(missile);
  }
}

void Map::makeNewGlue()
{
  if (money >= GLUE_CONSTRUCTION_COST)
  {
    money -= GLUE_CONSTRUCTION_COST;
    Glue *glue = new Glue(Point (tower_coordinate.x * BLOCK_LENGTH + CENTRE_OF_FIRST_BLOCK_X, tower_coordinate.y * BLOCK_WIDTH + CENTRE_OF_FIRST_BLOCK_Y));
    towers.push_back(glue);
  }
}

void Map::makeNewTesla()
{
  if (money >= TESLA_CONSTRUCTION_COST)
  {
    money -= TESLA_CONSTRUCTION_COST;
    Tesla *tesla = new Tesla(Point (tower_coordinate.x * BLOCK_LENGTH + CENTRE_OF_FIRST_BLOCK_X, tower_coordinate.y * BLOCK_WIDTH + CENTRE_OF_FIRST_BLOCK_Y));
    towers.push_back(tesla);
  }
}

void Map::upgradeTower()
{
  for (size_t i = 0; i < towers.size(); i++)
  {
    if (towers[i]->isEnoughMoney(money) && towers[i]->isTowerCoordinate(tower_coordinate))
    {
      money -= towers[i]->upgradeTower();
      return;
    }
  }
}


void Map::chooseTower(const char &pressed_key, Window *window)
{
  if (tower_coordinate.x == -1)
    return;
  if (!isTowerCoordinate())
  {
    if (pressed_key == GATLING_KEY1 || pressed_key == GATLING_KEY2)
      this->makeNewGatling();
    if (pressed_key == MISSILE_KEY1 || pressed_key == MISSILE_KEY2)
      this->makeNewMissile();
    if (pressed_key == GLUE_KEY1 || pressed_key == GLUE_KEY2)
      this->makeNewGlue();
    if (pressed_key == TESLA_KEY1 || pressed_key == TESLA_KEY2)
      this->makeNewTesla();
  }
  if (isTowerCoordinate() && (pressed_key == UPGRADE_KEY1 || pressed_key == UPGRADE_KEY2))
  {
    this->upgradeTower();
  }
  tower_coordinate.x = -1;
}

void Map::CheckMouseCoordinate(const Point &mouse_position)
{
  if (!this->isInPath(Point((mouse_position.x - FIRST_BLOCK_X) / BLOCK_LENGTH, (mouse_position.y - FIRST_BLOCK_Y) / BLOCK_WIDTH)) && mouse_position.x >= FIRST_BLOCK_X && mouse_position.x <= LAST_BLOCK_X && mouse_position.y >= FIRST_BLOCK_Y && mouse_position.y <= LAST_BLOCK_Y)
    tower_coordinate = Point((mouse_position.x - FIRST_BLOCK_X) / BLOCK_LENGTH, (mouse_position.y - FIRST_BLOCK_Y) / BLOCK_WIDTH);
}

void Map::getEvents(Window *window)
{
  if (window->has_pending_event()){
    Event event = window->poll_for_event();
    if (event.get_type() == Event::QUIT)
    {
      this->deleteAll();
      exit(0);
    }
    if (event.get_type() == Event::LCLICK && player_situation == "")
    {
      Point mouse_position = event.get_mouse_position();
      this->CheckMouseCoordinate(mouse_position);
    }
    if (event.get_type() == Event::KEY_PRESS && player_situation == "")
    {
      char pressed_key = event.get_pressed_key();
      this->chooseTower(pressed_key, window);
    }
  }
}
