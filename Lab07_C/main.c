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

struct node {
    struct node *parent;
    struct node *child1;
    struct node *child2;
    int key;
    int *characterisitics;
    int index;
};

void *emalloc(size_t s)
{
    void *result = malloc(s);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failure.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/* Prints the tree in Newick format */
int print_newick(node start){
    if (start.child1){
        printf("(");
        print_newick(*start.child1);
        printf(",");
        print_newick(*start.child2);
        printf(")");
    }else{
        printf("%d", start.key);
    }
    
    if (!(start.parent))
        printf("\n");
    
    return 0;
}

int main(int argc, const char * argv[])
{
    node *tree;
    int i, swap, leaves, nodes, rand, otus, htus;
    int *usedIndices;
    int *randomKeys;
    
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
    
    /* Create a set of random keys */
    randomKeys = emalloc(leaves * sizeof randomKeys[0]);
    for (i = 0; i < leaves; i++) {
        randomKeys[i] = i;
    }
    /* Shuffle */
    for (i = leaves - 1; i > 0; i--) {
        rand = arc4random_uniform(i);
        swap = randomKeys[i];
        randomKeys[i] = randomKeys[rand];
        randomKeys[rand] = swap;
    }
    for (i = 0; i < leaves; i++) {
        fprintf(stderr, "%i, ", randomKeys[i]);
    }
    fprintf(stderr, "\n");

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
        tree[i].index = i;
        tree[i].key = randomKeys[i - leaves];
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
        
        otus++;
    }
    print_newick(tree[0]);
    fprintf(stderr, "OTU's : %i\n", otus);
    fprintf(stderr, "HTU's : %i\n", htus);
    
    /* Deallocacte */
    free(tree);
    free(usedIndices);
    free(randomKeys);
    
    return EXIT_SUCCESS;
}

