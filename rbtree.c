#include "rbtree.h"

void rb_tree_init(rb_tree_t *tree)
{
    tree->nil = rb_tree_buynode(0, NULL, NULL, NULL);
    tree->nil->color = BLACK;

    tree->root = tree->nil;
    tree->root->p = tree->nil;
}
void rb_tree_insert(rb_tree_t *tree, uchar d)
{
    rb_node_t *node = tree->root, *temp;
    
    if(tree->nil == node)
    {
	tree->root = rb_tree_buynode(d, tree->nil,tree->nil, tree->nil);
	tree->root->color = BLACK;
	return;
    }
    
    while(tree->nil != node)
    {
	if(d > node->data)
	{
	    temp = node;
	    node = node->right;
	}
	else if(d < node->data)
	{
	    temp = node;
	    node = node->left;
	}
    }
    node = rb_tree_buynode(d, tree->nil, tree->nil, temp);
    if(d > temp->data)
	temp->right = node;
    else
	temp->left = node;

    // Fix up
    rb_tree_insert_fixup(node, tree);    
}
void rb_tree_insert_fixup(rb_node_t *node, rb_tree_t *tree)
{
    while(node->p->color == RED)
    {
	rb_node_t *pa = node->p, *gpa = node->p->p;
	rb_node_t *uncle = NULL;
	if(pa == gpa->left)
	{
	    uncle = gpa->right;

	    if(uncle->color == RED)
	    {
		pa->color = BLACK;
		uncle->color = BLACK;

		/* If gpa is root, we keep it BLACK */
		if(gpa->p != tree->nil)
		    gpa->color = RED;

		/* In order to loop, move to gpa */
		node = node->p->p;
	    }
	    else // uncle is BLACK
	    {
		if(node != pa->left)
		    rb_tree_left_rotate(node, tree);
		else
		    node = node->p;

		rb_tree_right_rotate(node, tree);
		node->color = BLACK;
		node->right->color = RED;

	    }
	}
	else /* right */
	{
	    uncle = gpa->left;

	    if(uncle->color == RED)
	    {
		pa->color = BLACK;
		uncle->color = BLACK;

		/* If gpa is root, we keep it BLACK */
		if(gpa->p != tree->nil)
		    gpa->color = RED;
		/* In order to loop, move to gpa */
		node = node->p->p;
	    }
	    else // uncle is black
	    {
		if(node != pa->right)
		    rb_tree_right_rotate(node, tree);
		else
		    node = node->p;

		rb_tree_left_rotate(node, tree);
		node->color = BLACK;
		node->left->color = RED;

	    }	
	}
    }
}
void rb_tree_delete(rb_node_t *node, rb_tree_t *tree)
{
    if(NULL == tree || tree->nil == node)
	return;
    
    rb_node_t * pa = node->p;
    rb_node_t *replace, *temp;
    rb_color_t original_color = node->color;
    /* Have one or no child */
    if(node->left == tree->nil || node->right == tree->nil)
    {
	if(node->left != tree->nil)
	{
	    rb_tree_transplant(tree, node, node->left);
	    replace = node->left;
	}
	else if(node->right != tree->nil)
	{
	    rb_tree_transplant(tree, node, node->right);
	    replace = node->right;
	}
	else /* No child */
	{
	    rb_tree_transplant(tree, node, node->left);
	    replace = node->left;
	}
    }
    else /* Have both children */
    {
	replace = rb_tree_successor(node, tree);
	original_color = replace->color;
	temp = replace->right;
	rb_node_t *t_pa = replace->p;
	rb_tree_transplant(tree, node, replace);

	replace->left = node->left;
	node->left->p = replace;
	
	if(replace != node->right)
	{
	    replace->right = node->right;
	    node->right->p = replace;

	    temp->p = t_pa;
	    t_pa->left = temp;
	}
	else
	    replace->right->p = replace;

	replace->color = node->color;
	replace = temp;
    }

    if(original_color == BLACK)
	rb_tree_delete_fixup(replace, tree);
    
    free(node);
}
void rb_tree_delete_fixup(rb_node_t *node, rb_tree_t *tree)
{
    rb_node_t *sibling;

    while(node->color == BLACK && node != tree->root)
    {
	if(node == node->p->left)
	    sibling = node->p->right;
	else
	    sibling = node->p->left;

	if(node == tree->nil && sibling == tree->nil)
	{
	    node = node->p;
	    continue;
	}
	if(node == node->p->left)
	{
	    if(sibling->color == RED)
	    {
		rb_tree_left_rotate(sibling, tree);
		node->p->color = RED;
		sibling->color = BLACK;
	    }
	    else if(sibling->color == BLACK)
	    {
		if(sibling->left->color == BLACK && sibling->right->color == BLACK)
		{
		    sibling->color = RED;
		    node = node->p;
		}
		else if(sibling->left->color == RED && sibling->right->color ==BLACK)
		{
		    sibling->left->color = BLACK;
		    sibling->color = RED;
		    rb_tree_right_rotate(sibling->left, tree);
		}
		else if(sibling->right->color == RED)
		{
		    node->p->color = BLACK;
		    sibling->color = RED;
		    sibling->right->color = BLACK;
		    rb_tree_left_rotate(sibling, tree);

		    node = tree->root;
		}
	    }
	}
	else
	{
	    if(sibling->color == RED)
	    {
		rb_tree_right_rotate(sibling, tree);
		node->p->color = RED;
		sibling->color = BLACK;
	    }
	    else if(sibling->color == BLACK)
	    {
		if(sibling->left->color == BLACK && sibling->right->color == BLACK)
		{
		    sibling->color = RED;
		    node = node->p;
		}
		else if(sibling->right->color == RED && sibling->left->color ==BLACK)
		{
		    sibling->right->color = BLACK;
		    sibling->color = RED;
		    rb_tree_right_rotate(sibling->right, tree);
		}
		else if(sibling->left->color == RED)
		{
		    node->p->color = BLACK;
		    sibling->color = RED;
		    sibling->left->color = BLACK;
		    rb_tree_right_rotate(sibling, tree);
		    node = tree->root;
		}
	    }
	}
    }
    /* Include the case that node->color == RED */
    node->color = BLACK;
}
void rb_tree_destroy(rb_tree_t *tree)
{
    
}
void rb_tree_print(rb_tree_t *tree)
{
    if(tree->root == tree->nil)
    {
	printf("\n");
	return;
    }
    rb_tree_print_rec(tree->root, tree);
    printf("\n");
}
void rb_tree_print_node(rb_node_t *node)
{
    printf("%d:0x%x\n", node->data, (unsigned int)node);
}
void rb_tree_print_rec(rb_node_t *node, rb_tree_t *tree)
{
    if(NULL == tree)
	return;
    if(node->left != tree->nil)
	rb_tree_print_rec(node->left, tree);
    printf("%d ", node->data);
    if(node->right != tree->nil)
	rb_tree_print_rec(node->right, tree);
}
rb_node_t* rb_tree_successor(rb_node_t *node, rb_tree_t *tree)
{

     rb_node_t *pa = node->p;
    if(node->right != tree->nil)
    {
	node = node->right;
	while(node->left != tree->nil)
	    node = node->left;
	return node;
    }
    /* If right child is nil */
    while(pa != tree->nil && node == pa->right)
    {
	node = pa;
	pa = pa->p;
    }
    return pa;
	
}
rb_node_t* rb_tree_find(rb_tree_t *tree, uchar d)
{
    if(NULL == tree)
	return tree->nil;

    rb_node_t *node = tree->root;
    while(d != node->data && node != tree->nil)
    {
	if(d > node->data)
	    node = node->right;
	else
	    node = node->left;	
    }
    if(d == node->data)
	return node;
    if(node == tree->nil)
	return tree->nil;    
}
rb_node_t* rb_tree_buynode(uchar data, rb_node_t *l, rb_node_t *r, rb_node_t *p)
{
    rb_node_t *t = (rb_node_t*) malloc(sizeof(rb_node_t));
    
    t->data = data;
    t->left = l;
    t->right = r;
    t->p = p;
    t->color = RED;
    return t;
}
void rb_tree_makeroot(rb_tree_t *tree)
{
    
}
void rb_tree_right_rotate(rb_node_t *node, rb_tree_t *tree)
{
    rb_node_t *pa = node->p;
    rb_node_t *gpa = node->p->p;

    node->p = gpa;
    // If gpa is root
    if(tree->nil == gpa)
	tree->root = node;
    else
    {
	if(pa == gpa->left)
	    gpa->left = node;
	else
	    gpa->right = node;
    }
    pa->left = node->right;
    if(node->right != tree->nil) /****/
	node->right->p = pa;

    node->right = pa;
    pa->p = node;
}
void rb_tree_left_rotate(rb_node_t *node, rb_tree_t *tree)
{
    rb_node_t *pa = node->p;
    rb_node_t *gpa = node->p->p;

    node->p = gpa;
    // If gpa is root
    if(tree->nil == gpa)
	tree->root = node;
    else
    {
	if(pa == gpa->left)
	    gpa->left = node;
	else
	    gpa->right = node;
    }
    
    pa->right = node->left;
    if(node->left != tree->nil) /****/
	node->left->p = pa;
    
    node->left = pa;
    pa->p = node;

}
/* Replace u with v , forget about u's children */
void rb_tree_transplant(rb_tree_t *tree, rb_node_t *u, rb_node_t *v)
{
    /* u is root */
    if(u->p == tree->nil)
    {
	tree->root = v;
	v->p = tree->nil;
	v->color = BLACK;
	return;
    }

    rb_node_t *pa = u->p;
    if(u == pa->left)
	pa->left = v;
    else
	pa->right = v;
    v->p = pa;


}

