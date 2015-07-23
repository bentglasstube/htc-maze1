#pragma once

#include <assert.h>
#include <malloc.h>
#include <stdint.h>

#include "node.h"

typedef struct nodeset {
  size_t size, capacity;
  nodep *nodes;
} nodeset;

void nodeset_init(nodeset *set, size_t capacity) {
  set->size = 0;
  set->capacity = capacity;
  set->nodes = (nodep *)malloc(set->capacity * sizeof(nodep));
}

uint8_t nodeset_contains(nodeset *set, node *node) {
  size_t i;
  for (i = 0; i < set->size; ++i) {
    if (nodes_equal(set->nodes[i], node)) return 1;
  }
  return 0;
}

void nodeset_add(nodeset *set, node *node) {
  if (nodeset_contains(set, node)) return;

  if (set->capacity == set->size) {
    set->capacity *= 2;
    set->nodes = (nodep *)realloc(set->nodes, set->capacity * sizeof(nodep));
  }

  set->nodes[set->size] = node;
  set->size++;
}

void nodeset_remove(nodeset *set, node *node) {
  size_t i;
  for (i = 0; i < set->size; ++i) {
    if (nodes_equal(set->nodes[i], node)) {
      set->nodes[i] = set->nodes[set->size - 1];
      set->size--;

      return;
    }
  }
}

node* nodeset_at(nodeset *set, size_t i) {
  return set->nodes[i];
}

void nodeset_clear(nodeset *set) {
  set->size = 0;
}

void nodeset_free(nodeset *set) {
  set->size = 0;
  set->capacity = 0;
  free(set->nodes);
}

