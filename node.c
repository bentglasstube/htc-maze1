#include "node.h"

uint8_t nodes_equal (node *a, node *b) {
  return a->x == b->x && a->y == b->y;
}

