#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#include "mylib.h"

#define IS_BLACK(x) ((NULL == (x)) || (BLACK == (x)->colour))
#define IS_RED(x) ((NULL != (x)) && (RED == (x)->colour))

struct tree {
    tree_t type;
	bst b;
	rbt r;
};

struct bst_node {
    char *key;
	int freq;
    bst left;
    bst right;
};

struct rbt_node {
    char *key;
	int freq;
    rbt_colour colour;
    rbt left;
    rbt right;
    rbt parent;
};

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int bst_depth(bst b)
{
    if (b == NULL) {
        return 0;
    }
    return 1 + max(bst_depth(b->left), bst_depth(b->right));
}

int rbt_depth(rbt r)
{
    if (r == NULL) {
        return 0;
    }
    return 1 + max(rbt_depth(r->left), rbt_depth(r->right));
}

/* Returns value of deepest node */
int tree_depth(tree t)
{
    if (t->type == BST)
    {
        return bst_depth(t->b);
    }
    else
        {
            return rbt_depth(t->r);
        }
}

static void tree_output_dot_aux_rbt(rbt t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->freq,
                (RED == t->colour) ? "red":"black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux_rbt(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux_rbt(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

static void tree_output_dot_aux_bst(bst t, FILE *out) {
    if(t->key != NULL) {
        fprintf(out, "\"%s\"[label=\"{<f0>%s:%d|{<f1>|<f2>}}\"color=%s];\n",
                t->key, t->key, t->freq, "black");
    }
    if(t->left != NULL) {
        tree_output_dot_aux_bst(t->left, out);
        fprintf(out, "\"%s\":f1 -> \"%s\":f0;\n", t->key, t->left->key);
    }
    if(t->right != NULL) {
        tree_output_dot_aux_bst(t->right, out);
        fprintf(out, "\"%s\":f2 -> \"%s\":f0;\n", t->key, t->right->key);
    }
}

void tree_output_dot_aux(tree t, FILE *out) {
    if (t->type == BST) {
        tree_output_dot_aux_bst(t->b, out);
    } else {
        tree_output_dot_aux_rbt(t->r, out);
    }
}

struct bst_node* bst_new(char* item) {
    bst result = emalloc(sizeof *result);
    result->key = emalloc(sizeof (char) * (strlen(item) + 1));
    strcpy(result->key, item);
    result->freq = 1;
    result->left = NULL;
    result->right = NULL;
    return result;
}

struct rbt_node* rbt_new(char* item) {
    rbt result = emalloc(sizeof *result);
    result->colour = RED;
    result->freq = 1;
    result->key = emalloc(sizeof (char) * (strlen(item) + 1));
    strcpy(result->key, item);
    result->left = result->right = NULL;
    return result;
}

struct tree* tree_new(tree_t type) {
    tree result = emalloc(sizeof *result);
    result->type = type;
    result->b = NULL;
    result->r = NULL;
    return result;
}

void change_tree_type(tree t, tree_t type)
{
	t->type = type;
}

void bst_inorder(bst b, void f(char* string, int freq))
{
	if (b == NULL)
       	{
		return;
	}
	else
	{
               	bst_inorder(b->left, f);
               	f(b->key, b->freq);
               	bst_inorder(b->right, f);
	}
}

static rbt rbt_fix(rbt r) {
    if(IS_RED(r->left)){
        if(IS_RED(r->left->left)){
            if(IS_RED(r->right)){
                r->colour = RED;
                r->left->colour = BLACK;
                r->right->colour = BLACK;
            }else if(IS_BLACK(r->right)){
                r=rbt_right_rotate(r);
                r->colour = BLACK;
                r->right->colour = RED;
            }
        } else if (IS_RED(r->left->right)) {
            if(IS_RED(r->right)){
                r->colour=RED;
                r->left->colour = BLACK;
                r->right->colour = BLACK;
            }else if(IS_BLACK(r->right)){
                r->left = rbt_left_rotate(r->left);
                r = rbt_right_rotate(r);
                r->colour = BLACK;
                r->right->colour = RED;
            }
        }
    } else if(IS_RED(r->right)) {
        if (IS_RED(r->right->left)) {
            if (IS_RED(r->left)) {
                r->colour = RED;
                r->left->colour = BLACK;
                r->right->colour = BLACK;
            } else if (IS_BLACK(r->left)) {
                r->right = rbt_right_rotate(r->right);
                r = rbt_left_rotate(r);
                r->colour = BLACK;
                r->right->colour = RED;
            }
        }else if (IS_RED(r->right->right)) {
            if (IS_RED(r->left)){
                r->colour = RED;
                r->left->colour= BLACK;
                r->right->colour = BLACK;
            }
            else if (IS_RED(r->right)) {
                r = rbt_left_rotate(r);
                r->colour = BLACK;
                r->left->colour = RED;
            }
        }
    }
    return r;
}

void rbt_inorder(rbt r, void f(char* string, int freq))
{
	if (r == NULL)
        {
		return;
	}
	else
	{
                rbt_inorder(r->left, f);
                f(r->key, r->freq);
                rbt_inorder(r->right, f);
        }
}

void inorder(tree t, void f(char* string, int freq))
{
	if (t->type == BST)
	{
		bst_inorder(t->b, f);
	}
	else
	{
		rbt_inorder(t->r, f);
	}
}

void bst_preorder(bst b, void f(char* string, int freq))
{
	if (b == NULL)
	{
		return;
	}
	else
	{
        	f(b->key, b->freq);
        	bst_preorder(b->left, f);
        	bst_preorder(b->right, f);
    	}
}

void rbt_preorder(rbt r, void f(char* string, int freq))
{
        if (r == NULL)
        {
		return;
	}
	else
	{
                f(r->key, r->freq);
                rbt_preorder(r->left, f);
                rbt_preorder(r->right, f);
        }
}

void preorder(tree t, void f(char* string, int freq))
{
        if (t->type == BST)
	{
		bst_preorder(t->b, f);
	}
	else
	{
		rbt_preorder(t->r, f);
	}
}

/* Left rotate.*/
struct rbt_node* rbt_left_rotate(rbt node)
{
        rbt tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        return tmp;
}

/* Right totate */
struct rbt_node* rbt_right_rotate(rbt node)
{
        rbt tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        return tmp;
}

struct rbt_node* rbt_insert_aux(rbt node, char* item)
{
    if (node == NULL)
        {
            node = emalloc(sizeof *node);
		node->freq = 1;
            node->left = NULL;
            node->right = NULL;
            node->parent = NULL;
            node->colour = RED;
            node->key = emalloc(sizeof(char) * (strlen(item) + 1));
		strcpy(node->key, item);
        }
	else if (strcmp(node->key, item) == 0)
	{
		node->freq += 1;
	}
    else if (strcmp(node->key, item) > 0) /* key is bigger, go right */
        {
            node->right = rbt_insert(node->right, item);
        }
    else if (strcmp(node->key, item) < 0) /* key is smaller, go left */
        {
            node->left = rbt_insert(node->left, item);
        }

    /* if BST MODE don't call fixup */
    return node;
}

struct rbt_node* rbt_insert(rbt node, char* item)
{
	rbt root;
    root = rbt_insert_aux(node, item);

    node = rbt_fix(root);
    return node;
}

struct bst_node* bst_insert(bst node, char* item) {
    int cmp;
    if (node == NULL) {
        return bst_new(item);
    }
    cmp = strcmp(item, node->key); /* cmp = item - node->key */
    if (cmp == 0) {
        node->freq += 1;
    }else if (cmp > 0) {
        node->right = bst_insert(node->right, item);
    } else {
        node->left = bst_insert(node->left, item);
    }
    return node;
}

struct tree* insert(tree t, char* item)
{
	/* is bst */
    if (t->type == BST)
	{
		t->b = bst_insert(t->b, item);
		return t;
	}
	/* is rbt */
	else
	{
		t->r = rbt_insert(t->r, item);
		return t;
	}
}

struct bst_node* bst_search(bst b, char* key)
{
	/*printf("BEST SEARCH\n");*/
	/*printf("b key: %s, key: %s\n", b->key, key);*/
	if (b == NULL || b->key == key)
	{
        	return b;
    	}
    	if (b->key < key)
	{
        	return bst_search(b->right, key);
    	}
    	else
	{
		return bst_search(b->left, key);
	}
}

struct rbt_node* rbt_search(rbt r, char* key)
{
	printf("RBT SEARCH\n");
	if (r == NULL || r->key == key)
	{
		return r;
	}
        if (r->key < key)
	{
        	return rbt_search(r->right, key);
        }
	else
	{
        	return rbt_search(r->left, key);
	}
}

int search(tree t, char* key)
{
	/*if ((t->type == BST && t->b == NULL) || (t->type == RBT && t->r == NULL))
	{
		return 0;
	}*/
	bst resultB = NULL;
	rbt resultR = NULL;
    	if (t->type == BST)
	{
		printf("BST\n");
		resultB = bst_search(t->b, key);
		if (strcmp(resultB->key, key) == 0)
		{
			printf("err2\n");
			return 1;
		}
		else if (strcmp(resultB->key, key) != 0)
		{
			printf("err2\n");
			return 0;
		}
		else
		{
			return 0;
		}
	}
	else if (t->type == RBT)
	{
		printf("RBT\n");
		resultR = rbt_search(t->r, key);
		if (strcmp(resultR->key, key) == 0)
		{
			printf("err3\n");
			return 1;
		}
		else if (strcmp(resultR->key, key) != 0)
		{
			printf("err4\n");
			return 0;
		}
		else
		{
			return 0;
		}
	}
	return 0;
}

rbt rbt_free(rbt b) {
    if (b == NULL)
        {
            return NULL;
        }
    b->left = rbt_free(b->left);
    b->right = rbt_free(b->right);
    free(b->key);
    free(b);
    return NULL;
}


bst bst_free(bst b) {
    if (b == NULL)
        {
            return NULL;
        }
    b->left = bst_free(b->left);
    b->right = bst_free(b->right);
    free(b->key);
    free(b);
    return NULL;
}

void tree_free(tree t) {
    if (t->type == BST) {
        t->b = bst_free(t->b);
    }
    else
        {
            t->r = rbt_free(t->r);
        }
    free(t);
}
