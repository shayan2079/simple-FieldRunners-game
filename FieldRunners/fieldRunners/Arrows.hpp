#ifndef ARROWS

#define ARROWS "ARROWS"

#include "../src/rsdl.hpp"
#include <vector>

#define NUMBER_OF_ARROWS 3

class Arrows
{
public:
  Arrows() {}
  Arrows (const Point &start_block, const Point &end_block);
  void fixLastArrowCoordinate(int index, const std::vector<Point> &path);
  void fixArrowsCoordinates(int index, const std::vector<Point> &path);
  void putFirstDirection(const std::vector<Point> &path);
  void getArrowsCoordinates(const std::vector<Point> &path);
  void drawArrows(Window *window, std::vector<Point> enemies_path);
  Point getArrowCoordinate(){ return arrow_coordinate; }
  Point getStartCoordinate(){ return start_coordinate; }
  Point getEndCoordinate(){ return end_coordinate; }
  int isGoodTime();
private:
  Point arrow_coordinate;
  Point start_coordinate;
  Point end_coordinate;
  std::vector<Point> draw_coordinates{NUMBER_OF_ARROWS};
  std::vector<std::string> draw_direction{NUMBER_OF_ARROWS};
  int path_index;
  double start_time;
};

#endif
