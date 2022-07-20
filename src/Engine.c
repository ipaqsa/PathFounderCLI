#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Engine.h"
#include "DS/queue_lib.h"
#include "DS/list_lib.h"

void color_set(int color){
    int bg = 0;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | color));
}

typedef enum Colors {
    Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray,
    DarkGray, LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
} color_t;

void convert_to_point(point_t* point, int el) {
    point->x = el % WIDTH;
    point->y = el / WIDTH;
}

void map_read(map_t* map) {
    FILE *file = fopen("../data/map.txt", "r");
    char buffer[75];
    int line = 0;
    while (!feof(file)) {
        fgets(buffer, 75, file);
        for (int i = 0; i < 75; ++i) {
            if (buffer[i] == '#') map->area_r[line][i] = 'w';
        }
        line++;
    }
    fclose(file);
}

map_t* map_init() {
    map_t* map = malloc(sizeof(map_t));
    map->begin_p.x = 0;
    map->begin_p.y = 0;
    map->goal_p.x = WIDTH-1;
    map->goal_p.y = HEIGHT-1;
    map->area = malloc(sizeof(char*)*HEIGHT);
    map->area_r = malloc(sizeof(char*)*HEIGHT);
    map->relationship = malloc(sizeof(int*)*RELATIONSIZE);
    map->path = list_init(0);
    for (int y = 0; y < RELATIONSIZE; y++) {
        map->relationship[y] = malloc(sizeof(int)*RELATIONSIZE);
    }

    for (int y = 0; y < HEIGHT; y++) {
        map->area[y] = malloc(sizeof(char)*WIDTH);
        map->area_r[y] = malloc(sizeof(char)*WIDTH);
    }

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            map->area[y][x] = background;
            if (y == map->begin_p.y && x == map->begin_p.x)
                map->area_r[y][x] = 'b';
            else if (y == map->goal_p.y && x == map->goal_p.x)
                map->area_r[y][x] = 'g';
            else
                map->area_r[y][x] = 'n';
        }
    }

    for (int y = 0; y < RELATIONSIZE; y++)
        for (int x = 0; x < RELATIONSIZE; x++)
            map->relationship[y][x] = 0;

    map_read(map);
    return map;
}

void set_begin(map_t* map, point_t point) {
    map->area_r[map->begin_p.y][map->begin_p.x] = 'n';
    map->begin_p = point;
    map->area_r[point.y][point.x] = 'b';
}

void set_goal(map_t* map, point_t point) {
    map->area_r[map->goal_p.y][map->goal_p.x] = 'n';
    map->goal_p = point;
    map->area_r[map->goal_p.y][map->goal_p.x] = 'g';
}



void map_show(map_t* map, int flag) {
    if (flag != 2) {
        for (int y = 0; y < HEIGHT; y++) {
            printf("\n");
            for (int x = 0; x < WIDTH; x++) {
                if (flag) {
                    if (map->area_r[y][x] == 'b') color_set(Blue);
                    else if (map->area_r[y][x] == 'w') color_set(Black);
                    else if (map->area_r[y][x] == 'g') color_set(Red);
                    else if (map->area_r[y][x] == 'p') color_set(Green);
                    else color_set(White);
                    printf("%c", map->area[y][x]);
                }
                else
                    printf("%c", map->area_r[y][x]);
            }
        }
    }
    else {
        for (int y = 0; y < RELATIONSIZE; ++y) {
            printf("\n");
            for (int x = 0; x < RELATIONSIZE; x++) {
                printf("%d ", map->relationship[y][x]);
            }
        }
    }
}
int convert(point_t* point) {
    return point->y*WIDTH+point->x;
}


void set_relationship(map_t* map, point_t* point_o, point_t* point_t) {
    if (map->area_r[point_o->y][point_o->x] != 'w' && map->area_r[point_t->y][point_t->x] != 'w') {
        int coord_o = convert(point_o);
        int coord_t = convert(point_t);
        map->relationship[coord_o][coord_t] = 1;
        map->relationship[coord_t][coord_o] = 1;
    }
}

void cell_relationship(map_t* map, point_t* point) {
    int x_is_null = point->x == 0 ? 1 : 0;
    int y_is_null = point->y == 0 ? 1 : 0;
    int x_is_end = point->x == WIDTH-1 ? 1 : 0;
    int y_is_end = point->y == HEIGHT-1 ? 1 : 0;
    if (x_is_null && y_is_null) {
//        LeftAngleHigh
        point_t points[2] = {point->x+1, point->y, point->x, point->y+1};
        for (int i = 0; i < 2; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
    else if (x_is_null && y_is_end) {
//        LeftAngleLow
        point_t points[2] = {point->x+1, point->y, point->x,point->y-1};
        for (int i = 0; i < 2; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
    else if (x_is_end && y_is_null) {
//        RightAngleHigh
        point_t points[2] = {point->x-1, point->y, point->x, point->y+1};
        for (int i = 0; i < 2; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
    else if (x_is_end && y_is_end) {
//        RightAngleLow
        point_t points[2] = {point->x-1, point->y, point->x, point->y-1};
        for (int i = 0; i < 2; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
    else if (!x_is_null && !x_is_end && y_is_null) {
//        HighArea
        point_t points[3] = {point->x+1, point->y, point->x-1, point->y, point->x, point->y+1};
        for (int i = 0; i < 3; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
    else if (!x_is_null && !x_is_end && y_is_end) {
//        LowArea
        point_t points[3] = {point->x+1, point->y, point->x-1, point->y, point->x, point->y-1};
        for (int i = 0; i < 3; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
    else if (x_is_null && !y_is_null && !y_is_end) {
//        LeftArea
        point_t points[3] = {point->x+1, point->y, point->x, point->y+1, point->x, point->y-1};
        for (int i = 0; i < 3; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
    else if (x_is_end && !y_is_null && !y_is_end) {
//        RightArea
        point_t points[3] = {point->x-1, point->y, point->x, point->y+1, point->x, point->y-1};
        for (int i = 0; i < 3; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
    else {
//        CenterArea
        point_t points[4] = {point->x+1, point->y, point->x-1, point->y, point->x, point->y+1, point->x, point->y-1};
        for (int i = 0; i < 4; i++) {
            set_relationship(map, point, &points[i]);
        }
    }
}

void map_relationship(map_t* map) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            point_t point = {x,y};
            cell_relationship(map,&point);
        }
    }
}

void get_relationship_a(map_t* map, int idx, list_t** result) {
    *result = list_init(0);
    for (int i = 0; i < RELATIONSIZE; i++)
        if (map->relationship[idx][i] == 1 && i != idx)
            list_add_to_end(*result, i);
    list_first_delete(*result);
}


void print_path(map_t* map) {
    for (int i = 0; i < list_size(map->path); i++) {
        point_t point = {0,0};
        convert_to_point(&point, list_data(map->path, i));
        map->area_r[point.y][point.x] = 'p';
    }
}
void BFS_a(map_t* map) {
    map_relationship(map);
    int start = convert(&map->begin_p);
    int goal = convert(&map->goal_p);

    queue_t* queue = queue_init(RELATIONSIZE);
    queue_push(queue, start);
    list_t* visited = list_init(0);
    int predecessor[RELATIONSIZE];
    predecessor[start] = start;

    while (!queue_is_empty(queue)) {
        int c_node = queue_pop(queue);
        list_t *next_nodes = NULL;
        get_relationship_a(map, c_node, &next_nodes);
        for (int i = 0; i < list_size(next_nodes); i++) {
            int c_neighbor = list_data(next_nodes, i);
            if (!in_list(visited, c_neighbor) && !in_queue(queue, c_neighbor)) {
                queue_push(queue, c_neighbor);
                predecessor[c_neighbor] = c_node;
            }
        }
        list_add_to_end(visited, c_node);
    }
    int tmp = goal;
    while (tmp != start) {
        list_add_to_end(map->path, tmp);
        if (tmp > RELATIONSIZE) exit(-2);
        tmp = predecessor[tmp];
    }
    list_add_to_end(map->path, start);
    list_first_delete(map->path);

    list_reverse(&map->path);
    print_path(map);
    list_first_delete(visited);
    list_delete(visited);
}


void map_delete(map_t* map) {

}