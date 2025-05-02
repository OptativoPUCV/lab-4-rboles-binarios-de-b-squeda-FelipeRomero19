#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap *new = (TreeMap*)malloc(sizeof(TreeMap)) ;
    if (new == NULL)return NULL ;
    new->root = NULL ;
    new->current = NULL ;
    new->lower_than = lower_than;

    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL) return;

    TreeNode *nuevo = createTreeNode(key, value) ;
    if(nuevo == NULL)return;

    if(tree->root == NULL)
    {
        tree->root = nuevo ;
        tree->current = nuevo ;
        return;
    }

    TreeNode * actual = tree->root;
    TreeNode * padre = NULL;
    while(actual != NULL)
    {
        padre = actual ;
        if(is_equal(tree, actual->pair->key, key) == 1)
        {
            free(nuevo->pair) ;
            free(nuevo) ;
            tree->current = actual ;
            return;
        }
        if(tree->lower_than(key, actual->pair->key))
        {
            actual = actual->left;
        }
        else
        {
            actual = actual->right ;
        }
    }
    nuevo->parent = padre ;
    if(tree->lower_than(key, padre->pair->key))
    {
        padre->left = nuevo ;
    }
    else
    {
        padre->right = nuevo ;
    }
    tree->current = nuevo ;
}

TreeNode * minimum(TreeNode * x){
    if (x == NULL)return NULL ;
    TreeNode *actual = x ;
    while(actual->left != NULL)
    {
        actual = actual->left ;
    }
    return actual;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if(tree == NULL || node == NULL) return;

    //NODO SIN HIJOS
    if(node->left == NULL && node->right == NULL)
    {
        if(node == tree->root)
        {
            tree->root = NULL;
        }
        else
        {
            if(node->parent->left == node)
            {
                node->parent->left = NULL;
            }
            else
            {
                node->parent->right = NULL;
            }
        }
        free(node->pair);
        free(node);
        return;
    }
    //NODO CON UN SOLO HIJO
    if((node->left == NULL) != (node->right == NULL))
    {
        TreeNode *hijo = (node->left != NULL) ? node->left : node->right;
        if(node == tree->root)
        {
            tree->root = hijo;
            hijo->parent = NULL;
        }
        else
        {
            if(node->parent->left == node)
            {
                node->parent->left = hijo ; 
            }
            else
            {
                node->parent->right = hijo;
            }
            hijo->parent = node->parent;
        }
        free(node->pair);
        free(node);
        return;
    }
    //CASO CON DOS HIJOS
    if(node->left != NULL && node->right != NULL)
    {
        TreeNode *min_node = minimum(node->right);

        node->pair->key = min_node->pair->key;
        node->pair->value = min_node->pair->value;

        removeNode(tree, min_node);
        return;
    }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
    if(tree == NULL || tree->root == NULL)return NULL ;

    TreeNode *current = tree->root ;

    while(current != NULL)
    {
        if(is_equal(tree, current->pair->key, key) == 1)
        {
            tree->current = current ;
            return current->pair ;
        }
        if(tree->lower_than(key, current->pair->key))
        {
            current = current->left;
        }
        else
        {
            current = current->right ;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL)return NULL;

    TreeNode *first = minimum(tree->root);

    tree->current = first ;
    return first->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
