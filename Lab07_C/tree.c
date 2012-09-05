#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_RANGE 100 // Defines the maximum number of different options for a given characteristic

typedef struct treenode treenode;
typedef struct Tree Tree;

struct treenode {
    int key;
    int *chars;
    int chars_nb;
    treenode *parent;
    treenode *left;
    treenode *right;
};

struct Tree {
    treenode * root;
    int leaves;
};

/* Tree constructor.
 * Frees memory for the empty tree structure and returns a pointer to the root node
 */
Tree *
Tree_init(int l){
    int i;
    treenode * root;
    if(!(root=malloc((l+l-1)*sizeof(treenode)))){
        fprintf(stderr,"Insufficient memory to allocate the tree");
        exit(EXIT_FAILURE);
    }

    for (i=0;i<l;i++){
        (root+i)->left = NULL;
        (root+i)->right = NULL;
        (root+i)->parent = NULL;
    }

    Tree * t;
    if(!(t=malloc(sizeof(Tree)))){
        fprintf(stderr,"Insufficient memory to allocate the tree");
        exit(EXIT_FAILURE);
    }

    t->root = root;
    t->leaves = l;
    return t;
}

/* Generates a random tree topology by changing pointers in the nodes */
int
rnd_tree(Tree* t){
    int i;
    int tree [(t->leaves)*2 - 2];

    for (i = 0; i < (t->leaves)*2 - 2; i++)
        tree[i] = 0;

    int node = t->leaves + 1;
    int leaf = 2;
    
    /* Initialise the smallest possible tree */
    t->root[0].left = &t->root[1];
    t->root[0].right = &t->root[2];
    t->root[1].parent = &t->root[0];
    t->root[2].parent = &t->root[0];
    tree [0] = 1;
    tree [1] = 2;
    
    /*loop through the leaf array of size 10*/
    for(leaf = 3;leaf <= t->leaves ;leaf++,node++){
        int rnd_node = tree[rand()%((leaf-2)*2)];

        if(t->root[rnd_node].parent->right == &t->root[rnd_node]){
            t->root[rnd_node].parent->right = &t->root[node];
         
        }else{
            t->root[rnd_node].parent->left = &t->root[node];
        }
        
        t->root[node].parent = t->root[rnd_node].parent;
        t->root[rnd_node].parent = &t->root[node];
        t->root[node].left = &t->root[rnd_node];
        t->root[node].right = &t->root[leaf];
        t->root[leaf].parent = &t->root[node];
        
        tree[(leaf-2)*2] = leaf;
        tree[(leaf-2)*2+1] = node;
    }
    return 0;
}

/* Randomly names the leaves of a tree */
int
name_leaves(Tree* t){
    int i;

    int indexes[t->leaves];
    for (i=0;i<t->leaves;i++)
        indexes[i] = i + 1;

    /* Fisher–Yates shuffle */
    for (i=t->leaves-1; i>0;i--){
        int r = rand()%(i+1);
        int temp = indexes[r];
        indexes[r] = indexes[i];
        indexes[i] = temp;
    }

    for (i=0;i<t->leaves;i++)
        t->root[i+1].key = indexes[i];

    return 0;
}

/* Reorders the tree recursively so that the smaller values of key stay on the left */
int 
reorder_tree(treenode * node){
    if (node->left){
        int min_left_sbtree = reorder_tree(node->left);
        int min_right_sbtree = reorder_tree(node->right);
        if (min_left_sbtree > min_right_sbtree){
           treenode * dummy = node->left;
           node->left = node->right;
           node->right = dummy;
        }
        return  min_left_sbtree < min_right_sbtree ? min_left_sbtree : min_right_sbtree;
    }else{
        return (*node).key;
    }
}

///* Print the "key" value of the leaves */
//int print_leaves(treenode * t){
//    int i;
//    for (i=0;i<LEAVES;i++)
//        printf("(%d) ",t[i+1].key);
//    printf("\n");
//    return 0;
//}

/* Prints the tree in Newick format */
int
print_newick(treenode* node){
    if (node->left){
        printf("(");
        print_newick(node->left);
        printf(",");
        print_newick(node->right);
        printf(")");
    }else{
        printf("%d",(*node).key);
    }

    if (!(node->parent))
        printf("\n");

    return 0;
}

int
rnd_characteristics(Tree * t, int length, int max_range){
    int status = 0;
    int i,j;
    if(max_range < 2){
        fprintf(stderr,"Invalid range (%d) for random characteristics (min. 2), set to binary characteristics",max_range);
        max_range = 2;
        status = 1;
    }
    for (i=0;i<t->leaves;i++){
        t->root[i+1].chars_nb = length;
        if(!(t->root[i+1].chars = malloc(length*sizeof(int)))){
            fprintf(stderr,"Insufficient memory to allocate the tree");
            exit(EXIT_FAILURE);
        }
    }

    for (i=0;i<length;i++){
        int range = rand() % (max_range-1) + 2;
        //printf("RANGE FOR CHAR %d: %d\n ",i,range);
        for (j=0;j<t->leaves;j++){
           t->root[j+1].chars[i] = rand() % range;
            //printf("%d ",t->root[j+1].chars[i]);
        }
        //printf("\n");
    }
    return status;
}

/* Simply, print the characteristics of each node */
int
print_chars(Tree * t){
    int status = 0;
    int i,j;
    for (i=0;i<t->leaves;i++){
        printf("(%2d) ",t->root[i+1].key);
        for (j=0;j<t->root[i+1].chars_nb;j++){
            printf("%d ",t->root[i+1].chars[j]);
        }
        printf("\n");
    }
    return status;
}

/* Calculate Fitch cost of a whole Tree structure */
int
get_Fitch_cost(Tree * t, int *cost){
    int status = 0;
    int n,i,j;
    *cost = 0;
    for (n=0;n<t->root[1].chars_nb;n++){
        int range=0;
        int tmp[MAX_RANGE];
        for(i=0;i<t->leaves;i++){
        
            int found = 0;
            for (j=0;j<range;j++){
                if (t->root[i+1].chars[n] == tmp[j]){
                    found++;
                    break;
                }
            }
            if (!found){
                tmp[range] = t->root[i+1].chars[n];
                range++;
            }
        }
                
        int label[range];
        for (i=0;i<range;i++){
            label[i] = -1;
        }

        *cost += fitch(t->root,label,range);
    }

    return status;
}

/* Calculate Fitch cost of a subtree */
int
fitch(treenode * node, int *label, int range){
    int i;
    int cost = 0;

    if (node->left){
        int l_left[range];
        int l_right[range];
        for (i=0;i<range;i++){
            l_left[i] = -1;
            l_right[i] = -1;
        }
        int c_left = fitch(node->left,l_left,range);
        int c_right = fitch(node->right,l_right,range);

        if(!(get_intersection(l_left,l_right,range,label))){
            cost++;
        }

        cost += c_left + c_right;

    }else{
        label[0] = (*node).chars[0];
    }

    return cost;
}

int get_intersection(int * array1, int * array2, int length, int * output){
    int i,j,k=0;
    for (i=0;i<length;i++){
        for (j=0;j<length;j++){
            if (array1[i] < 0 || array2[j] < 0)
                continue;
                
            if (array1[i] == array2[j]){
                output[k] = array1[i];
                k++;
            }
        }
    }

    if (!k){
        for (i=0;i<length;i++){
            if (array1[i] >= 0){
                output[k] = array1[i];
                k++;
            }
            if (array2[i] >= 0){
                output[k] = array2[i];
                k++;
            }
        }
        return 0;
    }else{
        return 1;
    }
}

int
main (void){
    srand ( time(NULL) );
    int i;

    /* Tree constructor */
    Tree * t = Tree_init(9);

    /* Creates a random topology for the tree */ 
    rnd_tree(t);

    /* Randomly associates integer indexes to the leaves */
    name_leaves(t);

    /* Reorder the tree */
    reorder_tree(t->root);

    /* Print the tree in Newick format */
    print_newick(t->root);
    
    /* Create species with random characteristics */
    rnd_characteristics(t,3,5);
    printf("-----\n");

    /* Print the characteristics of every leaf */
    print_chars(t);
    printf("-----\n");

   /* Calculate Fitch cost */
   int cost;
   get_Fitch_cost(t,&cost);

   printf("COST: %d\n",cost);
    
   // for(i = 0; i<=LEAVES; i ++){
   //     t[i].key = &list[i];
   //     printf("%d \n",t->key[i]);
   // }
    
    
    return EXIT_SUCCESS;
}
