#pragma once

#include <stdint.h>

typedef struct node {
  uint32_t x, y;
  uint32_t gscore, fscore;
  struct node *parent;
} node, *nodep;

uint8_t nodes_equal (node *a, node *b) {
  if (a == NULL) return 0;
  if (b == NULL) return 0;
  return a->x == b->x && a->y == b->y;
}
