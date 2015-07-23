#include "node.h"

bool nodes_equal (node *a, node *b) {
  return a->x == b->x && a->y == b->y;
}

