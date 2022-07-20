#ifndef PATHFOUNDED_ENGINE_H
#define PATHFOUNDED_ENGINE_H

#include "DS/list_lib.h"
#define HEIGHT 30
#define WIDTH 75
#define background (char)219
#define RELATIONSIZE HEIGHT*WIDTH

typedef struct {
    int x;
    int y;
} point_t;

typedef struct {
    char** area;
    char** area_r;
    int** relationship;
    list_t* path;
    point_t begin_p;
    point_t goal_p;
} map_t;

map_t* map_init();
void set_begin(map_t* map, point_t point);
void set_goal(map_t* map, point_t point);
void map_show(map_t* map, int flag);
void map_delete(map_t* map);
void BFS_a(map_t* map);
#endif