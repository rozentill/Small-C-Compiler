#include <stdio.h>
#include <string.h>

typedef struct AVLTreeNode{
  char type;
  char * name;

  int strMem;
  int strName;

  int arrSize;

  int h;//Height
  struct AVLTreeNode * l;//left child
  struct AVLTreeNode * r;//right child

} PAVLNode;

typedef PAVLNode AVLTree;

int MAX(int a,int b){return (a>b)?a:b;}
int Height(PAVLNode * x){return x->h;}

AVLTree * SingleRotateWithLeft(PAVLNode * k2)
{
    PAVLNode * k1;
    k1 = k2->l;
    k2->l = k1->r;
    k1->r = k2;
    k2->h = MAX( Height( k2->l ), Height( k2->r ) ) + 1;
    k1->h = MAX( Height( k1->l ), k2->h ) + 1;
    return k1;  /* New root */
}

AVLTree * SingleRotateWithRight(PAVLNode * k1)
{
    PAVLNode * k2;
    k2 = k1->r;
    k1->r = k2->l;
    k2->l = k1;
    k1->h = MAX( Height( k1->l ), Height( k1->r ) ) + 1;
    k2->h = MAX( Height( k2->r ), k1->h ) + 1;
    return k2;  /* New root */
}

AVLTree * DoubleRotateWithLeft( PAVLNode * k3 )
{
            /* Rotate between K1 and K2 */
            k3->l = SingleRotateWithRight( k3->l );
            /* Rotate between K3 and K2 */
            return SingleRotateWithLeft(k3);
}

AVLTree * DoubleRotateWithRight( PAVLNode * k1 )
{
            /* Rotate between K3 and K2 */
            k1->r = SingleRotateWithLeft( k1->r );
            /* Rotate between K1 and K2 */
            return SingleRotateWithRight( k1 );
}

AVLTree * Insert(PAVLNode * X, AVLTree * T )
 {
            if( T == NULL )
            {
                /* Create and return a one-node tree */
                T = X;

            }
            else if(strcmp(X->name,T->name)<0)//插入情况1
            {
                T->l = Insert( X, T->l );
                if( Height( T->l ) - Height( T->r ) == 2 )
                    if(strcmp(X->name,T->l->name)<0)//左边左子树 单旋转
                        T = SingleRotateWithLeft( T );
                    else
                        T = DoubleRotateWithLeft( T );//左边右子树
            }
            else if(strcmp(X->name,T->name)>0) //插入情况2
            {
                T->r = Insert( X, T->r );
                if( Height( T->r ) - Height( T->l ) == 2 )
                    if(strcmp(X->name,T->name)>0)//右边右子树 单旋转
                        T = SingleRotateWithRight( T );
                    else
                        T = DoubleRotateWithRight( T );//右边左子树
            }

            
            /* Else X is in the tree already; we'll do nothing */
            T->h = MAX( Height( T->l ), Height( T->r ) ) + 1;
            return T;
}

PAVLNode * Find(char * name,AVLTree * root){
    if (strcmp(name,root->name)<0)
    {
        Find(name,root->l);
    }
    else if (strcmp(name,root->name)>0)
    {
        Find(name,root->r);
    }
    else if (strcmp(name,root->name)==0)
    {
        return root;
    }
    else {
        return NULL;
    }
}
