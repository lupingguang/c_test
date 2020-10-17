#include <stdio.h>
#include <stdlib.h>

#define LEFT 1
#define RIGHT 2

typedef struct BSTreeNode
{
    int data;
    struct BSTreeNode *left;//左子树
    struct BSTreeNode *right;//右子树
}BSTree;

void PreOrder_Traverse(BSTree *nod, int level);    //前序遍历二叉树，并打印
void SearchData(int targ, BSTree *nod);    //查找特定值
BSTree *AddNewNode(BSTree *cur, int NewData);    //添加新的节点
BSTree *DeletNode( BSTree *RootNode, int DelData);    //删除节点
BSTree *SearchSuccessorNode(BSTree *nod);    //搜索后继节点
BSTree *SearchParentofSNode(BSTree *Pnod, BSTree *nod);    //搜索后继节点的父节点

int main()
{
    BSTree RootNod;
    int num;
    //创建树的根节点
    RootNod.data = 88;
    RootNod.left = NULL;
    RootNod.right = NULL;
    
    for(int i=0; i<10; i++)
    {

        printf("输入新插入数据\n");
        scanf("%d", &num);
        AddNewNode(&RootNod, num);
    }

    PreOrder_Traverse(&RootNod, 1);

    printf("输入查找数据\n");
    scanf("%d", &num);
    SearchData(num, &RootNod);

    printf("输入删除的数据\n");
    scanf("%d", &num);
    DeletNode( &RootNod, num);


    PreOrder_Traverse(&RootNod, 1);

    return 0;
}

/* 搜索后继节点的父节点 */
BSTree *SearchParentofSNode(BSTree *Pnod, BSTree *nod)
{
    while (1)
    {
        if (nod->left != NULL)
        {
            Pnod = nod;
            nod = nod->left;
        }
        else
        {
            break;
        }
    }

    return Pnod;
}

/* 搜索后继节点 */
BSTree *SearchSuccessorNode(BSTree *nod)
{
    while (1)
    {
        if (nod->left != NULL)
        {
            nod = nod->left;
        }
        else
        {
            break;
        }
    }

    return nod;
}

/* 删除节点 */
BSTree *DeletNode(BSTree *cur, int DelData)
{
    BSTree *SNode = NULL; //后继节点
    BSTree *PSNode = NULL;    //后继节点的父节点
    BSTree *temp = NULL;    //临时保存待释放节点的子树，避免free后找不到左右子树


    if (cur == NULL)
    {
        printf("删除节点不存在\n");
        exit(0);
    }
    else if (DelData > cur->data)
    {
        cur->right = DeletNode(cur->right, DelData);
    }
    else if (DelData < cur->data)
    {
        cur->left = DeletNode(cur->left, DelData);
    }
    else if (DelData == cur->data)
    {
        if (cur->left == NULL && cur->right == NULL)    //待删除节点为叶子节点
        {
            free(cur);
            return NULL;
        }
        else if(cur->left != NULL && cur->right == NULL)    //待删除节点只有左子树
        {
            temp = cur->left;
            free(cur);

            return temp;;
        }
        else if(cur->left == NULL && cur->right != NULL)    //待删除节点只有右子树
        {
            temp = cur->right;
            free(cur);

            return cur->right;
        }
        else if(cur->left != NULL && cur->right != NULL)    //待删除节点既有左子树也有右子树
        {
            SNode = SearchSuccessorNode(cur->right);    //搜索后继节点
            PSNode = SearchParentofSNode(cur->right, cur->right);    //搜索后继节点的父节点

            if (cur->right == SNode)    //后继节点为待删除节点的右子树（后继节点有右子树和没有右子树的操作相同）
            {
                SNode->left = cur->left;
                free(cur);

                return SNode;
            }
            else if (cur->right != SNode && SNode->right == NULL)    //后继节点不为待删除节点的右子树，并且该后继节点没有右子树
            {
                SNode->left = cur->left;
                SNode->right = cur->right;
                PSNode->left = NULL;
                free(cur);

                return SNode;
            }
            else if (cur->right != SNode && SNode->right != NULL)    //后继节点不为待删除节点的右子树，并且该后继节点有右子树
            {

                PSNode->left = SNode->right;    //后继节点的右子树作为后继节点父节点的左子树
                SNode->left = cur->left;
                SNode->right = cur->right;
                free(cur);

                return SNode;
            }
        }
    }

    return cur;
}

/* 添加新节点 */
BSTree *AddNewNode(BSTree *cur, int NewData)
{
    if (cur == NULL)
    {
        if ((cur = (BSTree *)malloc(sizeof(BSTree))) == NULL)    //创建新节点
        {
            printf("内存不足");
            exit(0);
        }
        cur->data = NewData;
        cur->left = NULL;
        cur->right = NULL;

        return cur;
    }
    if (NewData > cur->data)
    {
        cur->right = AddNewNode(cur->right, NewData);
    }
    else if (NewData < cur->data)
    {
        cur->left = AddNewNode(cur->left, NewData);
    }
    else if (NewData == cur->data)
    {
        printf("不允许插入重复值\n");
        exit(0);
    }

    return cur;
}

/* 查找特定值 */
void SearchData(int targ, BSTree *nod)
{
    if (nod != NULL)
    {
        if (nod->data == targ)
        {
            printf("查找值存在，值为%d\n", nod->data);
        }
        else if (nod->data > targ)
        {
            SearchData(targ, nod->left);    //递归查找左子树
        }
        else if (nod->data < targ)
        {
            SearchData(targ, nod->right);    //递归查找右子树
        }
    }
    else if (nod == NULL)
    {
        printf("查找值不存在\n");
    }
}


/* 前序遍历二叉树，并打印 */
void PreOrder_Traverse(BSTree *nod, int level)
{
    if (nod == NULL)
    {
        return ;
    }

    printf("data = %d level = %d\n", nod->data, level);
    PreOrder_Traverse(nod->left, level + 1);
    PreOrder_Traverse(nod->right, level + 1);
}
