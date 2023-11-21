#include "hash_table.h"
#include <math.h>
#include <signal.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <threads.h>
const int HT_PRIME_1 = 151;
const int HT_PRIME_2 = 163;
static ht_item HT_DELETED_ITEM = {NULL, NULL};

ht_hash_table *ht_new() {
  ht_hash_table *ht = malloc(sizeof(ht_hash_table));
  ht->size = 53;
  ht->count = 0;
  ht->items = calloc((size_t)ht->size, sizeof(ht_item *));
  return ht;
}

static ht_item *ht_new_item(const char *key, const char *value) {
  ht_item *i = malloc(sizeof(ht_item));
  i->key = strdup(key);
  i->value = strdup(value);
  return i;
}

static void ht_del_item(ht_item *i) {
  free(i->key);
  free(i->value);
  free(i);
}

static void ht_del_hash_table(ht_hash_table *ht) {
  for (int i = 0; i < ht->size; i++) {
    ht_item *item = ht->items[i];
    if (item != NULL) {
      ht_del_item(item);
    }
  }
  free(ht->items);
  free(ht);
}
static int ht_hash(const char *s, const int a, const int m) {
  long hash = 0;
  const int lens = strlen(s);
  for (int i = 0; i < lens; i++) {
    hash += (long)pow(a, lens - (i + 1)) * s[i];
    hash = hash % m;
  }
  return (int)hash;
}
static int ht_get_hash(const char *s, const int num_buckets,
                       const int attempt) {
  const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
  const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
  return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}
void ht_insert(ht_hash_table *ht, const char *key, const char *value) {
  ht_item *item = ht_new_item(key, value);
  int index = ht_get_hash(item->key, ht->size, 0);
  ht_item *cur_item = ht->items[index];
  int i = 1;
  while (cur_item != NULL) {
    if (cur_item != &HT_DELETED_ITEM) {
      if (strcmp(key, cur_item->key) == 0) {
        ht_del_item(cur_item);
        ht->items[index] = item;
        return;
      }
    }
    index = ht_get_hash(item->key, ht->size, i);
    cur_item = ht->items[index];
    i++;
  }
  ht->items[index] = item;
  ht->count++;
}

char *ht_search(ht_hash_table *ht, const char *key) {
  int index = ht_get_hash(key, ht->size, 0);
  ht_item *item = ht->items[index];
  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(key, item->key) == 0) {
        return item->value;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    item = ht->items[index];
    i++;
  }
  return NULL;
}

void ht_delete(ht_hash_table *ht, const char *key) {
  int index = ht_get_hash(key, ht->size, 0);
  ht_item *item = ht->items[index];
  int i = 1;
  while (item != NULL) {
    if (item != &HT_DELETED_ITEM) {
      if (strcmp(key, item->key) == 0) {
        ht_del_item(item);
        ht->items[index] = &HT_DELETED_ITEM;
      }
    }
    index = ht_get_hash(key, ht->size, i);
    i++;
  }
  ht->count--;
}

int main() {
  ht_hash_table *ht = ht_new();
  ht_del_hash_table(ht);
}
