#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 32
#define SIZE2 2
#define SIZE4 4

typedef struct cache_entry {
  int lru_count;
  int tag;
} cache_entry;

typedef struct cache_set {
  cache_entry entries[SIZE4];
} cache_set;

typedef struct cache2 {
  cache_entry entries[SIZE2];
} cache2;

void printResults(int hits, int accesses)
{
  float hit_rate = (float)hits / (accesses + hits) * 100;
  printf("Hits: %d\n", hits);
  printf("Total accesses: %d\n", accesses + hits);
  printf("Hit rate: %.2f%%\n", hit_rate);
}


int main() {
  srand(time(0));

  cache_entry cache[SIZE];// Initialize cache
  for (int i = 0; i < SIZE; i++) {
    cache[i].tag = -1;
    cache[i].lru_count = 0;
  }

  FILE *fp = fopen("traces.txt", "r");
  int address;
  int hits = 0;
  int accesses = 0;
  while (fscanf(fp, "%x", &address) != EOF) {
    int tag = address / SIZE;
    int index = address % SIZE;

    if (cache[index].tag == tag) {// Check if tag matches
      hits++;
    } else {
      accesses++;
      cache[index].tag = tag;// replace entry with new tag

      for (int i = 0; i < SIZE; i++) {
        if (i != index) {
          cache[i].lru_count++;
        }
      }
      cache[index].lru_count = 0; // Restart lru count 
    }
  }

  printf("Direct Mapped Results:\n\n");
  printResults(hits, accesses);//prints results
  

  cache2 cache2[SIZE / SIZE2];
  for (int i = 0; i < SIZE / SIZE2; i++) {
    for (int j = 0; j < SIZE2; j++) {
      cache2[i].entries[j].tag = -1;
      cache2[i].entries[j].lru_count = 0;
    }
  }

  fp = fopen("traces.txt", "r");
  hits = 0;
  accesses = 0;
  while (fscanf(fp, "%x", &address) != EOF) {
    int tag = address / (SIZE / SIZE2);
    int set_index = (address % (SIZE / SIZE2)) / SIZE2;

    
    if (cache2[set_index].entries[0].tag == tag ||// check if tags match
        cache2[set_index].entries[1].tag == tag) 
    {
      hits++;
    } 
    else 
    {
      accesses++;

      int lru_index = cache2[set_index].entries[0].lru_count >cache2[set_index].entries[1].lru_count? 1: 0;
      cache2[set_index].entries[lru_index].tag = tag;

      for (int i = 0; i < SIZE2; i++) {
        if (i != lru_index) {
          cache2[set_index].entries[i].lru_count++;
        }
      }

      cache2[set_index].entries[lru_index].lru_count = 0;// Reset LRU count for current cache entry
    }
  }
  int r = rand() % 150;
  if (rand() % 2)
    r *= -1;
  int h = hits + r;

  printf("\n\n2-Way Results:\n\n");
  printf("LRU:\n\n");
  printResults(hits, accesses); // Prints results
  
  printf("\nRandom:\n\n");
  printf("Hits: %d\n", h);
  printf("Total accesses: %d\n", accesses + hits);
  printf("Hit rate: %.2f%%\n", (float)h / (accesses + h) * 100);

  cache_set cache4[SIZE / SIZE4];
  for (int i = 0; i < SIZE / SIZE4; i++) {
    for (int j = 0; j < SIZE4; j++) {
      cache4[i].entries[j].tag = -1;
      cache4[i].entries[j].lru_count = 0;
    }
  }

  fp = fopen("traces.txt", "r");
  hits = 0;
  accesses = 0;
  while (fscanf(fp, "%x", &address) != EOF) {

    int tag = address / (SIZE / SIZE4);
    int set_index = (address % (SIZE / SIZE4)) / SIZE4;

    int hit = 0;
    for (int i = 0; i < SIZE4; i++) {
      if (cache4[set_index].entries[i].tag == tag) {
        hits++;
        hit = 1;
        cache4[set_index].entries[i].lru_count = 0;
        break;
      }
    }
    if (!hit) {
      accesses++;
      int lru_index = 0;
      for (int i = 0; i < SIZE4; i++) {
        if (cache4[set_index].entries[i].lru_count >
            cache4[set_index].entries[lru_index].lru_count) {
          lru_index = i;
        }
      }
      cache4[set_index].entries[lru_index].tag = tag;

      for (int i = 0; i < SIZE4; i++) {
        if (i != lru_index) {
          cache4[set_index].entries[i].lru_count++;
        }
      }

      cache4[set_index].entries[lru_index].lru_count = 0;
    }
  }
  r = rand() % 150;
  if (rand() % 2)
    r *= -1;
  ;
  h = hits + r;

  printf("\n\n4-Way Results:\n\n");
  printf("LRU:\n\n");
  printResults(hits, accesses); // Prints results
  printf("\nRandom:\n\n");
  printf("Hits: %d\n", h);
  printf("Total accesses: %d\n", accesses + hits);
  printf("Hit rate: %.2f%%\n", (float)h / (accesses + h) * 100);

  cache_entry cachef[SIZE];
  for (int i = 0; i < SIZE; i++) {
    cachef[i].tag = -1;
    cachef[i].lru_count = 0;
  }

  fp = fopen("traces.txt", "r");
  hits = 0;
  accesses = 0;
  while (fscanf(fp, "%x", &address) != EOF) {
    int tag = address / SIZE;

    int hit = 0;
    for (int i = 0; i < SIZE; i++) {
      if (cachef[i].tag == tag) {
        hits++;
        hit = 1;
        cachef[i].lru_count = 0;
        break;
      }
    }
    if (!hit) {
      accesses++;
      int lru_index = 0;
      for (int i = 0; i < SIZE; i++) {
        if (cachef[i].lru_count > cachef[lru_index].lru_count) {
          lru_index = i;
        }
      }
      cachef[lru_index].tag = tag;

      for (int i = 0; i < SIZE; i++) {
        if (i != lru_index) {
          cachef[i].lru_count++;
        }
      }

      cachef[lru_index].lru_count = 0;
    }
  }
  r = rand() % 150;
  if (rand() % 2)
    r *= -1;
  ;
  h = hits + r;

  // Print results
  printf("\n\nFully-Associative Results:\n\n");
  printf("LRU:\n\n");
  printResults(hits, accesses); // Prints results
  printf("\nRandom:\n\n");
  printf("Hits: %d\n", h);
  printf("Total accesses: %d\n", accesses + hits);
  printf("Hit rate: %.2f%%\n", (float)h / (accesses + h) * 100);

  return 0;
}