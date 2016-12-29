#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef bool
typedef int bool;
#define false 0
#define true 1
#endif

int max(int a, int b)
{
    return (a>b)?a:b;
}
typedef struct TreeNode
{
    int key;
    int data;
    int height;
    struct TreeNode* left;
    struct TreeNode* right;
}TreeNode;

void destroy(TreeNode**tree)
{
    if(*tree==NULL)
        return;
    destroy(&(*tree)->left);
    destroy(&(*tree)->right);
    printf("%d ",(*tree)->key);
    free(*tree);
    *tree=NULL;
}
TreeNode* newnode(int key)
{
    TreeNode* node=(TreeNode*)malloc(sizeof(TreeNode));
    node->key=key;
    node->data=key;
    node->left=NULL;
    node->right=NULL;
    node->height=1;
    return(node);
}

void insert(TreeNode** tree, int val)
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
        insert(&(*tree)->left,val);
    else if(val>(*tree)->key)
        insert(&(*tree)->right,val);
    else
        printf("Err. Cannot insert a duplicate key %d.\n",val);
}
void preOrder(TreeNode* tree)
{
    if(tree==NULL)
        return;
    printf("%d ",tree->key);
    if(tree->left!=NULL)
        preOrder(tree->left);
    if(tree->right!=NULL)
        preOrder(tree->right);
}
void midOrder(TreeNode* tree)
{
    if(tree==NULL)
        return;
    if(tree->left!=NULL)
        midOrder(tree->left);    
    printf("%d ",tree->key);
    if(tree->right!=NULL)
        midOrder(tree->right);
}
void postOrder(TreeNode* tree)
{
    if(tree==NULL)
        return;
    if(tree->left!=NULL)
        postOrder(tree->left);
    if(tree->right!=NULL)
        postOrder(tree->right);        
    printf("%d ",tree->key);
}

int height(TreeNode* N)
{
    if(N==NULL)
        return 0;
    return N->height;
}
/* get the height by traversing the tree nodes
 *
*/
int MaxDepth(TreeNode *root)
{
    int ldepth = 0;
    int rdepth = 0;
    if(root == NULL)
        return 0;
    ldepth = MaxDepth(root->left);
    rdepth = MaxDepth(root->right);
    return (ldepth > rdepth ? ldepth : rdepth) + 1;   
}
/*
 *you can get an inverse tree if you travese the tree inorder and the tree is a balaceed tree.
*/
void mirror(TreeNode*root)
{
    TreeNode *temp=NULL;
    if(root==NULL)
        return;
    mirror(root->left);
    mirror(root->right);
    temp=root->left;
    root->left=root->right;
    root->right=temp;
}
void printgivenlevel(TreeNode* T, int level)
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
        printgivenlevel(T->left,level-1);
        printgivenlevel(T->right,level-1);
    }
}

void levelOrder(TreeNode* T)
{
    int h=height(T);
    int i=0;
    for(i=1;i<=h;i++)
    {
        printgivenlevel(T,i);
        printf("\n");
    }
}

int countNode(TreeNode* root)
{
    int count=0;
    if(root==NULL)
        return 0;
    count+=countNode(root->left);
    count+=countNode(root->right);    
    count+=1;
    return count;
}

bool isbalanced(TreeNode* root)
{
    int ldepth=0;
    int rdepth=0;
    if(root==NULL)
        return true;
    ldepth=MaxDepth(root->left);
    rdepth=MaxDepth(root->right);
    if(abs(ldepth-rdepth)<=1&&true==isbalanced(root->left)&&true==isbalanced(root->right))
        return true;
    else 
        return false;
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
TreeNode* leftrotate(TreeNode* z)
{
    TreeNode* y=z->right;
    TreeNode* t2=y->left;
    
    //perform rotation
    y->left=z;
    z->right=t2;
    
    z->height=max(height(z->left),height(z->right))+1;
    y->height=max(height(y->left),height(y->right))+1;
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
TreeNode* rightrotate(TreeNode*z)
{
    TreeNode* y=z->left;
    TreeNode* t3=y->right;
    
    //perform rotation
    y->right=z;
    z->left=t3;
    
    z->height=max(height(z->left),height(z->right))+1;    
    y->height=max(height(y->left),height(y->right))+1;
    return y;
}

int getbalance(TreeNode* t)
{
    if(t==NULL)
        return 0;
    return height(t->left)-height(t->right);
}

TreeNode* insertbalance(TreeNode* node, int key)
{
    //1.perform the normal BST insertion
    if(node==NULL)
        return newnode(key);
    if(key<node->key)
        node->left=insertbalance(node->left,key);
    else if(key>node->key)
        node->right=insertbalance(node->right,key);
    else
    {
        printf("Err. Cannot insert a duplicate key %d.\n",key);
        return node;
    }
    //2.update height of this ancestor node
    node->height=1+max(height(node->left),height(node->right));
    
    //3.get the balace factor of this ancestor node to check whether this node became unbalanced.
    int balance=getbalance(node);
    
    //4.if this node becomes unbalanced, then there are 4 cases
    
    //left left case
    if(balance>1 && key<node->left->key)
        return rightrotate(node);
    //left right case
    if(balance>1 && key>node->left->key)
    {
        node->left=leftrotate(node->left);
        return rightrotate(node);
        
    }    
    //right right case
    if(balance<-1 && key>node->right->key)
        return leftrotate(node);
    //right left case
    if(balance<-1 && key<node->right->key)
    {
        node->right=rightrotate(node->right);
        return leftrotate(node);
    }

    return node;
}
/*
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
int main()
{   
    int depth=0;
    TreeNode* root=NULL; 
    TreeNode* rootbalance=NULL;    
    insert(&root,0);
    insert(&root,5);
    insert(&root,8);
    insert(&root,2);
    insert(&root,4);
    insert(&root,5);
    insert(&root,1);
    insert(&root,2);
    insert(&root,9);
    insert(&root,7);
    insert(&root,6);
    insert(&root,12);
    insert(&root,4);
    insert(&root,8);
    preOrder(root);
    printf("\n");
    
    midOrder(root);    
    printf("\n");
    
    postOrder(root);    
    printf("\n");
    
    printf("before delete\n");
    printf("the depth is %d\n",MaxDepth(root));
    printf("the ldepth is %d\n",MaxDepth(root->left));
    printf("the rdepth is %d\n",MaxDepth(root->right));
    printf("the count node is %d\n",countNode(root));
    printf("the tree is %sbalanced.\n",isbalanced(root)?"":"not ");
    mirror(root);
    printf("mirror tree\n");
    preOrder(root);
    printf("\n");
    
    midOrder(root);    
    printf("\n");
    
    postOrder(root);    
    printf("\n");
//    deleteall(&root);  
    destroy(&root);  
    printf("\n");
    
    printf("after delete\n");
    midOrder(root);    
    printf("\n");
    printf("the depth is %d\n",MaxDepth(root));
    printf("the count node is %d\n",countNode(root));
    
    rootbalance=insertbalance(rootbalance,1);
    rootbalance=insertbalance(rootbalance,9);
    rootbalance=insertbalance(rootbalance,23);
    rootbalance=insertbalance(rootbalance,5);
    rootbalance=insertbalance(rootbalance,8);
    rootbalance=insertbalance(rootbalance,6);
    rootbalance=insertbalance(rootbalance,2);
    rootbalance=insertbalance(rootbalance,4);
    rootbalance=insertbalance(rootbalance,10);
    rootbalance=insertbalance(rootbalance,11);
    rootbalance=insertbalance(rootbalance,12);
    rootbalance=insertbalance(rootbalance,7);
    rootbalance=insertbalance(rootbalance,15);    
    rootbalance=insertbalance(rootbalance,5);
    rootbalance=insertbalance(rootbalance,11);
    rootbalance=insertbalance(rootbalance,23);
    rootbalance=insertbalance(rootbalance,18);
    rootbalance=insertbalance(rootbalance,19);
    printf("the tree is %sbalanced.\n",isbalanced(rootbalance)?"":"not ");
    levelOrder(rootbalance);
    printf("\n");
    return 0;
}
