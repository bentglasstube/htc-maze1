#include "nodeset.h"

#include <malloc.h>

void nodeset_init(nodeset *set, size_t capacity) {
  set->size = 0;
  set->capacity = capacity;
  set->nodes = (nodep *)malloc(set->capacity * sizeof(nodep));
}

bool nodeset_contains(nodeset *set, node *node) {
  size_t i;
  for (i = 0; i < set->size; ++i) {
    if (nodes_equal(set->nodes[i], node)) return true;
  }
  return false;
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

void nodeset_clear(nodeset *set) {
  set->size = 0;
}

void nodeset_free(nodeset *set) {
  set->size = 0;
  set->capacity = 0;
  free(set->nodes);
}


