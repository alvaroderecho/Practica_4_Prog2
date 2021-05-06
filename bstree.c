#include <stdlib.h>
#include <stdio.h>
#include "bstree.h"

/* START [_BSTNode] */
typedef struct _BSTNode
{
    void *info;
    struct _BSTNode *left;
    struct _BSTNode *right;
} BSTNode;
/* END [_BSTNode] */

/* START [_BSTree] */
struct _BSTree
{
    BSTNode *root;
    P_tree_ele_print print_ele;
    P_tree_ele_cmp cmp_ele;
};
/* END [_BSTree] */

/*** BSTNode TAD private functions ***/
BSTNode *_bst_node_new()
{
    BSTNode *pn = NULL;

    pn = malloc(sizeof(BSTNode));
    if (!pn)
    {
        return NULL;
    }

    pn->left = pn->right = NULL;
    pn->info = NULL;
    return pn;
}

void *node_find_min(BSTNode *node);
void *node_find_max(BSTNode *node);
Bool node_contains(BSTNode *node, const void *elem);
Status node_insert(BSTNode *node, const void *elem);

void _bst_node_free(BSTNode *pn)
{
    if (!pn)
        return;

    free(pn);
}

void _bst_node_free_rec(BSTNode *pn)
{
    if (!pn)
        return;

    _bst_node_free_rec(pn->left);
    _bst_node_free_rec(pn->right);
    _bst_node_free(pn);

    return;
}

int _bst_depth_rec(BSTNode *pn)
{
    int depthR, depthL;
    if (!pn)
        return 0;

    depthL = _bst_depth_rec(pn->left);
    depthR = _bst_depth_rec(pn->right);

    if (depthR > depthL)
    {
        return depthR + 1;
    }
    else
    {
        return depthL + 1;
    }
}

int _bst_size_rec(BSTNode *pn)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_size_rec(pn->left);
    count += _bst_size_rec(pn->right);

    return count + 1;
}

int _bst_preOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += print_ele(pf, pn->info);
    count += _bst_preOrder_rec(pn->left, pf, print_ele);
    count += _bst_preOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_inOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_inOrder_rec(pn->left, pf, print_ele);
    count += print_ele(pf, pn->info);
    count += _bst_inOrder_rec(pn->right, pf, print_ele);

    return count;
}

int _bst_postOrder_rec(BSTNode *pn, FILE *pf, P_tree_ele_print print_ele)
{
    int count = 0;
    if (!pn)
        return count;

    count += _bst_postOrder_rec(pn->left, pf, print_ele);
    count += _bst_postOrder_rec(pn->right, pf, print_ele);
    count += print_ele(pf, pn->info);

    return count;
}

/*** BSTree TAD functions ***/
BSTree *tree_init(P_tree_ele_print print_ele, P_tree_ele_cmp cmp_ele)
{
    if (!print_ele || !cmp_ele)
        return NULL;

    BSTree *tree = malloc(sizeof(BSTree));
    if (!tree)
    {
        return NULL;
    }

    tree->root = NULL;
    tree->print_ele = print_ele;
    tree->cmp_ele = cmp_ele;

    return tree;
}

void tree_destroy(BSTree *tree)
{
    if (!tree)
        return;

    _bst_node_free_rec(tree->root);

    free(tree);
    return;
}

Bool tree_isEmpty(const BSTree *tree)
{
    if (!tree || !tree->root)
        return TRUE;
    return FALSE;
}

int tree_depth(const BSTree *tree)
{
    if (!tree)
        return -1;

    return _bst_depth_rec(tree->root);
}

size_t tree_size(const BSTree *tree)
{
    if (!tree)
        return -1;

    return _bst_size_rec(tree->root);
}

int tree_preOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_preOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_inOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_inOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

int tree_postOrder(FILE *f, const BSTree *tree)
{
    if (!f || !tree)
        return -1;

    return _bst_postOrder_rec(tree->root, f, tree->print_ele) + fprintf(f, "\n");
}

/**** TODO: find_min, find_max, insert, contains, remove ****/

void *tree_find_min(BSTree *tree)
{

    if (!tree)
        return NULL;

    return node_find_min(tree->root);
}

void *node_find_min(BSTNode *node)
{ //privada

    if (!node)
        return node->info;

    node_find_min(node->left);
}

void *tree_find_max(BSTree *tree)
{

    if (!tree)
        return NULL;

    return node_find_max(tree->root);
}

void *node_find_max(BSTNode *node)
{ //privada

    if (!node)
        return node->info;

    node_find_max(node->right);
}

Bool tree_contains(BSTree *tree, const void *elem)
{

    if (!tree || !elem)
        return FALSE;

    if (node_contains(tree->root, elem) == TRUE)
        return TRUE;

    return FALSE;
}

Bool node_contains(BSTNode *node, const void *elem)
{
    if (*(int *)node->info == *(int *)elem)
        return TRUE;
    if (*(int *)elem < *(int *)node->info)
    {
        if (node->left == NULL)
            return FALSE;
        node_contains(node->left, elem);
    }
    else
    {
        if (node->right == NULL)
            return FALSE;
        node_contains(node->right, elem);
    }
    return FALSE;
}

Status tree_insert(BSTree *tree, const void *elem)
{
    if (!tree || !elem)
        return ERROR;

    if (tree_contains(tree, elem) == TRUE)
        return OK;

    if (node_insert(tree->root, elem) == TRUE)
        return OK;
    return ERROR;
}

Status node_insert(BSTNode *node, const void *elem)
{
    BSTNode *new_node = NULL;

    if (*(int *)elem < *(int *)node->info)
    {
        if (node->left == NULL)
        {
            new_node = _bst_node_new();
            if (new_node == NULL)
                return ERROR;
            new_node->info = elem;
            node->left = new_node;
            return OK;
        }
        else

            node_insert(node->left, elem);
    }
    else
    {

        if (node->right == NULL)
        {
            new_node = _bst_node_new();
            if (new_node == NULL)
                return ERROR;
            new_node->info = elem;
            node->right = new_node;
            return OK;
        }
        else
            node_insert(node->right, elem);
    }

    return ERROR;
}