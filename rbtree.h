#ifndef _RBTREE_H_
#define _RBTREE_H_
// A red black tree implementation
// 2013(C) Matt Luo <mante.luo@gmail.com>

#include <stdio.h>
#include <malloc.h>

typedef unsigned char uchar;

typedef enum rb_color_e rb_color_t;
enum rb_color_e{ RED=0, BLACK};

typedef struct rb_node_s rb_node_t;
struct rb_node_s
{
    rb_node_t *p;
    rb_node_t *left;
    rb_node_t *right;
    rb_color_t color;
    uchar data;
};

typedef struct rb_tree_s rb_tree_t;
struct rb_tree_s
{
    rb_node_t *root;
    rb_node_t *nil;
};

void rb_tree_init(rb_tree_t *tree);
void rb_tree_insert(rb_tree_t *tree, uchar d);
void rb_tree_insert_fixup(rb_node_t *node, rb_tree_t *tree);
void rb_tree_delete(rb_node_t *node, rb_tree_t *tree);
void rb_tree_delete_fixup(rb_node_t *node, rb_tree_t *tree);
void rb_tree_destroy(rb_tree_t *tree);
void rb_tree_print(rb_tree_t *tree);
void rb_tree_print_node(rb_node_t *node);
rb_node_t* rb_tree_successor(rb_node_t *node, rb_tree_t *tree);
rb_node_t* rb_tree_find(rb_tree_t *tree, uchar d);
static void rb_tree_print_rec(rb_node_t *node, rb_tree_t *tree);
static rb_node_t* rb_tree_buynode(uchar data, rb_node_t *l, rb_node_t *r, rb_node_t *p);
static void rb_tree_makeroot(rb_tree_t *tree);
static void rb_tree_right_rotate(rb_node_t *node, rb_tree_t *tree);
static void rb_tree_left_rotate(rb_node_t *node, rb_tree_t *tree);
static void rb_tree_transplant(rb_tree_t *tree, rb_node_t *u, rb_node_t *v);
#define rb_tree_child(n) n == n->p->left ? left : right
#define rb_tree_child_rev(n) n == n->p->left ? right : left

#endif /* _RBTREE_H_ */
