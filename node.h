#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct node {
  int32_t x, y;
  uint32_t gscore, fscore;
  struct node *parent;
} node, *nodep;

bool nodes_equal (node *a, node *b);
