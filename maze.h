#pragma once

#include <stdint.h>
#include <stdio.h>

#include "node.h"

typedef struct maze {
  uint32_t height, width, capacity;
  int32_t sx, sy, gx, gy;
  node **nodes;
} maze;


void maze_init(maze *maze);
void maze_read(maze *maze, FILE *stream);
void maze_astar(maze *maze);
void maze_print_path(maze *maze);
void maze_free(maze *maze);
