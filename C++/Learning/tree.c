#include <stdio.h>
#include <stdlib.h>

#define arrlen(arr) sizeof(arr) / sizeof(arr[0])
#define memresize(ptr,amnt) ptr = realloc(ptr, amnt)
#define memnew malloc
#define memdel free

#define V_PTR_VAL(ptr,key) (*(key*)ptr)

struct node {
   void* data;
   struct node** children;
   int total;
};

struct node* make_root(void* data) {
   struct node* n = malloc(sizeof(struct node));
   n->data = data;
   n->children = calloc(1, sizeof(struct node*));
   n->total = 0;
   return n;
}

struct node* new_child(struct node* root, void* data) {
   struct node* child = make_root(data);
   root->children[root->total] = child;
   root->total++;
   root->children = realloc(root->children, root->total + 1 * sizeof(struct node));
   return child;
}

void free_node(struct node* root) {
   for (int i = 0; i < root->total; i++) {
      free_node(root->children[i]);
   }
   free(root->data);
   free(root);
}

int main(void) {
   struct node* root = make_root(1);
   struct node* left = new_child(root, "hi");
   struct node* right = new_child(root, 'R');
   struct node* left_right = new_child(left, 'A');

   printf("%d\n", root->data);
   printf("%s", root->children[0]->data);


   free_node(root);
   return 0;
}
