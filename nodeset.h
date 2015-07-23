#pragma once

#include <stddef.h>

#include "node.h"

typedef struct nodeset {
  size_t size, capacity;
  nodep *nodes;
} nodeset;

void nodeset_init(nodeset *set, size_t capacity);
uint8_t nodeset_contains(nodeset *set, node *node);
void nodeset_add(nodeset *set, node *node);
void nodeset_remove(nodeset *set, node *node);
void nodeset_clear(nodeset *set);
void nodeset_free(nodeset *set);

