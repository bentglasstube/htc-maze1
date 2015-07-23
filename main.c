#include <stdio.h>

#include "node.h"
#include "maze.h"

int main() {
  maze maze;

  maze_init(&maze);
  maze_read(&maze, stdin);
  maze_astar(&maze);
  maze_print_path(&maze);
  maze_free(&maze);

  return 0;
}
