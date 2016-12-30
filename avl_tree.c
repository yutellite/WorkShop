#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifndef bool
typedef int bool;
#define false 0
#define true 1
#endif

#ifdef DEBUG
    #define ASSERT(p) assert(p)
#else
    #define ASSERT(p) (void*)(p)
#endif

typedef struct TreeNode
{
    int key;
    int data;
    int height;
    struct TreeNode* left;
    struct TreeNode* right;
}TreeNode;

int max(int a, int b)
{
    return (a>b)?a:b;
}

void avl_destroy(TreeNode**tree)
{
    if(*tree==NULL)
        return;
    avl_destroy(&(*tree)->left);
    avl_destroy(&(*tree)->right);
    printf("%d ",(*tree)->key);
    free(*tree);
    *tree=NULL;
}
TreeNode* avl_new_node(int key)
{
    TreeNode* node=(TreeNode*)malloc(sizeof(TreeNode));
    node->key=key;
    node->data=key;
    node->left=NULL;
    node->right=NULL;
    node->height=1;
    return(node);
}

void avl_insert(TreeNode** tree, int val)
{
    TreeNode* temp=NULL;
    if(*tree==NULL)
    {
        temp=(TreeNode*)malloc(sizeof(TreeNode));
        temp->left=temp->right=NULL;
        temp->key=temp->data=val;
        *tree=temp;
        return;
    }
    
    if(val<(*tree)->key)
        avl_insert(&(*tree)->left,val);
    else if(val>(*tree)->key)
        avl_insert(&(*tree)->right,val);
    else
        printf("Err. Cannot avl_insert a duplicate key %d.\n",val);
}

int avl_height(TreeNode* N)
{
    if(N==NULL)
        return 0;
    return N->height;
}

int avl_get_balance(TreeNode* t)
{
    if(t==NULL)
        return 0;
    return avl_height(t->left)-avl_height(t->right);
}

/*
  z                                y
 /  \                            /   \ 
T1   y     Left Rotate(z)       z      x
    /  \   - - - - - - - ->    / \    / \
   T2   x                     T1  T2 T3orT4
       / \
     T3orT4
*/
TreeNode* avl_left_rotate(TreeNode* z)
{
    ASSERT(z);
    TreeNode* y=z->right;
    TreeNode* t2=y->left;
    
    //perform rotation
    y->left=z;
    z->right=t2;
    
    z->height=max(avl_height(z->left),avl_height(z->right))+1;
    y->height=max(avl_height(y->left),avl_height(y->right))+1;
    return y;
}
/*
         z                                      y 
        / \                                   /   \
       y   T4      Right Rotate (z)          x     z
      / \          - - - - - - - - ->      /  \   /  \ 
     x   T3                               T1 orT2 T3  T4
    / \
  T1 orT2  
*/

TreeNode* avl_right_rotate(TreeNode* z)
{
    ASSERT(z);
    TreeNode* y=z->left;
    TreeNode* t3=y->right;
    
    //perform rotation
    y->right=z;
    z->left=t3;
    
    z->height=max(avl_height(z->left),avl_height(z->right))+1;    
    y->height=max(avl_height(y->left),avl_height(y->right))+1;
    return y;
}

/*
T1, T2 and T3 are subtrees of the tree rooted with y (on left side)
or x (on right side)
                y                               x
               / \     Right Rotation          /  \
              x   T3   – – – – – – – >        T1   y
             / \       < - - - - - - -            / \
            T1  T2     Left Rotation            T2  T3
Keys in both of the above trees follow the following order
      keys(T1) < key(x) < keys(T2) < key(y) < keys(T3)
So BST property is not violated anywhere.

a) Left Left Case

T1, T2, T3 and T4 are subtrees.
         z                                      y 
        / \                                   /   \
       y   T4      Right Rotate (z)          x      z
      / \          - - - - - - - - ->      /       /  \ 
     x   T3                               T1      T3  T4
    / \
  T1 orT2  
b) Left Right Case

     z                               z                           x
    / \                            /   \                        /  \ 
   y   T4  Left Rotate (y)        x    T4  Right Rotate(z)    y      z
  / \      - - - - - - - - ->    /  \      - - - - - - - ->  / \    / \
T1   x                          y    T3                    T1  T2 T3  T4
    / \                        / \
  T2 orT3                    T1   T2
c) Right Right Case

  z                                y
 /  \                            /   \ 
T1   y     Left Rotate(z)       z      x
    /  \   - - - - - - - ->    / \    / \
   T2   x                     T1  T2 T3  T4
       / \
     T3orT4
d) Right Left Case

   z                            z                            x
  / \                          / \                          /  \ 
T1   y   Right Rotate (y)    T1   x      Left Rotate(z)   z      y
    / \  - - - - - - - - ->     /  \   - - - - - - - ->  / \    / \
   x   T4                      T2   y                  T1  T2  T3  T4
  / \                              /  \
T2 orT3                           T3   T4
*/
TreeNode* avl_insert_node(TreeNode* node, int key)
{
    //1.perform the normal BST avl_insertion
    if(node==NULL)
        return avl_new_node(key);
    if(key<node->key)
        node->left=avl_insert_node(node->left,key);
    else if(key>node->key)
        node->right=avl_insert_node(node->right,key);
    else
    {
        printf("Err. Cannot avl_insert a duplicate key %d.\n",key);
        return node;
    }
    //2.update height of this ancestor node
    node->height=1+max(avl_height(node->left),avl_height(node->right));
    
    //3.get the balace factor of this ancestor node to check whether this node became unbalanced.
    int balance=avl_get_balance(node);
    
    //4.if this node becomes unbalanced, then there are 4 cases
    
    //left left case
    if(balance>1 && key<node->left->key)
        return avl_right_rotate(node);
    //left right case
    if(balance>1 && key>node->left->key)
    {
        node->left=avl_left_rotate(node->left);
        return avl_right_rotate(node);
        
    }    
    //right right case
    if(balance<-1 && key>node->right->key)
        return avl_left_rotate(node);
    //right left case
    if(balance<-1 && key<node->right->key)
    {
        node->right=avl_right_rotate(node->right);
        return avl_left_rotate(node);
    }

    return node;
}

TreeNode* avl_min_value_node(TreeNode* node)
{
    TreeNode* cur_node=node;
    while(cur_node->left!=NULL)
        cur_node=cur_node->left;
    
    return cur_node;
}

TreeNode* avl_delete_node(TreeNode* node, int key)
{
    TreeNode* temp=NULL;
    if(node==NULL)
        return node;
    if(key<node->key)
        node->left=avl_delete_node(node->left, key);
    else if(key>node->key)
        node->right=avl_delete_node(node->right, key);
    else
    {
        if(node->left==NULL || node->right==NULL)
        {
            temp=node->left?node->left:node->right;
            if(temp==NULL)
            {
                temp=node;
                node=NULL;
            }
            else
                
                *node=*temp;
            free(temp);
        }
        else
        {
            //node with two children: get the inorder
            //successor: smallest in the right subtree
            temp=avl_min_value_node(node->right);
            //memcpy(node,temp,sizeof(TreeNode));-->this is wrong, it will rewrite the left and right point and lost the subtree
            //so we only cope the key and data, remain the subtree pointers
            node->key=temp->key;
            node->data=temp->data;
            node->right=avl_delete_node(node->right,temp->key);
        }
    }
    //the tree only had one node then return
    if(node==NULL)
        return node;
    
    //update height of current node
    node->height=1+max(avl_height(node->left), avl_height(node->right));
    
    //check if balance
    int balance=avl_get_balance(node);
    
    //left left case
    if(balance>1 && key<node->left->key)
        return avl_right_rotate(node);
    //left right case
    if(balance>1 && key>node->left->key)
    {
        node->left=avl_left_rotate(node->left);
        return avl_right_rotate(node);
        
    }    
    //right right case
    if(balance<-1 && key>node->right->key)
        return avl_left_rotate(node);
    //right left case
    if(balance<-1 && key<node->right->key)
    {
        node->right=avl_right_rotate(node->right);
        return avl_left_rotate(node);
    }

    return node;
}

void avl_print_given_level(TreeNode* T, int level)
{
    if(T==NULL)
    {
        printf("null ");
        return;
    }
    if(level==1)
        printf("%d ",T->key);
    else
    {
        avl_print_given_level(T->left,level-1);
        avl_print_given_level(T->right,level-1);
    }
}
void avl_pre_order(TreeNode* tree)
{
    if(tree==NULL)
        return;
    printf("%d ",tree->key);
    if(tree->left!=NULL)
        avl_pre_order(tree->left);
    if(tree->right!=NULL)
        avl_pre_order(tree->right);
}
void avl_mid_order(TreeNode* tree)
{
    if(tree==NULL)
        return;
    if(tree->left!=NULL)
        avl_mid_order(tree->left);    
    printf("%d ",tree->key);
    if(tree->right!=NULL)
        avl_mid_order(tree->right);
}
void avl_post_order(TreeNode* tree)
{
    if(tree==NULL)
        return;
    if(tree->left!=NULL)
        avl_post_order(tree->left);
    if(tree->right!=NULL)
        avl_post_order(tree->right);        
    printf("%d ",tree->key);
}

void avl_level_order(TreeNode* T)
{
    if(T==NULL)
        return;
    int h=avl_height(T);
    int i=0;
    for(i=1;i<=h;i++)
    {
        avl_print_given_level(T,i);
        printf("\n");
    }
}
/* get the height by traversing the tree nodes
 *
*/
int avl_max_depth(TreeNode *root)
{
    int ldepth = 0;
    int rdepth = 0;
    if(root == NULL)
        return 0;
    ldepth = avl_max_depth(root->left);
    rdepth = avl_max_depth(root->right);
    return (ldepth > rdepth ? ldepth : rdepth) + 1;   
}
/*
 *you can get an inverse tree if you travese the tree inorder and the tree is a balaceed tree.
*/
void avl_mirror(TreeNode*root)
{
    TreeNode *temp=NULL;
    if(root==NULL)
        return;
    avl_mirror(root->left);
    avl_mirror(root->right);
    temp=root->left;
    root->left=root->right;
    root->right=temp;
}

int avl_count_node(TreeNode* root)
{
    int count=0;
    if(root==NULL)
        return 0;
    count+=avl_count_node(root->left);
    count+=avl_count_node(root->right);    
    count+=1;
    return count;
}

bool avl_is_balanced(TreeNode* root)
{
    int ldepth=0;
    int rdepth=0;
    if(root==NULL)
        return true;
    ldepth=avl_max_depth(root->left);
    rdepth=avl_max_depth(root->right);
    if(abs(ldepth-rdepth)<=1&&true==avl_is_balanced(root->left)&&true==avl_is_balanced(root->right))
        return true;
    else 
        return false;
}
int main()
{   
    int depth=0;
    TreeNode* root=NULL; 
    TreeNode* rootbalance=NULL;    
    avl_insert(&root,0);
    avl_insert(&root,5);
    avl_insert(&root,8);
    avl_insert(&root,2);
    avl_insert(&root,4);
    avl_insert(&root,5);
    avl_insert(&root,1);
    avl_insert(&root,2);
    avl_insert(&root,9);
    avl_insert(&root,7);
    avl_insert(&root,6);
    avl_insert(&root,12);
    avl_insert(&root,4);
    avl_insert(&root,8);
    avl_pre_order(root);
    printf("\n");
    
    avl_mid_order(root);    
    printf("\n");
    
    avl_post_order(root);    
    printf("\n");
    
    printf("before delete\n");
    printf("the depth is %d\n",avl_max_depth(root));
    printf("the ldepth is %d\n",avl_max_depth(root->left));
    printf("the rdepth is %d\n",avl_max_depth(root->right));
    printf("the count node is %d\n",avl_count_node(root));
    printf("the tree is %sbalanced.\n",avl_is_balanced(root)?"":"not ");
    avl_mirror(root);
    printf("mirror tree\n");
    avl_pre_order(root);
    printf("\n");
    
    avl_mid_order(root);    
    printf("\n");
    
    avl_post_order(root);    
    printf("\n");
//    deleteall(&root);  
    avl_destroy(&root);  
    printf("\n");
    
    printf("after delete\n");
    avl_mid_order(root);    
    printf("\n");
    printf("the depth is %d\n",avl_max_depth(root));
    printf("the count node is %d\n",avl_count_node(root));
    
    //test for insert balance
    rootbalance=avl_insert_node(rootbalance,1);
    rootbalance=avl_insert_node(rootbalance,9);
    rootbalance=avl_insert_node(rootbalance,23);
    rootbalance=avl_insert_node(rootbalance,5);
    rootbalance=avl_insert_node(rootbalance,8);
    rootbalance=avl_insert_node(rootbalance,6);
    rootbalance=avl_insert_node(rootbalance,2);
    rootbalance=avl_insert_node(rootbalance,4);
    rootbalance=avl_insert_node(rootbalance,10);
    rootbalance=avl_insert_node(rootbalance,11);
    rootbalance=avl_insert_node(rootbalance,12);
    rootbalance=avl_insert_node(rootbalance,7);
    rootbalance=avl_insert_node(rootbalance,15);    
    rootbalance=avl_insert_node(rootbalance,5);
    rootbalance=avl_insert_node(rootbalance,11);
    rootbalance=avl_insert_node(rootbalance,23);
    rootbalance=avl_insert_node(rootbalance,18);
    rootbalance=avl_insert_node(rootbalance,19);
    printf("the tree is %sbalanced.\n",avl_is_balanced(rootbalance)?"":"not ");
    avl_level_order(rootbalance);
    printf("\n");
    
    //test for delete balance
    rootbalance=avl_delete_node(rootbalance,12);
    printf("the tree is %sbalanced.\n",avl_is_balanced(rootbalance)?"":"not ");
    avl_level_order(rootbalance);
    printf("\n");   

    //test for delete balance
    rootbalance=avl_delete_node(rootbalance,8);
    printf("the tree is %sbalanced.\n",avl_is_balanced(rootbalance)?"":"not ");
    avl_level_order(rootbalance);
    printf("\n");        
    return 0;
}
