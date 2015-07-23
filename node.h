#pragma once

#include <stdint.h>

typedef struct node {
  uint32_t x, y;
  uint32_t gscore, fscore;
  struct node *parent;
} node, *nodep;

uint8_t nodes_equal (node *a, node *b);
