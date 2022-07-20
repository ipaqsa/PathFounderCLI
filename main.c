#include <stdio.h>
#include "src/Engine.h"


int main() {
    map_t* map = map_init();
    point_t goal = {12,13};
    set_goal(map, goal);
    BFS_a(map);

    map_show(map, 1);
    getchar();
    return 0;
}
