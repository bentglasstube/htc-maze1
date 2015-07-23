#pragma once

#include <stdint.h>
#include <stdio.h>

#include "node.h"

typedef struct maze {
  int height, width, capacity;
  int sx, sy, gx, gy;
  node **nodes;
} maze;


void maze_init(maze *maze);
void maze_read(maze *maze, FILE *stream);
void maze_astar(maze *maze);
void maze_print_path(maze *maze);
void maze_print_scores(maze *maze);
void maze_free(maze *maze);
