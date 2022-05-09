#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct tree {
    int value;
    struct tree* right;
    struct tree* left;
} tree;


void Build_tree(int temp, tree** derevo){
    if (*derevo == NULL) {
        tree* new_tree = (tree*) malloc(sizeof(tree));
        new_tree->left = NULL;
        new_tree->right = NULL;
        new_tree->value = temp;
        *derevo = new_tree;
    }
    else {
        if ((*derevo)->value <= temp){
            Build_tree(temp, &((*derevo)->right));
        }
        else{
            Build_tree(temp, &((*derevo)->left));
        }
    }
}
int find(int value, tree* derevo){
    int high = 0;
    while (derevo != NULL){
        high++;
        //printf("%d\n", high);
        if (value == derevo->value){
            return high;
        }
        else if (value < derevo->value){
            derevo = derevo->left;
        }
        else {
            derevo = derevo->right;
        }
    }
    return -1;
}
void deleter2(tree** derevo){
    if(*derevo != NULL){
    deleter2(&((*derevo)->right));
    deleter2(&((*derevo)->left));
    free(*derevo);
    *derevo = NULL;
    }
}
void print_tree(tree* derevo) {
    if (derevo!=NULL) { 
        print_tree(derevo->left); 
        printf("%d ", derevo->value); 
        print_tree(derevo->right); 
    }
}

void deleter(tree* derevo){
    if(derevo != NULL){
        deleter(derevo->left);
        deleter(derevo->right);
        free(derevo);
    }
}
int main() {
    printf("at the end  of the input please will enter any symbol\n");
    printf("Enter tree :");
    tree* drevo = NULL;
    int temp;
    while (scanf("%d", &temp)) {
        Build_tree(temp, &drevo);
    }
    printf("your tree :");
    print_tree(drevo);
    printf("\n");      
    char curr;
    scanf("%c", &curr);
    printf("what find :");
    scanf("%d", &temp);
    // printf("%d", temp);
    temp = find(temp, drevo);
    printf("high = %d\n", temp);
    //deleter2(&drevo);
    deleter(drevo);
}
