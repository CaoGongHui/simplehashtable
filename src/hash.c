#include "hash.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

uint32_t hash(char *s) {
  uint32_t h = 5381;
  char c;
  while ((c = *s++)) {
    h = 33 * h + c;
  }
  return h;
}
// return the index of the buckets of keyc
uint32_t get_bucket(HashTable *h, char *key) { return hash(key) % h->nBuckets; }
HashTable *HashTable_new() {
  int nBuckets = 4;
  HashTable *h = malloc(sizeof(HashTable));
  h->nBuckets = nBuckets;
  h->buckets = calloc(nBuckets, sizeof(Entry *));
  return h;
}
void HashTable_set(HashTable *h, char *key, void *val) {
  uint32_t bucket = get_bucket(h, key);
  Entry *v = h->buckets[bucket];
  while (v != NULL) {
    if (strcmp(v->key, key) == 0) {
      v->val = val;
      return;
    }
    v = v->next;
  }
  //没有找到现有的Entry则创建一个新的
  Entry *newVal = malloc(sizeof(Entry));
  newVal->key = strdup(key);
  newVal->val = val;
  newVal->next = h->buckets[bucket];
  h->buckets[bucket] = newVal;
}
void HashTable_free(HashTable *h) {
  for (int b = 0;b < h->nBuckets ; b++) {
    Entry *v = h->buckets[b];
    while (v != NULL) {
      Entry *next = v->next;
      free(v->key);
      free(v);
      v = next;
    }
  }
  free(h->buckets);
  free(h);
}
int main() {
  HashTable *h = HashTable_new();
  int a = 5;
  HashTable_set(h, "item_a", &a);
}
