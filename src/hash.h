typedef struct Entry{
  char *key;
  void *val;
  struct Entry *next;
} Entry;
typedef struct HashTable{
  Entry **buckets;
  int nBuckets;
} HashTable;

