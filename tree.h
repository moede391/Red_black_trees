#ifndef TREE_H
#define TREE_H
#include <stdio.h>

typedef enum { RED, BLACK } rbt_colour;
typedef enum tree_e { BST, RBT } tree_t;
typedef struct bst_node *bst;
typedef struct tree *tree;
typedef struct rbt_node *rbt;

extern void change_tree_type(tree t, tree_t type);
extern void preorder(tree t, void f(char* string, int freq));
extern void inorder(tree t, void f(char* string, int freq));
extern struct rbt_node* rbt_insert_aux(rbt node, char* item);
extern struct rbt_node* rbt_insert(rbt node, char* item);
extern struct rbt_node* rbt_free(rbt node);
extern struct rbt_node* rbt_left_rotate(rbt node);
extern struct rbt_node* rbt_right_rotate(rbt node);
extern void tree_output_dot_aux(tree t, FILE *out);

extern struct tree* tree_new(tree_t type);
extern struct tree* insert(tree t, char* item);
extern int tree_depth(tree t);
extern int search(tree t, char* key);
extern void tree_free(tree t);

#endif
