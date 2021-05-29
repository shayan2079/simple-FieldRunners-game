#include "../src/rsdl.hpp"
#include "Map.hpp"
#include <iostream>


#define MAP_LENGTH 1365
#define MAP_WIDTH 1024



using namespace std;






int main()
{
  Map map;
  map.getEnemiesInformation();
  Window *window = new Window (MAP_LENGTH, MAP_WIDTH, "Field Runners");
  window->play_music("Sounds/ThemeSong.mp3");
  while (1)
  {
    map.getEventsAndUpdateMap(window);
    map.drawMap(window);
    delay(1);
  }
  return 0;
}
