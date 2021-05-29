#include "Arrows.hpp"
#include <algorithm>
#include "../src/rsdl.hpp"
#include <iostream>
#include <chrono>

using namespace std;

#define CENTRE_OF_FIRST_BLOCK_X 150
#define CENTRE_OF_FIRST_BLOCK_Y 205
#define BLOCK_WIDTH 60
#define BLOCK_LENGTH 60
#define ARROW_MOVMENT 15
#define ARROW_LENGTH 8
#define FIRST_BLOCK_X 120
#define FIRST_BLOCK_Y 175
#define ARROWS_PERIOD 100
#define NUMBER_OF_ARROWS 3
#define DOWN "down"
#define RIGHT "right"
#define UP "up"
#define LEFT "left"

Arrows::Arrows (const Point &start_block, const Point &end_block)
{
  arrow_coordinate = start_block;
  start_coordinate = start_block;
  end_coordinate = end_block;
  start_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

void Arrows::fixLastArrowCoordinate(int index, const vector<Point> &path)
{
  Point block_coordinate = Point (CENTRE_OF_FIRST_BLOCK_X + BLOCK_LENGTH*path[index].x, CENTRE_OF_FIRST_BLOCK_Y + BLOCK_WIDTH*path[index].y);
  if ((draw_coordinates[2].x - CENTRE_OF_FIRST_BLOCK_X) % BLOCK_LENGTH == 0 && (draw_coordinates[2].y - CENTRE_OF_FIRST_BLOCK_Y) % BLOCK_WIDTH == 0)
    path_index ++;
  if (draw_direction[2] == RIGHT)
  {
    if (draw_coordinates[2].x + ARROW_MOVMENT > block_coordinate.x)
    {
      if (path[index].y < path[index +1].y)
      {
        draw_coordinates[2].y += ARROW_MOVMENT;
        draw_direction[2] = DOWN;
        return;
      }
      if (path[index].y > path[index +1].y)
      {
        draw_coordinates[2].y -= ARROW_MOVMENT;
        draw_direction[2] = UP;
        return;
      }
    }
    draw_coordinates[2].x += ARROW_MOVMENT;
    return;
  }
  if (draw_direction[2] == LEFT)
  {
    if (draw_coordinates[2].x - ARROW_MOVMENT < block_coordinate.x)
    {
      if (path[index].y < path[index +1].y)
      {
        draw_coordinates[2].y += ARROW_MOVMENT;
        draw_direction[2] = DOWN;
        return;
      }
      if (path[index].y > path[index +1].y)
      {
        draw_coordinates[2].y -= ARROW_MOVMENT;
        draw_direction[2] = UP;
        return;
      }
    }
    draw_coordinates[2].x -= ARROW_MOVMENT;
    return;
  }
  if (draw_direction[2] == DOWN)
  {
    if (draw_coordinates[2].y + ARROW_MOVMENT > block_coordinate.y)
    {
      if (path[index].x < path[index +1].x)
      {
        draw_coordinates[2].x += ARROW_MOVMENT;
        draw_direction[2] = RIGHT;
        return;
      }
      if (path[index].x > path[index +1].x)
      {
        draw_coordinates[2].x -= ARROW_MOVMENT;
        draw_direction[2] = LEFT;
        return;
      }
    }
    draw_coordinates[2].y += ARROW_MOVMENT;
    return;
  }
  if (draw_direction[2] == UP)
  {
    if (draw_coordinates[2].y - ARROW_MOVMENT < block_coordinate.y)
    {
      if (path[index].x < path[index +1].x)
      {
        draw_coordinates[2].x += ARROW_MOVMENT;
        draw_direction[2] = RIGHT;
        return;
      }
      if (path[index].x > path[index +1].x)
      {
        draw_coordinates[2].x -= ARROW_MOVMENT;
        draw_direction[2] = LEFT;
        return;
      }
    }
    draw_coordinates[2].y -= ARROW_MOVMENT;
    return;
  }
}

void Arrows::fixArrowsCoordinates(int index, const vector<Point> &path)
{
  draw_coordinates[0] = draw_coordinates[1];
  draw_coordinates[1] = draw_coordinates[2];
  draw_direction[0] = draw_direction[1];
  draw_direction[1] = draw_direction[2];
  arrow_coordinate = Point((draw_coordinates[0].x - FIRST_BLOCK_X) / BLOCK_LENGTH, (draw_coordinates[0].y - FIRST_BLOCK_Y) / BLOCK_WIDTH);
  if (index == path.size() - 1 && path[index - 1].x > path[index].x)
  {
    draw_coordinates[2] = Point (draw_coordinates[2].x - ARROW_MOVMENT, draw_coordinates[2].y);
    draw_direction[2] = LEFT;
    return;
  }
  if (index == path.size() - 1 && path[index - 1].x < path[index].x)
  {
    draw_coordinates[2] = Point (draw_coordinates[2].x + ARROW_MOVMENT, draw_coordinates[2].y);
    draw_direction[2] = RIGHT;
    return;
  }
  if (index == path.size() - 1 && path[index - 1].y > path[index].y)
  {
    draw_coordinates[2] = Point (draw_coordinates[2].x, draw_coordinates[2].y - ARROW_MOVMENT);
    draw_direction[2] = UP;
    return;
  }
  if (index == path.size() - 1 && path[index - 1].y < path[index].y)
  {
    draw_coordinates[2] = Point (draw_coordinates[2].x, draw_coordinates[2].y + ARROW_MOVMENT);
    draw_direction[2] = DOWN;
    return;
  }
  this->fixLastArrowCoordinate(index, path);
}

void Arrows::putFirstDirection(const vector<Point> &path)
{
  int square_coordinate_y = CENTRE_OF_FIRST_BLOCK_Y + start_coordinate.y * BLOCK_WIDTH, square_coordinate_x = CENTRE_OF_FIRST_BLOCK_X + start_coordinate.x * BLOCK_LENGTH;
  arrow_coordinate = start_coordinate;
  path_index = 1;
  if (1 == path.size())
  {
    draw_coordinates[0] = Point (square_coordinate_x - ARROW_MOVMENT, square_coordinate_y);
    draw_coordinates[1] = Point (square_coordinate_x, square_coordinate_y);
    draw_coordinates[2] = Point (square_coordinate_x + ARROW_MOVMENT, square_coordinate_y);
    draw_direction[0] = draw_direction[1] = draw_direction[2] = RIGHT;
    return;
  }
  if (path[0].x < path [1].x)
  {
    draw_direction[0] = draw_direction[1] = draw_direction[2] = RIGHT;
    draw_coordinates[0] = Point (square_coordinate_x - ARROW_MOVMENT, square_coordinate_y);
    draw_coordinates[1] = Point (square_coordinate_x, square_coordinate_y);
    draw_coordinates[2] = Point (square_coordinate_x + ARROW_MOVMENT, square_coordinate_y);
    return;
  }
  if (path[0].x > path [1].x)
  {
    draw_direction[0] = draw_direction[1] = draw_direction[2] = LEFT;
    draw_coordinates[0] = Point (square_coordinate_x + ARROW_MOVMENT, square_coordinate_y);
    draw_coordinates[1] = Point (square_coordinate_x, square_coordinate_y);
    draw_coordinates[2] = Point (square_coordinate_x - ARROW_MOVMENT, square_coordinate_y);
    return;
  }
  if (path[0].y < path [1].y)
  {
    draw_direction[0] = draw_direction[1] = draw_direction[2] = DOWN;
    draw_coordinates[0] = Point (square_coordinate_x, square_coordinate_y - ARROW_MOVMENT);
    draw_coordinates[1] = Point (square_coordinate_x, square_coordinate_y);
    draw_coordinates[2] = Point (square_coordinate_x, square_coordinate_y + ARROW_MOVMENT);
    return;
  }
  if (path[0].y > path [1].y)
  {
    draw_direction[0] = draw_direction[1] = draw_direction[2] = UP;
    draw_coordinates[0] = Point (square_coordinate_x, square_coordinate_y + ARROW_MOVMENT);
    draw_coordinates[1] = Point (square_coordinate_x, square_coordinate_y);
    draw_coordinates[2] = Point (square_coordinate_x, square_coordinate_y - ARROW_MOVMENT);
    return;
  }
}

int Arrows::isGoodTime()
{
  auto current_time = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
  if (current_time - start_time >= ARROWS_PERIOD ||  draw_direction[0] == "")
  {
    start_time = current_time;
    return 1;
  }
  return 0;
}

void Arrows::getArrowsCoordinates(const vector<Point> &path)
{
  if (!this->isGoodTime())
    return;
  if ((arrow_coordinate.x == end_coordinate.x && arrow_coordinate.y == end_coordinate.y) || path.size() == 1 || draw_direction[0] == "")
  {
    this->putFirstDirection(path);
    return;
  }
  this->fixArrowsCoordinates(path_index, path);
}

void Arrows::drawArrows(Window *window, vector<Point> enemies_path)
{
  this->getArrowsCoordinates(enemies_path);
  for (int i = 0; i < NUMBER_OF_ARROWS; i++)
  {
    window->draw_img(("MapImages/Arrows/" + draw_direction[i] + "/" + to_string(i + 2) + ".png"), Rectangle(draw_coordinates[i].x - ARROW_LENGTH / 2, draw_coordinates[i].y - ARROW_LENGTH / 2, ARROW_LENGTH, ARROW_LENGTH));
  }
}
