#ifndef ENEMIES

#define ENEMIES "ENEMIES"

#include <string>
#include "../src/rsdl.hpp"
#include <vector>

#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define FIRST_BLOCK_X 120
#define FIRST_BLOCK_Y 175

class Enemies
{
public:
  Enemies(const int enemy_health, const int enemy_speed, const int enemy_death_prize, const int enemy_survivation_damage, const int number_of_right_steps, const int number_of_up_steps, const int number_of_down_steps, const int wave, const int period, const int step_period);
  std::string getDrawDirection(){ return draw_direction; }
  Point getCoordinate(){ return Point((coordinate.x - FIRST_BLOCK_X) / BLOCK_LENGTH, (coordinate.y - FIRST_BLOCK_Y) / BLOCK_WIDTH); }
  int isInCentreOfBlock();
  int getPathIndex();
  void drawEnemy(Window *window, const int kind);
  int isEnemyArrive(const Point &end_coordinate);
  float getDistance(const Point &tower_coordinate);
  float getAngle(const Point &tower_coordinate);
  Point getEnemiesCoordinate(){ return coordinate; }
  void setEnemySituation(const std::string enemy_situation);
  int isEnemyMatch(const std::string enemy_situation);
  int checkEnemySituation();
  int getEnemySurvivationDamage(){ return survivation_damage; }
  virtual void updateSteps() = 0;
  virtual void decreaseEnemyHealth(const int &tower_damage, const std::string &shot_kind, const int& tower_level) = 0;
  virtual void drawEnemy(Window *window) = 0;
  virtual int isGoodTime() = 0;
  virtual void setCoordinate(const Point movement, const std::string direction) = 0;
protected:
  int health_bar;
  Point coordinate;
  int health;
  int speed;
  int path_index;
  int death_prize;
  int survivation_damage;
  int block_number;
  int fix_step_period;
  int fix_step_original_period;
  double draw_period;
  std::string draw_direction;
  std::string situation;
  double start_time;
  int enemies_period;
  double glue_time;
  int glue_period;
  int enemies_original_period;
  std::vector<int> number_of_steps{4};
  std::vector<int> steps{4};
};

#endif
