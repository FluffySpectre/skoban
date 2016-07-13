#ifndef SKOBAN_H
#define SKOBAN_H

#include <allegro.h>
#include <fstream>

using namespace std;

// Kartengröße
#define MAP_WIDTH       32
#define MAP_HEIGHT      24

// Tilegröße
#define TILE_WIDTH      20
#define TILE_HEIGHT     20

// Kartentiles
#define SKY              1
#define BRICK            2
#define FLOOR            3
#define HOLE             4

// Objekte
#define PLAYER         100
#define BOX            101

void Setup();
void LoadMap(const char *path);
void DrawMap();
void MovePlayer();

#endif
