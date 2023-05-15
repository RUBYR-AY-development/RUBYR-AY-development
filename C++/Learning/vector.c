/*
* Simple recreation of the C++ std::vector.
* Can support any type, no type restrictions.
* It can only add elements and free itself right now (I might add more later).
*/

#include <stdio.h>
#include <stdlib.h>

// using this over make_vector is easier
#define create_vector(name) vector name = make_vector();

typedef struct _vector {
   void** items;
   int total;
} _vector;
typedef _vector* vector;


vector make_vector() {
   vector v = malloc(sizeof(struct _vector));
   v->items = calloc(1, sizeof(void*));
   v->total = 0;
   return v;
}

void vector_push_back(vector v, void* thing) {
   v->items[v->total] = thing;
   v->total++;
   v->items = realloc(v->items, (v->total + 1) * sizeof(void*));
}

void vector_free(vector v, int free_items) {
   if (free_items) {
      for (int i = 0; i < v->total; i++) {
         free(v->items[i]);
      }
   }
   free(v->items);
   free(v);
}
