#ifndef MAP

#define MAP "MAP"

#include <vector>
#include "../src/rsdl.hpp"
#include "Arrows.hpp"
#include "Towers.hpp"
#include "Gatling.hpp"
#include "Missile.hpp"
#include "Glue.hpp"
#include "Tesla.hpp"
#include "Enemies.hpp"
#include "Runners.hpp"
#include "StubbornRunners.hpp"
#include "SuperTroopers.hpp"
#include "Scramblers.hpp"
#include <chrono>

class Map
{
public:
  void getEnemiesPath();
  Map(){}
  Map(std::vector<Point> path);
  int getLength(){ return length; }
  int getWidth(){ return width; }
  Point getFirstBlockCoordinates(){ return first_block_coordinates; }
  int getBlockLength(){ return block_length; }
  int getBlockWidth(){ return block_width; }
  void updateGame(Window *window);
  void updateTowers(Window *window);
  void updateEnemies();
  void checkEndWave();
  void checkChooseEnemiesTime();
  void checkEnemiesSituation();
  std::vector<Point> getPath(){ return enemies_path; }
  void drawBackGround(Window *window);
  void drawTowerRange(Window *window);
  void drawGameMap(Window *window);
  void drawEndGame(Window *window);
  void drawMap(Window *window);
  void chooseTower(const char &pressed_key, Window *window);
  void getEvents(Window *window);
  void drawTowers(Window *window);
  void getEnemiesInformation();
  void getEnemiesWavesInformation();
  void drawEnemies(Window *window);
  void setCoordinate(const int &i, const std::string &enemy_draw_direction);
  Point getBlockCoordinate(const Point &block);
  void setFirstCoordinate(const int & i);
  void setNewCoordinate(const int & i, const std::string &enemy_draw_direction);
  void moveEnemy(const int & i, const std::string &enemy_draw_direction);
  void setDirectionAndMoveEnemy(const int & i, const std::string &enemy_draw_direction);
  int isEnemyRemain();
  Enemies* chooseEnemies();
  int isInPath(const Point mouse_position);
  int isTowerCoordinate();
  void CheckMouseCoordinate(const Point &mouse_position);
  void getEventsAndUpdateMap(Window *window);
  void checkLoseCondition();
  void checkWinCondition();
  void setEnemiesStep();
  void makeNewGatling();
  void makeNewMissile();
  void makeNewGlue();
  void setEnemiesCoordinate();
  void makeNewTesla();
  void checkTowersLockedEnemy();
  void shootEnemies(Window *window);
  void findNearestEnemy(const int tower_index, const Point &coordinate);
  void chooseTowersLockedEnemy();
  void getTowersAngles();
  int isWaveEnds();
  void deleteEnemies();
  void deleteAllEnemies();
  void deleteTowers();
  void deleteAll();
  void checkEndConditions();
  void upgradeTower();
  void makeNewEnemies(const int number_of_runners, const int number_of_stubborn_runners, const int number_of_super_troopers, const int number_of_scramblers);
private:
  int length;
  int width;
  Point first_block_coordinates;
  int block_length;
  int block_width;
  Arrows arrow;
  double end_wave_time;
  std::string player_situation;
  std::vector<Towers*> towers;
  std::vector<Point> enemies_path;
  std::vector<std::vector<std::vector<Enemies*> > > enemies;
  int wave;
  double end_font_edit_time;
  int end_font_size;
  int player_health;
  int money;
  double draw_enemy_start_time;
  std::vector<Enemies*> choosen_enemies;
  Point tower_coordinate;
  void separateCoordinates(std::stringstream &coordinates);
  void drawPath(Window *window);
  std::vector<Point> completePath(const std::vector<Point> &path);
  std::vector<Point> distinguishDirection(const Point &first_block, const Point &second_block);
  int notAdjacent(const Point &first_block, const Point &second_block);
  std::vector<Point> completeDistance(Point first_block, Point second_block, int x_increaser, int y_increaser);
};

#endif
