//
//  main.c
//  Lab07_C
//
//  Created by John Hervey on 8/22/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct node node;
/* typedef struct tree tree; */

struct node {
    struct node *parent;
    struct node *child1;
    struct node *child2;
    int key;
    int *characterisitics;
    int index;
};

/* struct tree {
    node *root;
    int numberOfLeaves;
}; */

void *emalloc(size_t s)
{
    void *result = malloc(s);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failure.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/* tree treeInit(int size) {
    
}*/

int main(int argc, const char * argv[])
{
    node *tree;
    int i, leaves, nodes, rand, otus, htus;
    int *usedIndices;
    
    /* Get tree size */
    if (argc != 2) {
        fprintf(stderr, "Please specify n value.\nUsage: %s #[nvalue]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    leaves = atoi(argv[1]);
    nodes = leaves + leaves - 1;
    
    /* Allocate and initialise to zero */
    tree = emalloc(nodes * sizeof(tree[0]));
    usedIndices = emalloc(nodes * sizeof(usedIndices[0]));
    for (i = 0; i < nodes; i++) {
        tree[i].parent = NULL;
        tree[i].child1 = NULL;
        tree[i].child2 = NULL;
        usedIndices[i] = 0;
    }

    /* Initialise Root */
    usedIndices[0] = 1;
    htus = 1;
    otus = 0;
    
    /* Fill the tree */
    /* The array is set up with the root follwed by all htus first, then the leaves */
    for (i = leaves; i < nodes; i++) {
        /* Find a random real insertion point (an htu) */
        rand = arc4random_uniform(leaves);
        while (!usedIndices[rand]) {
            rand = arc4random_uniform(leaves);
            
        }
        fprintf(stderr, "%i\n", rand);
        /* Make this insertion point the parent of the new node */
        tree[i].parent = &tree[rand];
        /* Leaves do not have children */
        tree[i].child1 = NULL;
        tree[i].child2 = NULL;
        /* If the insertion point htu has empty chlidren, insert the new node there. Otherwise create a new htu and swap pointers */
        if (tree[rand].child1 == NULL) {
            tree[rand].child1 = &tree[i];
        } else if (tree[rand].child2 == NULL) {
            tree[rand].child2 = &tree[i];
        } else {
            tree[htus].child1 = tree[rand].child1;
            tree[rand].child1 = &tree[htus];
            tree[htus].child2 = &tree[i];
            tree[i].parent = &tree[htus];
            usedIndices[htus] = 1;
            htus++;
        }
        tree[i].index = i;
        tree[i].key = arc4random_uniform(leaves);
        otus++;
    }
    
    
    return EXIT_SUCCESS;
}

