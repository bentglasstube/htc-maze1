#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct node {
  int x, y;
  uint16_t gscore, fscore;
  struct node *parent;
} node, *nodep;

bool nodes_equal (node *a, node *b);
