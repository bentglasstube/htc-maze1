#include "maze.h"

#include <stdlib.h>
#include <string.h>

#include "nodeset.h"

void maze_init(maze *maze) {
  maze->height = maze->width = maze->capacity = 0;
  maze->sx = maze->sy = 0;
  maze->gx = maze->gy = 0;

  maze->nodes = NULL;
}

inline node* maze_node_at(maze *maze, int32_t x, int32_t y) {
  if (x < 0 || x >= maze->width) return NULL;
  if (y < 0 || y >= maze->height) return NULL;
  return maze->nodes[y * maze->width + x];
}

void maze_read(maze *maze, FILE *stream) {
  for (;;) {
    char *line = NULL;
    size_t len = 0, i;
    ssize_t read = getline(&line, &len, stream);
    uint32_t y;

    if (read == -1) {
      free(line);
      break;
    }

    // truncate line endings
    while (line[read - 1] == '\n' || line[read - 1] == '\r') read--;

    // fprintf(stderr, "M %s\n", line);

    if (maze->height == 0) {
      // initially allocate maze width and height
      maze->width = read;
      maze->height = 1;

      maze->capacity = 4 * maze->width;
      maze->nodes = (node **)realloc(maze->nodes, sizeof(node *) * maze->capacity);
      memset(maze->nodes, 0, maze->capacity * sizeof(node *));
    } else {
      // reallocate more nodes if needed
      if (maze->capacity == maze->height * maze->width) {
        size_t new_cap = maze->capacity + 4 * maze->width;
        maze->nodes = (node **)realloc(maze->nodes, sizeof(node *) * new_cap);
        memset(maze->nodes + maze->capacity, 0, (new_cap - maze->capacity) * sizeof(node *));
        maze->capacity = new_cap;
      }

      maze->height++;
    }

    y = maze->height - 1;

    for (i = 0; i < read; ++i) {
      char c = line[i];

      if (c == 'S') {
        maze->sx = i;
        maze->sy = y;
      } else if (c == 'F') {
        maze->gx = i;
        maze->gy = y;
      }

      if (c != 'X') {
        size_t j = y * maze->width + i;

        maze->nodes[j] = (node *)malloc(sizeof(node));

        maze->nodes[j]->x = i;
        maze->nodes[j]->y = y;
        maze->nodes[j]->parent = NULL;
        maze->nodes[j]->gscore = 0;
        maze->nodes[j]->fscore = 0;
      }
    }

    free(line);
  }
}

inline node* maze_start(maze *maze) {
  return maze_node_at(maze, maze->sx, maze->sy);
}

inline node* maze_goal(maze *maze) {
  return maze_node_at(maze, maze->gx, maze->gy);
}

inline void _check_and_add_node(maze *maze, nodeset *set, int32_t x, int32_t y) {
  node *node = maze_node_at(maze, x, y);
  if (node != NULL) nodeset_add(set, node);
}

void maze_neighbors(maze *maze, int32_t x, int32_t y, nodeset* output) {
  nodeset_clear(output);

  _check_and_add_node(maze, output, x - 1, y);
  _check_and_add_node(maze, output, x + 1, y);
  _check_and_add_node(maze, output, x, y - 1);
  _check_and_add_node(maze, output, x, y + 1);
}

inline uint32_t maze_heuristic(maze *maze, node *node) {
  return abs(node->x - maze->sx) + abs(node->y - maze->sy);
}

void maze_astar(maze *maze) {
  nodeset closed, open, neighbors;
  node *start, *goal;

  nodeset_init(&closed, 16);
  nodeset_init(&open, 16);

  start = maze_start(maze);
  goal = maze_goal(maze);

  nodeset_add(&open, goal);

  goal->gscore = 0;
  goal->fscore = maze_heuristic(maze, goal);

  nodeset_init(&neighbors, 4);

  while (open.size > 0) {
    size_t i;
    node *current = open.nodes[0];
    uint32_t best = current->fscore;

    for (i = 1; i < open.size; ++i) {
      if (open.nodes[i]->fscore <= best) {
        current = open.nodes[i];
        best = current->fscore;
      }
    }

    // fprintf(stderr, "> %u,%u %u\n", current->x, current->y, current->fscore);

    if (nodes_equal(current, start)) {
      nodeset_clear(&open);
      break;
    }

    nodeset_remove(&open, current);
    nodeset_add(&closed, current);

    maze_neighbors(maze, current->x, current->y, &neighbors);

    for (i = 0; i < neighbors.size; ++i) {
      node *neighbor = neighbors.nodes[i];
      uint32_t gscore = current->gscore + 1;

      if (nodeset_contains(&closed, neighbor)) continue;

      if (!nodeset_contains(&open, neighbor) || gscore < neighbor->gscore) {
        neighbor->parent = current;
        neighbor->gscore = gscore;
        neighbor->fscore = gscore + maze_heuristic(maze, neighbor);
        nodeset_add(&open, neighbor);

        // fprintf(stderr, "+ %u,%u %u\n", neighbor->x, neighbor->y, neighbor->fscore);
      }
    }
  }

  nodeset_free(&open);
  nodeset_free(&closed);
  nodeset_free(&neighbors);
}

void maze_print_path(maze *maze) {
  node *current, *goal;

  current = maze_start(maze);
  goal = maze_goal(maze);

  while (!nodes_equal(current, goal)) {
    node *next= current->parent;
    char dir = ' ';

    if (current->x < next->x) dir = 'E';
    if (current->x > next->x) dir = 'W';
    if (current->y < next->y) dir = 'S';
    if (current->y > next->y) dir = 'N';

    printf("%c\n", dir);

    current = next;
  }
}

void maze_free(maze *maze) {
  size_t i;

  for (i = 0; i < maze->capacity; i++) free(maze->nodes[i]);
  maze->capacity = maze->height = maze->width = 0;
  free(maze->nodes);
}

