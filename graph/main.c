/**
 * C: 邻接表表示的"有向图(List Directed Graph)"
 *
 * @author skywang
 * @date 2014/04/18
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#define MAX 100
#define isLetter(a)  ((((a)>='a')&&((a)<='z')) || (((a)>='A')&&((a)<='Z')))
#define LENGTH(a)  (sizeof(a)/sizeof(a[0]))

// 邻接表中的边
typedef struct _ENode
{
    int ivex;                   // 该边所指向的终止顶点的位置
    struct _ENode *next_edge;   // 指向下一条边的指针
}ENode, *PENode;

// 邻接表中表的顶点
typedef struct _VNode
{
    char data;              // 顶点信息
    ENode *first_edge;      // 指向第一条依附该顶点的边
}VNode;

// 邻接表
typedef struct _LGraph
{
    int vexnum;             // 图的顶点的数目
    int edgnum;             // 图的边的数目
    VNode vexs[MAX];        // 每个顶点VNode为数组一个元素，每个元素上有一条链表，链表元素为边ENode
}LGraph;

/*
 * 返回ch在matrix矩阵中的位置
 */
static int get_position(LGraph g, char ch)
{
    int i;
    for(i=0; i<g.vexnum; i++)
        if(g.vexs[i].data==ch)
            return i;
    return -1;
}

/*
 * 读取一个输入字符
 */
static char read_char()
{
    char ch;

    do {
        ch = getchar();
    } while(!isLetter(ch));

    return ch;
}

/*
 * 将node链接到list的末尾
 */
static void link_last(ENode *list, ENode *node)
{
    ENode *p = list;

    while(p->next_edge)
        p = p->next_edge;
    p->next_edge = node;
}

int visited[1];
//对图g中第i个顶点，递归完成深度优先遍历
void DFS(LGraph g, int i)
{
    ENode *p;
    printf("| %c ",g.vexs[i].data);
    visited[i] = 1;
    p = g.vexs[i].first_edge;
    while( NULL != p)
    {
	if(visited[p->ivex] ==0)
            DFS(g,p->ivex);
         p = p->next_edge;
    }
}

//深度优先遍历图g,找出所有顶点
void DFStraverse(LGraph g)
{
    int v;
    for (v=0; v < g.vexnum; v++) 
        visited[v] =0;
    for (v=0; v < g.vexnum; v++)
        if (!visited[v])
            DFS(g, v);
    printf("\n");
}
//深度优先遍历查找两个顶点之间的所有路径
void Path_DFS(LGraph G, int i, int j)
{
	/*
        B[1]->C[2]->E[4]
	B[1]->E[4]
        */
	static int n = 0;
	static char vertex[2];//路径经过的顶点中的数据
        //打标记，表明某个顶点访问过了
	visited[i] = 1;
	//n代表某一条path上已经访问到第几个顶点了
        n++;
        
	vertex[n] = G.vexs[i].data;
        
        //递归遍历起始顶点链表上所有可到达的顶点
	ENode *p = G.vexs[i].first_edge;
	while(p)
	{
		if(visited[p->ivex] == 0)
			Path_DFS(G, p->ivex, j);
		p = p->next_edge;
	}
	//若某次递归到达了终止顶点，则打印出来
	if(i == j)
	{
		for(int m = 1; m <= n; m++)
		        if(m < n)
			    printf("%d(%c)->",get_position(G, vertex[m]), vertex[m]);
		        else
		            printf("%d(%c)",get_position(G, vertex[m]), vertex[m]); 
		printf("\n");
	}


        //无论这次递归是否到达终止顶点，都回退当前遍历过得顶点，一步步回到分叉顶点，再次遍历另一个分支
        //清标记，证明对于某个顶点的访问已结束，现在回退为未访问状态
        visited[i] = 0;
	n--;       
}

void SimplePath(LGraph G, int i, int j)
{
	int m;
	//int on[100];
	for(m = 1; m <= G.vexnum; m++)
		visited[m] = 0;
	Path_DFS(G, i, j);
}

/*
 * 创建邻接表对应的图(自己输入)
 */
LGraph* create_lgraph()
{
    char c1, c2;
    int v, e;
    int i, p1, p2;
    ENode *node1, *node2;
    LGraph* pG;

    // 输入"顶点数"和"边数"
    printf("input vertex number: ");
    scanf("%d", &v);
    printf("input edge number: ");
    scanf("%d", &e);
    if ( v < 1 || e < 1 || (e > (v * (v-1))))
    {
        printf("input error: invalid parameters!\n");
        return NULL;
    }
 
    if ((pG=(LGraph*)malloc(sizeof(LGraph))) == NULL )
        return NULL;
    memset(pG, 0, sizeof(LGraph));

    // 初始化"顶点数"和"边数"
    pG->vexnum = v;
    pG->edgnum = e;
    // 初始化"邻接表"的顶点
    for(i=0; i<pG->vexnum; i++)
    {
        printf("vertex(%d): ", i);
        pG->vexs[i].data = read_char();
        pG->vexs[i].first_edge = NULL;
    }

    // 初始化"邻接表"的边
    for(i=0; i<pG->edgnum; i++)
    {
        // 读取边的起始顶点和结束顶点
        printf("edge(%d):from dest ", i);
        c1 = read_char();
        c2 = read_char();

        p1 = get_position(*pG, c1);
        p2 = get_position(*pG, c2);
        // 初始化node1
        node1 = (ENode*)malloc(sizeof(ENode));
        node1->ivex             = p2;
        // 将node1链接到"p1所在链表的末尾"
        if(pG->vexs[p1].first_edge == NULL)
          pG->vexs[p1].first_edge = node1;
        else
            link_last(pG->vexs[p1].first_edge, node1);
    }

    return pG;
}

/*
 * 创建邻接表对应的图(用已提供的数据)
 */
LGraph* create_example_lgraph()
{
    char c1, c2;
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    char edges[][2] = {
        {'A', 'B'}, 
        {'B', 'C'}, 
        {'B', 'E'}, 
        {'B', 'F'}, 
        {'C', 'E'}, 
        {'D', 'C'}, 
        {'E', 'B'}, 
        {'E', 'D'}, 
        {'F', 'G'},
        {'G', 'E'}}; 
    int vlen = LENGTH(vexs);
    int elen = LENGTH(edges);
    int i, p1, p2;
    ENode *node1, *node2;
    LGraph* pG;


    if ((pG=(LGraph*)malloc(sizeof(LGraph))) == NULL )
        return NULL;
    memset(pG, 0, sizeof(LGraph));

    // 初始化"顶点数"和"边数"
    pG->vexnum = vlen;
    pG->edgnum = elen;
    // 填充"邻接表"的顶点
    for(i=0; i<pG->vexnum; i++)
    {
        pG->vexs[i].data = vexs[i];
        pG->vexs[i].first_edge = NULL;
    }

    // 填充"邻接表"的边
    for(i=0; i<pG->edgnum; i++)
    {
        // 读取边的起始顶点和结束顶点
        c1 = edges[i][0];
        c2 = edges[i][1];

        p1 = get_position(*pG, c1);
        p2 = get_position(*pG, c2);
        // 初始化node1
        node1 = (ENode*)malloc(sizeof(ENode));
        node1->ivex = p2;
        // 将node1链接到"p1所在链表的末尾"
        if(pG->vexs[p1].first_edge == NULL)
          pG->vexs[p1].first_edge = node1;
        else
            link_last(pG->vexs[p1].first_edge, node1);
    }

    return pG;
}

/*
 * 打印邻接表图
 */
void print_lgraph(LGraph G)
{
    int i,j;
    ENode *node;

    printf("List Graph:\n");
    for (i = 0; i < G.vexnum; i++)
    {
        printf("%d(%c): -> ", i, G.vexs[i].data);
        node = G.vexs[i].first_edge;
        while (node != NULL)
        {
            printf("%d(%c) ", node->ivex, G.vexs[node->ivex].data);
            node = node->next_edge;
        }
        printf("\n");
    }
}

void main()
{
    LGraph* pG;

    // 自定义"图"(自己输入数据)
    //pG = create_lgraph();
    // 采用已有的"图"
    pG = create_example_lgraph();

    // 打印图的邻居表
    print_lgraph(*pG);
    //深度优先遍历找出所有顶点
    //DFStraverse(*pG);

    int start, end;
    char s ,e;
    printf("input vertex start:\n");
    scanf("%s", &s);
    start = get_position(*pG, s);
    printf("input vertex end:\n");
    scanf("%s", &e);
    end = get_position(*pG, e);

    printf("the path from %c to %c:\n", s, e);
    //深度优先找出某两个顶点之间的所有路径
    SimplePath(*pG, start, end);
}
