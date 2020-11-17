#include"BPTree.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h> 


KeyType Unavailable = -1;
pBPlusTree __CombineWithRight(pBPlusTree T);
pBPlusTree __CombineWithLeft(pBPlusTree T);

void __MoveRightToLeft(pBPlusTree T);
void __MoveLeftToRight(pBPlusTree T);

//#define printf //

/* 生成节点并初始化 */
pBPlusNode MallocNewNode()
{
    pBPlusNode NewNode;
    int i;
    NewNode = (pBPlusTree)malloc(sizeof(struct BPlusNode));

    i = 1;
    while (i < M + 1){
        NewNode->Key[i] = Unavailable;
        NewNode->StData[i] = ' ';
        NewNode->Children[i] = NULL;
        i++;
    }
    NewNode->Next = NULL;
    NewNode->Parent = NULL;
    NewNode->KeyNum = 0;

    return NewNode;
}

/* 初始化 */
pBPlusTree Initialize(){

    pBPlusTree T;
    /* 根结点 */
    T = MallocNewNode();

    return T;
}

/* 给予叶子节点的横向链表，遍历叶子节点*/
void TraversalLeaf(pBPlusTree T)
{
    pBPlusNode TempNode = T;
    unsigned int index = 1;
    while(TempNode->Children[1] != NULL)
    {
        TempNode = TempNode->Children[1];
    }
    while( TempNode != NULL)
    {
        printf("[%s]Traversal Leaf,leaf: %p\n", __FUNCTION__,TempNode);
        for(index =1; index <=TempNode->KeyNum; index++)
        {
            printf("    [%s] key[%d] = %d\n", __FUNCTION__, index, TempNode->Key[index]);
        }
        TempNode = TempNode->Next;
    }
}

/* 查询B+树中是否含有某个key ，无论找到与否，最后一定返回叶子节点地址*/
SearchResult SearchKey(pBPlusTree T, KeyType KeyValue)
{
    int index = 0;
    pBPlusNode TempNode = T;
    pBPlusNode ParentNode =NULL;
    SearchResult Res;

    Res.KeyIndex = 1;
    Res.NodePtr  = TempNode;
    Res.NodeParentPtr = NULL;
    Res.staus = 0;

    if(TempNode->Children[1] == NULL)
    {
        for(index = 1; index <= TempNode->KeyNum; index++) //顺序遍历该节点的所有key
        {
            if( TempNode->Key[index ] >= KeyValue) 
            {
                if( TempNode->Key[index] == KeyValue)
                    Res.staus = 1;
                else
                    Res.staus = 0;
                goto OK;
            }
            else //当前key小于查询key，继续for循环
            {
                Res.staus = 0;
                continue;
            }
        }
OK:
        Res.KeyIndex = index ;
        Res.NodePtr  = TempNode;
        Res.NodeParentPtr = ParentNode;
        printf("[%s] ,Only seach root node index = %d, KeyValue:%d, Res.staus:%d\n", __FUNCTION__, index, KeyValue, Res.staus);
        return Res;
    }

    while(1) //  一直遍历到叶子节点
    {
        printf("[%s]search node key:%d\n", __FUNCTION__, KeyValue);
        ParentNode = TempNode->Parent == NULL? TempNode:(TempNode->Parent);

        if(TempNode->Key[1] > KeyValue) //遍历的当前节点第一个key大于查询key，直接向下查找第一个子节点
        {
            printf("[%s]search next most left node key:%d\n", __FUNCTION__,KeyValue);
            Res.KeyIndex = 1;
            Res.NodePtr  = TempNode;

            if(TempNode->Children[1] != NULL)
            {
                ParentNode = TempNode;
                Res.NodeParentPtr = ParentNode;
                TempNode = TempNode->Children[1];
            }
            else
            {
                return Res;
            }
        }
        else if(TempNode->Key[TempNode->KeyNum] < KeyValue) //遍历的当前节点最后一个key小于等于查询key，若有子节点，直接向下查找后节点
        {
            printf("[%s]search next most right node key:%d \n", __FUNCTION__, KeyValue);

            Res.KeyIndex = TempNode->KeyNum + 1;
            Res.NodePtr  = TempNode;

            if(TempNode->Children[TempNode->KeyNum] != NULL)
            {
                ParentNode = TempNode;
                Res.NodeParentPtr = ParentNode;
                TempNode = TempNode->Children[TempNode->KeyNum];
            }
            else
            {
                return Res;
            }
        }
        else //遍历的当前节点中间某一个key大于查询key，需顺序查找
        {
            printf("[%s]search middle or leaf node all, key %d  cur node:%p\n", __FUNCTION__, KeyValue, TempNode);
            for(index = 1; index <= TempNode->KeyNum; index++) //顺序遍历该节点的所有key
            {
                if( TempNode->Key[index] < KeyValue) //当前key小于查询key，继续for循环
                    continue;
                else if(TempNode->Children[index] != NULL)//当前key大于等于查询key且有后续叶子节点，跳出for循环，继续执行外层while
                {
                    printf("[%s]search this middle or leaf node all key %d,cur node:%p\n", __FUNCTION__, KeyValue, TempNode);

                    Res.KeyIndex = index;
                    Res.NodePtr  = TempNode;
                    Res.NodeParentPtr = ParentNode;
                    if( TempNode->Key[index] == KeyValue) //当前key等于查询key，查询成功，但是不返回,需在叶子节点也找到该key
                    {
                        Res.staus = 1;
                        printf("[%s]search success key %d in middle node,keep search, cur node:%p\n", __FUNCTION__,KeyValue, TempNode);
                        break;
                    }
                    else
                    {
                        
                        Res.staus = 0; //当前key不等于查询key，查询失败，遍历下一级叶子节点
                        printf("[%s]search this middle node fail , goto next node, key:%d, cur node:%p\n", __FUNCTION__, KeyValue, TempNode);
                        break;
                    }
                }
                else //当前key大于等于查询key且无后续叶子节点
                {
                    Res.KeyIndex = index ;
                    Res.NodePtr  = TempNode;
                    Res.NodeParentPtr = ParentNode;
                    if( TempNode->Key[index] == KeyValue) //当前key等于查询key，查询成功，返回对应的叶子节点地址与key索引
                        Res.staus = 1;
                    else
                        Res.staus = 0; //当前key不等于查询key，查询失败，返回查询key可插入的叶子节点地址与key索引

                    printf("[%s]search status:%d key:%d in leaf node, parent:%p\n", __FUNCTION__, Res.staus,KeyValue , ParentNode);
                    return Res;
                }
            }
            TempNode = TempNode->Children[index];
        }
    }
    return Res;
}

pBPlusTree SpliteNode(pBPlusTree T, pBPlusNode Node)
{
    pBPlusNode newNode;
    pBPlusNode parentNode;
    pBPlusNode newRootNode;
    unsigned char isNewRoot = 0;
    /* 新分裂结点 */
    newNode = MallocNewNode();
    //新根节点先与T一致
    newRootNode = T;
    //若待分裂节点为根节点，需重新生成根节点
    if(Node->Parent == NULL)
    {
        newRootNode = MallocNewNode();
        Node->Parent = newRootNode;
        //新的根节点，只包含待分裂节点与新分裂出的节点，因此key也只有两个
        newRootNode->Key[1] = Node->Key[LIMIT_M_2];
        newRootNode->Key[2] = Node->Key[Node->KeyNum];
        newRootNode->Children[1] = newNode;
        newRootNode->Children[2] = Node;
        newRootNode->KeyNum = 2;
        parentNode = newRootNode;
        isNewRoot = 1;
        printf("create new root node\n");
    }
    else
    {
        parentNode = Node->Parent;
        printf("no need create new root node, keep parent node as :%p\n",parentNode);
    }

    //将原节点前半部分key移入新建node,并清空原节点前半部分key相关数据
    for( unsigned int index = 1; index <= (LIMIT_M_2 ) ; index++)
    {
        newNode->Key[index ] = Node->Key[index ];
        newNode->Children[index] = Node->Children[index ];
        newNode->StData[index] = Node->StData[index];
        newNode->KeyNum++;
    }
    //将新节点的子节点的父节点更新为自己
    for(unsigned int index4 = 1; index4 <= newNode->KeyNum; index4++)
    {
        if(newNode->Children[index4] != NULL)
        {
            newNode->Children[index4]->Parent = newNode;
        }
    }

    newNode->Parent = parentNode;
    newNode->Next  = Node;

    //取巧,横向指针未设计prev指针，所以分裂新节点后，要依赖父节点转接找到左兄弟节点，并将其next指向新分裂节点
    for(unsigned int chd = 1; chd <= newNode->Parent->KeyNum; chd++)
    {
        if((newNode->Parent->Children[chd] == Node) && (chd != 1))
        {
            newNode->Parent->Children[chd -1 ]->Next = newNode;
            break;
        //printf(" %p next: %p prev: %p\n",newNode, newNode->Next, newNode->Parent->Children[chd]);
        }
    }


    //将原节点后半部分key移入前半部分，并清空后半部分数据
    for(unsigned int index1 = 1 ; index1 <= (unsigned int)((Node->KeyNum) - LIMIT_M_2); index1++)
    {
        Node->Key[index1 ] = Node->Key[index1 + LIMIT_M_2];
        Node->Children[index1] = Node->Children[index1 + LIMIT_M_2];
        Node->StData[index1] = Node->StData[index1 + LIMIT_M_2];

     }
    for(unsigned int index2 = LIMIT_M_2 +2 ; index2 <= Node->KeyNum; index2++)
    {
        Node->Key[index2 ] = Unavailable;
        Node->Children[index2] = NULL;
        Node->StData[index2] = Unavailable;
     }
    //原节点减去LIMIT_M_2个key
    Node->KeyNum = Node->KeyNum - LIMIT_M_2;
    Node->Parent = parentNode;

    //原节点的父节点(新生成的父节点除外)key从某处后移，容纳新节点的最后一个key放入
    if(!isNewRoot)
    {
        for(int index1 = 1; index1 <= (parentNode->KeyNum); index1++) //顺序遍历该节点的所有key
        {
            if( (parentNode->Key[index1]) < (newNode->Key[LIMIT_M_2]))
                continue;
            else
            {
                for(int subindex = M; subindex >= index1; subindex--)
                {
                    parentNode->Key[subindex + 1] = parentNode->Key[subindex ];
                    parentNode->Children[subindex + 1] = parentNode->Children[subindex];
                }
                parentNode->Key[index1 ] = newNode->Key[(LIMIT_M_2 )];
                parentNode->Children[index1 ] = newNode;
                parentNode->KeyNum++;
                parentNode->Next =NULL;
                break;
            }
        }
    }
    
    //判断是否需递归分裂父节点
    if(parentNode->KeyNum > M)
        //递归分裂，注意不生成新根节点时，newRootNode = T,否则新根节点由最深递归函数生成
        newRootNode = SpliteNode(newRootNode ,parentNode);
    //newRootNode可能为下一层递归生成的新的节点，也可能保持为本次递归传入的T
    return newRootNode;

}

/* 插入key,最后一定在叶子节点插入*/
pBPlusTree InsertKey(pBPlusTree T, KeyType KeyValue, Data data)
{
      /*
    [2 65 ]
    [1 2 ] [3 4 65 ] */


    SearchResult Res;
    Res = SearchKey( T, KeyValue);
    if(Res.staus == 1)
    {
        printf("key:%d in tree already\n", KeyValue);
        return T;
    }
    else
    {
        printf("[%s]insert key:%d, data:%c, position:%d\n", __FUNCTION__,KeyValue, data , Res.KeyIndex);
        pBPlusNode CurNode = Res.NodePtr;
        //在节点key数组的中间插入，需后移一个空间
        if(Res.KeyIndex <= Res.NodePtr->KeyNum)
        {
            for(int index = CurNode->KeyNum; index >= Res.KeyIndex; index--)
            {
                CurNode->Key[index + 1] = CurNode->Key[index];
                CurNode->Children[index + 1] = CurNode->Children[index];
                CurNode->StData[index + 1] = CurNode->StData[index];
            }
        }

        CurNode->Key[Res.KeyIndex] = KeyValue;
        CurNode->Children[Res.KeyIndex] = NULL;
        CurNode->StData[Res.KeyIndex] = data;
        CurNode->Parent = Res.NodeParentPtr;
        CurNode->KeyNum++;

        //叶子节点新插入的key过大，更新所有父节点最大key
        if(T->Key[T->KeyNum] < KeyValue && CurNode->Parent != NULL)
        {
            printf("[%s]KeyValue:%d is big update all parent key\n", __FUNCTION__,KeyValue);
            T->Key[T->KeyNum] = KeyValue;
            pBPlusNode Node = T;
            while (Node->Parent != NULL)
            {
                Node = Node->Parent;
                printf("[%s]Key:%d change to %d\n",\
                       __FUNCTION__, Node->Key[Node->KeyNum], KeyValue);
                Node->Key[Node->KeyNum] = KeyValue;
            }
        }

        if(Res.NodePtr->KeyNum <= M)
            return T;
        else
        {
            printf("leaf3 node :%p, T.KEY[1]:%d key[2]: %d key[3]: %d key[4]: %d key[5]: %dnmum:%d\n",CurNode,CurNode->Key[1], CurNode->Key[2], CurNode->Key[3],CurNode->Key[4],CurNode->Key[5],CurNode->KeyNum);
            T = SpliteNode(T,CurNode);
            return T;
        }
    }
}
//更新同一路径下的所有node的最大key，删除时使用
int __updateMaxKeyValue(pBPlusNode T, KeyType DeleteKey, KeyType Key)
{

    pBPlusNode TEMP = T;
    KeyType DKey = DeleteKey;
    KeyType tempKey = Key;
    unsigned int index =1;
    //printf("[%d]\n",__LINE__);

    if(TEMP->Parent != NULL)
    {
        //如果父节点的最后一个key等于删除key，那么需要递归更新父节点的父节点的最后一个key
        if(TEMP->Parent->Key[TEMP->Parent->KeyNum] == DKey)
        {
            __updateMaxKeyValue(TEMP->Parent, DKey, Key);
        }
        for(; index <= TEMP->Parent->KeyNum; index++)
        {
            if(TEMP->Parent->Children[index] == TEMP)
                break;
        }
        printf("[%s][%d] change %d to %d\n",__FUNCTION__, __LINE__, TEMP->Parent->Key[index], Key);
        TEMP->Parent->Key[index] = Key;
    }
    
    return 1;
    
}

//删除key，是借用还是组合的决策函数
pBPlusTree __TreeModifyMethod(pBPlusTree T)
{
    pBPlusNode CurNode = T;
    unsigned int pos = 1;
    pBPlusNode NewRoot =T;


    if(CurNode->Parent == NULL && CurNode->KeyNum < 2)
    {
        //根节点key数量至少为两个，否则直接删除该根节点，其唯一的子节点作为新根节点

        NewRoot = CurNode->Children[1];
        NewRoot->Parent = NULL;
        printf("[%s][%d] NEW ROOT %p key:%d num:%d\n",__FUNCTION__,__LINE__,NewRoot,NewRoot->Key[1],NewRoot->KeyNum);
        if(NewRoot->Children[1] != NULL)
        {
            for(unsigned int g = 1; g <= NewRoot->KeyNum; g++)
            {
                //这里不清楚孩子节点的父节点为啥不是当前NewRoot，懒得查了，直接更新一遍

                    NewRoot->Children[g]->Parent = NewRoot;
                printf("[%s][%d] child[%d] %p key:%d num:%d\n",__FUNCTION__,__LINE__,g, NewRoot->Children[g],NewRoot->Children[g]->Key[1],NewRoot->Children[g]->KeyNum);
            }
        }
        printf("[%s][%d] delete node:%p key[1]:%d\n",__FUNCTION__,__LINE__, CurNode,CurNode->Key[1]);

        free(CurNode);
        CurNode = NULL;

        return NewRoot;
    }
    else
    {
        //返回之前的root node
        while( NewRoot->Parent != NULL)
        {
            NewRoot = NewRoot->Parent;
        }

    }
    //printf("[%d]CurNode->Parent:%p, CurNode->KeyNum:%d\n",__LINE__,CurNode->Parent,CurNode->KeyNum);

    //3、判断删除key后，该节点key数量是否小于LIMIT_M_2(根节点除外)，不小于则删除结束，否则执行步骤4与步骤5
    if(CurNode->Parent != NULL && CurNode->KeyNum < LIMIT_M_2)
    {
        //先找到当前节点在父节点的索引
        for(pos; pos <= T->Parent->KeyNum; pos++)
        {
            if(T->Parent->Children[pos] == T)
                break;
        }

        //4、若满足一下三种情况之一，则借用节点(注意同步更新父节点对应key)，否则执行步骤5
        //4.1 若待删除key所在节点为最左侧节点
        if(CurNode->Parent->Children[1] == CurNode)
        {
            //且右兄弟节点可借，则借兄弟的第一个key，删除结束,否则执行步骤5
            if(CurNode->Parent->Children[2]->KeyNum > LIMIT_M_2)
            {
                //借右到左
                __MoveRightToLeft(CurNode);
                return NewRoot;
            }
            //5.1 且右兄弟节点可合并(合并后key数量不大于M)，则与右兄弟节点合并，删除结束
            else
            {
                //与右合并
                return __CombineWithRight(CurNode);;
            }
        }
        //4.2 若待删除key所在节点为最右侧节点
        else if(CurNode->Parent->Children[CurNode->Parent->KeyNum] == CurNode)
        {
            //且左兄弟节点可借，则借兄弟的最后一个key,否则执行步骤5
            if(CurNode->Parent->Children[CurNode->Parent->KeyNum - 1 ]->KeyNum > LIMIT_M_2)
            {
                //借左到右
                __MoveLeftToRight(CurNode);
                return NewRoot;
            }
            //5.2 且左兄弟节点合并，则与左兄弟节点合并，删除结束
            else
            {
                //与左合并
                 printf("[%s][%d] combine with left node\n",__FUNCTION__,__LINE__);
                return __CombineWithLeft(CurNode);
            }

        }
        //4.3 若待删除key所在节点为中间位置节点
        else
        {
            //printf("[%d]\n",__LINE__);

            //右兄弟节点可借，删除结束
            if(CurNode->Parent->Children[pos + 1]->KeyNum > LIMIT_M_2)
            {
                //借右到左
                //printf("[%d]\n",__LINE__);
                __MoveRightToLeft(CurNode);
                return NewRoot;
            }
            //左兄弟节点可借，删除结束
            else if(CurNode->Parent->Children[pos - 1]->KeyNum > LIMIT_M_2)
            {
                //借左到右
                //printf("[%d]\n",__LINE__);
                __MoveLeftToRight(CurNode);
               return NewRoot;
            }
            //5.3 右节点不可借，但可以合并
            else if(CurNode->Parent->Children[pos + 1]->KeyNum + CurNode->KeyNum <=  M)
            {
                //与右合并
                printf("[%s][%d] combine with right node\n",__FUNCTION__,__LINE__);
                return __CombineWithRight(CurNode);
            }

            //5.3 左节点不可借当可合并
            else if(CurNode->Parent->Children[pos - 1]->KeyNum + CurNode->KeyNum <=  M)
            {
                //与左合并
                printf("[%s][%d] combine with left node\n",__FUNCTION__,__LINE__);
                return __CombineWithLeft(CurNode);
            }
        }
    }
    else
    {
        return NewRoot;
    }
}

//当前key数量不够的node与其左兄弟合并
pBPlusTree __CombineWithLeft(pBPlusTree T)
{
    //  [4        65         70 ]
    //[3 4 ]   [5 65 ]   [67 70 ]
    unsigned int index = 1;

    unsigned int pos = 1;
    pBPlusTree NewRoot = T;
    //先找到当前节点在父节点的索引
    for(pos; pos <= T->Parent->KeyNum; pos++)
    {
        if(T->Parent->Children[pos] == T)
            break;
    }
    printf("[%s][%d] get parent index of child: %d total num %d\n",__FUNCTION__,__LINE__,pos,T->Parent->KeyNum);


    unsigned int offset = T->Parent->Children[pos-1]->KeyNum + 1;

    //当前节点key依次放入左节点key数组后面位置
    for(index = offset; index < offset + T->KeyNum; index++)
    {
        T->Parent->Children[pos-1]->Key[index] = T->Key[index - offset + 1];
        T->Parent->Children[pos-1]->Children[index] = T->Children[index - offset + 1];
        T->Parent->Children[pos-1]->KeyNum += T->KeyNum;
    }

    //当前节点的父节点也需要更新key
    for(index = pos; index < T->Parent->KeyNum; index++)
    {
        T->Parent->Key[index] = T->Parent->Key[index + 1];
        T->Parent->Children[index] = T->Parent->Children[index + 1];
        printf("[%s][%d]parent key[%d] %d to key[%d] %d\n",__FUNCTION__,__LINE__,index,T->Parent->Key[index],index,T->Parent->Key[index+1]);
    }
    T->Parent->Key[T->Parent->KeyNum] = -1;
    T->Parent->Children[T->Parent->KeyNum] = NULL;
    T->Parent->KeyNum--;

    //父节点更新后数量不足，需继续对父节点进行借用或组合调整
    if( T->Parent->KeyNum < LIMIT_M_2)
    {
        printf("[%s][%d]parent keynum %d not enough\n",__FUNCTION__,__LINE__,T->Parent->KeyNum);
        NewRoot = __TreeModifyMethod(T->Parent);
    }
    else
    {
        while( NewRoot->Parent != NULL)
        {
            NewRoot = NewRoot->Parent;
        }

    }
    printf("[%s][%d] delete node:%p key[1]:%d, root:%p \n",__FUNCTION__,__LINE__, T,T->Key[1],NewRoot);
    free(T);
    T = NULL;
    return NewRoot;

}

//当前key数量不够的node与其右兄弟合并
pBPlusTree __CombineWithRight(pBPlusTree T)
{
    unsigned int index = 1;
    unsigned int offset = T->KeyNum;
    unsigned int pos = 1;
    pBPlusTree NewRoot = T;
    //先找到当前节点在父节点的索引
    for(pos; pos <= T->Parent->KeyNum; pos++)
    {
        if(T->Parent->Children[pos] == T)
            break;
    }
    printf("[%s][%d] get parent index of child: %d total num %d\n",__FUNCTION__,__LINE__,pos,T->Parent->KeyNum);
    //右节点key向后移动offset位置
    for(index = T->Parent->Children[pos+1]->KeyNum; index >= 1; index--)
    {
        T->Parent->Children[pos+1]->Key[index + offset] = T->Parent->Children[pos+1]->Key[index];
        T->Parent->Children[pos+1]->Children[index + offset] = T->Parent->Children[pos+1]->Children[index];
    }

    //当前节点key依次放入右节点key数组
    for(index = 1; index <= T->KeyNum; index++)
    {
        T->Parent->Children[pos+1]->Key[index] = T->Key[index];
        T->Parent->Children[pos+1]->Children[index] = T->Children[index];
    }
    T->Parent->Children[pos+1]->KeyNum += T->KeyNum;

    //当前节点的父节点也需要更新key
    for(index = pos; index < T->Parent->KeyNum; index++)
    {
        T->Parent->Key[index] = T->Parent->Key[index + 1];
        T->Parent->Children[index] = T->Parent->Children[index + 1];
        printf("[%s][%d]parent key[%d] %d to key[%d] %d\n",__FUNCTION__,__LINE__,index,T->Parent->Key[index],index,T->Parent->Key[index+1]);
    }
    T->Parent->Key[T->Parent->KeyNum] = -1;
    T->Parent->Children[T->Parent->KeyNum] = NULL;
    T->Parent->KeyNum--;

    //父节点更新后数量不足，需继续对父节点进行借用或组合调整
    if( T->Parent->KeyNum < LIMIT_M_2)
    {
        printf("[%s][%d]parent keynum %d not enough\n",__FUNCTION__,__LINE__,T->Parent->KeyNum);
        NewRoot = __TreeModifyMethod(T->Parent);
    }
    else
    {
        while( NewRoot->Parent != NULL)
        {
            NewRoot = NewRoot->Parent;
        }
    }
    printf("[%s][%d] delete node:%p key[1]:%d\n",__FUNCTION__,__LINE__, T,T->Key[1]);
    free(T);
    T = NULL;
    return NewRoot;

}

//左兄弟节点的最后一个key移动到当前节点的第一个key位置
void __MoveLeftToRight(pBPlusTree T)
{
    unsigned int index = 1;
    for(index; index <= T->Parent->KeyNum; index++)
    {
        if(T->Parent->Children[index] == T)
            break;
    }


    //更新父节点key为当前节点的左兄弟节点的倒数第二个key
    T->Parent->Key[index -1] = T->Parent->Children[index-1]->Key[T->Parent->Children[index-1]->KeyNum -1];


    //当前节点key与Children后移一位
    for(unsigned int index1= T->KeyNum; index1 > 0 ; index1--)
    {
        T->Key[index1 + 1 ] = T->Key[index1];
        T->Children[index1 + 1 ] = T->Children[index1];
    }

    //空出的位置放置左兄弟节点的最后一个key与Children
    T->KeyNum++;
    T->Key[1] = T->Parent->Children[index-1]->Key[T->Parent->Children[index-1]->KeyNum];
    if(T->Children[1] != NULL)
        T->Children[1] = T->Parent->Children[index-1]->Children[T->Parent->Children[index-1]->KeyNum];

    //左兄弟节点的keyNum -1
    T->Parent->Children[index-1]->KeyNum--;

}

//右兄弟节点的第一个key移动到当前节点的最后一个key位置
void __MoveRightToLeft(pBPlusTree T)
{
    unsigned int index = 1;
    for(index; index <= T->Parent->KeyNum; index++)
    {
        if(T->Parent->Children[index] == T)
            break;
    }

    //更新父节点key为当前节点的右兄弟节点的第一个key
    T->Parent->Key[index] = T->Parent->Children[index+1]->Key[1];
    //增加当前节点的最后一个key为右兄弟节点的第一个key
    T->Key[T->KeyNum + 1] = T->Parent->Children[index+1]->Key[1];
    //增加当前节点的最后一个Children为右兄弟节点的第一个Children
    T->Children[T->KeyNum + 1] = T->Parent->Children[index+1]->Children[1];
    T->KeyNum++;

    //右兄弟节点key与Children前移一位
    T->Parent->Children[index+1]->KeyNum--;

    for(unsigned int index1= 1; index1 <= T->Parent->Children[index + 1]->KeyNum; index1++)
    {
        T->Parent->Children[index+1]->Key[index1] = T->Parent->Children[index+1]->Key[index1 + 1];
        T->Parent->Children[index+1]->Children[index1] = T->Parent->Children[index+1]->Children[index1 + 1];
    }
}

/* 删除key,首先在叶子节点插入，如删除叶子节点中的最大值，则需要以此更新父节点、父父节点...的key*/
/* 步骤：
        1、查找待删除key所在树节点，查找失败直接返回
        2、删除key(注意剩余key位置移动)，若删除最大值，则需要依次更新父节点、父父节点...的key
        3、判断删除key后，该节点key数量是否小于LIMIT_M_2，不小于则删除结束，否则执行步骤4
        4、若满足一下三种情况之一，则借用节点(注意同步更新父节点对应key)，否则执行步骤5
            4.1 若待删除key所在节点为最左侧节点，且右兄弟节点可借，则借兄弟的第一个key，删除结束，否则执行步骤5
            4.2 若待删除key所在节点为最右侧节点，且左兄弟节点可借，则借兄弟的最后一个key，删除结束，否则执行步骤5
            4.3 若待删除key所在节点为中间位置节点，则依次判断右兄弟节点、左兄弟节点是否可借，删除结束，否则执行步骤5
        5、若满足一下三种情况之一，则合并节点(合并后，父节点的key可能小于LIMIT_M_2，此时递归执行以上步骤)
            5.1 若待删除key所在节点为最左侧节点，且右兄弟节点可合并(合并后key数量不大于M)，则与右兄弟节点合并，删除结束
            5.2 若待删除key所在节点为最右侧节点，且左兄弟节点可合并，则与左兄弟节点合并，删除结束
            5.3 若待删除key所在节点为中间位置节点，则依次判断右兄弟节点、左兄弟节点是否可合并，删除结束
*/
pBPlusTree DeleteKey(pBPlusTree T, KeyType KeyValue)
{
    SearchResult Res;
    Res = SearchKey( T, KeyValue);
    //1、查找待删除key所在树节点，查找失败直接返回
    if(Res.staus == 0)
    {
        printf("key:%d not in tree , delete fail!\n", KeyValue);
        return 0;
    }

    printf("[%s]delete key: %d position:%d\n", __FUNCTION__, KeyValue , Res.KeyIndex);
    pBPlusNode CurNode = Res.NodePtr;

    //2、删除key(注意剩余key位置移动)，若删除最大值，则需要依次更新父节点、父父节点...的key
    //若删除key在数组的中间，将后续key依次前移一个空间
    if(Res.KeyIndex != Res.NodePtr->KeyNum)
    {
        for(int index = Res.KeyIndex; index < CurNode->KeyNum; index++)
        {
            CurNode->Key[index ] = CurNode->Key[index + 1];
            if(CurNode->Children[index] != NULL)
            {
                CurNode->Children[index ] = CurNode->Children[index + 1];
                CurNode->StData[index] = CurNode->StData[index + 1];
            }
        }
        CurNode->Key[ CurNode->KeyNum] = -1;
        CurNode->KeyNum--;
    }
    //若删除key在数组的末尾（最大值）,需要更新父节点为当前节点的倒数第二个key
    else
    {
        __updateMaxKeyValue(CurNode, CurNode->Key[CurNode->KeyNum], CurNode->Key[CurNode->KeyNum -1]);
        CurNode->Key[ CurNode->KeyNum] = -1;
        CurNode->KeyNum--;
    }

    //是否需对树调整
    pBPlusTree T1 =__TreeModifyMethod(CurNode);;
    return T1;

}

/****************************以下为打印树所需函数********************************/

//计算树深度
int countDeep(pBPlusTree T)
{
    int deepCnt = 1;
    pBPlusNode TEMP = T;

    while(TEMP->Children[1] != NULL)
    {
        deepCnt++;
        TEMP = TEMP->Children[1];
    }
    //printf("deeps:%d\n",deepCnt );
    return deepCnt;
}
//判断节点是否在最左侧路径上
int __judgeMostLeft(pBPlusNode T)
{
    pBPlusNode TEMP = T;
    int isModeLest = 1;
    while(TEMP->Parent != NULL)
    {
        if(TEMP->Parent->Children[1] == TEMP)
        {
            isModeLest = 1;
            TEMP = TEMP->Parent;
            //printf("1judge result:%d node:%p\n",isModeLest ,TEMP);
        }
        else
        {
            //printf("2judge result:%d  node:%p\n",isModeLest ,TEMP);
            isModeLest = 0;
            break;
        }
    }
    //printf("judge result:%d  node:%p\n",isModeLest ,TEMP);
    return isModeLest;
}

print_info_Header * __dumpAllTree(pBPlusNode T, int levelNum)
{
    static print_info_Header * pInfoHeader = NULL;
    static int deepCnt = 0;

    print_info_Node * pInfoNode = NULL;
    pBPlusNode TempNode = T;
    int needHeader;
    int endloop;
    int level = levelNum;

    if(T->Parent == NULL)//根节点
    {
        deepCnt = countDeep(T); //求树深度
        pInfoHeader = (print_info_Header * )malloc(deepCnt * sizeof(print_info_Header));//深度是多少就申请几个header
        printf("[%s]root node: %d addr:%p \n",__FUNCTION__,TempNode->Key[1],T);
    }

    if(TempNode->Parent == NULL) //根节点需要创建header，且只需将一个节点入链表
    {
        needHeader = 1;
        endloop = 1;
    }
    else if(__judgeMostLeft(TempNode))//中间或者叶子节点只有最左侧节点要创建header，入链表节点数量为父节点的孩子节点数量
    {
        needHeader = 1;
        endloop = TempNode->Parent->KeyNum;
        printf("[%s]middle or leaf left most node:%d addr:%p endloop:%d\n",__FUNCTION__,TempNode->Key[1], T, endloop);
    }
    else//其它节点无需创建链表header，入链表节点数量为父节点的孩子节点数量
    {
        needHeader = 0;
        endloop = TempNode->Parent->KeyNum;
        printf("[%s]middle or leaf normal node:%d  addr %p endloop:%d, parent:%p\n",__FUNCTION__,TempNode->Key[1], TempNode,endloop,TempNode->Parent);
    }

    if(needHeader == 1)//root节点需要创建header、中间/叶子节点只有最左侧节点要创建header
    {
        //一定要用堆分配head，否则函数执行完毕，栈变量会释放
        struct list_head * head = (list_head * )malloc(sizeof(list_head));
        head->next = head;
        head->prev = head;
        pInfoHeader[level].head = head;
        pInfoHeader[level].TreeLevel = level;
        printf("[%s]root or middle or leaf node:%d  make header, level:%d, head:%p\n",__FUNCTION__,TempNode->Key[1], level, (pInfoHeader[level].head));
    }

    //根节点入链表单独去做
    if(TempNode->Parent == NULL)
    {
        //链表中的节点需要动态申请
        pInfoNode = (print_info_Node * )malloc(sizeof(print_info_Node));
        pInfoNode->NodePtr = TempNode;
        list_add_tail(&(pInfoNode->list), pInfoHeader[0].head);
        if(TempNode->Children[1] != NULL)
        {
            for(unsigned int chd =1; chd <= TempNode->KeyNum; chd++)
            {
                __dumpAllTree(TempNode->Children[chd], 1);
            }
        }
        return pInfoHeader;
    }

    for(unsigned int index = 1; index <= endloop; index++)
    {
        //只有当前节点为父节点的最左侧子节点时才进行入链表操作(实际是把父节点的所有孩子节点全部入链表)
        if(TempNode->Parent->Children[1] == TempNode)
        {
            //链表中的节点需要动态申请
            pInfoNode = (print_info_Node * )malloc(sizeof(print_info_Node));
            pInfoNode->NodePtr = TempNode->Parent->Children[index];
            list_add_tail(&(pInfoNode->list), pInfoHeader[level].head);
            printf("     [%s]middle or leaf  node: %d insert level:%d ,list:%p header:%p\n",__FUNCTION__,pInfoNode->NodePtr->Key[1], level, &(pInfoNode->list), (pInfoHeader[level].head));
        }
        //递归子节点、将其加入下一层的链表
        if(TempNode->Children[index] != NULL)
            __dumpAllTree(TempNode->Children[index], level + 1);
    }

    return pInfoHeader;
}

//dumpAllTree函数动态申请的链表头、存放链表头的结构体、链表节点需以此释放
void PrintAllTree(pBPlusNode T)
{
    int deepCnt =0;
    int cnt;
    struct list_head *pos =NULL;
    struct list_head *n =NULL;
    pBPlusNode TempNode =NULL;
    print_info_Node * TempInfoNode =NULL;
    print_info_Header * pInfoHeader = __dumpAllTree(T, 0);
    deepCnt = countDeep(T); //求树深度

    printf("[%s]printf whole tree with level structure:\n",__FUNCTION__);
    for(unsigned int index = 0; index < deepCnt; index++)
    {
        printf("\n ");//每次循环为树的一层的所有树节点
        list_for_each(pos, (pInfoHeader[index].head))
        {
            printf(" ["); //每次循环为一个数节点
            TempInfoNode = container_of(pos, struct print_info_Node, list);
            for(unsigned int index = 1; index <= TempInfoNode->NodePtr->KeyNum; index++)
            {
                printf("%d " , TempInfoNode->NodePtr->Key[index]); //每次循环为一个树节点的一个key
            }
            free(TempInfoNode);
            printf("]");//每次循环为一个数节点
        }
        free(pInfoHeader[index].head);
    }
    free(pInfoHeader);
    printf("\n ");

}

/****************************以上为打印树所需函数********************************/

int main()
{

//测试插入数据
    pBPlusTree T = Initialize();
    for(int index = 65; index<= 72; index++){
        printf("========================\n");
        T= InsertKey(T, index, index+1);
    }


    for(int index = 1; index<= 5; index++){
        printf("=========================\n");
        T= InsertKey(T, index, index+1);
    }

//测试通过链表遍历叶子节点
    TraversalLeaf(T);

//中序遍历整颗树并打印
    PrintAllTree(T);
    /* 原始树
    [4 72 ]
    [2 4 ] [66 68 72 ]
    [1 2 ] [3 4 ] [5 65 66 ] [67 68 ] [69 70 71 72 ]
    */


// 测试删除
    T = DeleteKey(T, 72); //测试 删除最key，则需要依次更新父节点、父父节点.的key
    PrintAllTree(T);
    /* 删除72后
    [4 71 ]
    [2 4 ] [66 68 71 ]
    [1 2 ] [3 4 ] [5 65 66 ] [67 68 ] [69 70 71 ]
    */

    T = DeleteKey(T, 68); //测试 向右借key
    PrintAllTree(T);
    /* 删除68后
    [4 71 ]
    [2 4 ] [66 69 71 ]
    [1 2 ] [3 4 ] [5 65 66 ] [67 69 ] [70 71 ]
    */

    T = DeleteKey(T, 69); //测试 向左借key
    PrintAllTree(T);

    /* 删除69后
    [4 71 ]
    [2 4 ] [65 67 71 ]
    [1 2 ] [3 4 ] [5 65 ] [66 67 ] [70 71 ]
    */

    T = DeleteKey(T, 66); //测试 向右合并节点
    PrintAllTree(T);
    /* 删除66后
    [4 71 ]
    [2 4 ] [65 71 ]
    [1 2 ] [3 4 ] [5 65 ] [67 70 71 ]
    */

    T = DeleteKey(T, 2); //测试 向右合并节点后，若父节点key不足，再递归父节点向右合并，不能合并则释放原根节点，树层数降低
    PrintAllTree(T);
    /* 删除2后
    [4 65 71 ]
    [1 3 4 ] [5 65 ] [67 70 71 ]
    */

    T = DeleteKey(T, 71);
    PrintAllTree(T);
    /* 删除71后
    [4 65 70 ]
    [1 3 4 ] [5 65 ] [67 70 ]
    */
    
    T = DeleteKey(T, 1);
    PrintAllTree(T);
    /* 删除1后
    [4 65 70 ]
    [3 4 ] [5 65 ] [67 70 ]
    */
    T = DeleteKey(T, 70); //测试 向左合并节点
    PrintAllTree(T);

    /* 删除70后
    [4 65 ]
    [3 4 ] [5 65 67 ]
    */
    T = DeleteKey(T, 67);
    PrintAllTree(T);
    /* 删除67后
    [4 65 ]
    [3 4 ] [5 65 ]
    */

    T = DeleteKey(T, 5); //测试 向左合并节点，若父节点key不足，再递归父节点向右合并，不能合并则释放原根节点，树层数降低
    PrintAllTree(T);
    /* 删除5后
    [3 4 65 ]
    */
    PrintAllTree(T);

    return 0;
}
