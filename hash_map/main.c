#include "hash_list.h"
#include <stdio.h>
struct q_coef
{
    unsigned char coef;
    unsigned char index;
    struct hlist_node hash;
};

#define HASH_NUMBER 15
unsigned char coef[HASH_NUMBER] = {
    0x01, 0x02, 0x04, 0x08,0x10, 0x20, 0x40, 0x80, 0x1d, 0x3a, 0x74, 0xe8, 0xce, 0x87, 0x13,
};
struct q_coef q_coef_list[HASH_NUMBER];

struct hlist_head hashtbl[HASH_NUMBER];

static inline int hash_func(unsigned char k)
{
    int a, b, p, m;
    a = 104;
    b = 52;
    p = 233;
    m = HASH_NUMBER;
    return ((a * k + b) % p) % m;
}

static void hash_init(void)
{
    int i, j;
    for (i = 0 ; i < HASH_NUMBER ; i++) {
        INIT_HLIST_HEAD(&hashtbl[i]);
        INIT_HLIST_NODE(&q_coef_list[i].hash);
        q_coef_list[i].coef = coef[i];
        q_coef_list[i].index = i;
    }
    for (i = 0 ; i < HASH_NUMBER ; i++) {
        j = hash_func(q_coef_list[i].coef);
        //if ( i == 6) j = 10;
        hlist_add_head(&q_coef_list[i].hash, &hashtbl[j]);
    }
}

static void hash_test(void)
{
    int i, j;
    struct q_coef *q;
    struct hlist_node *hn;
    //桶定义为15大小，正好与待存数据量一致，但是哈希函数计算得到的key仍然可能冲突
    for (i = 0 ; i < HASH_NUMBER ; i++) 
    {
        j = hash_func(coef[i]);
        //printf("key =%d, value=%x member index=%d\n",j, coef[i],i);
        //不冲突的key，那么hashtbl只会指向一个node，node的next为NULL
        //若冲突，那么hashtbl指向以该key为key的首个node，node的next继续指向下一个相同key的元素
        //针对同一个tbl位置(key冲突)存储多个value情况，会打印出该key对应的所有value(该tbl位置存储的链表上所有的元素)
        //判断链表结束的条件是:node节点的next成员指向NULL(后续无节点)
        hn = hashtbl[j].first; 
        struct hlist_node *temp_hn = NULL;
        do
        {
            q = container_of(hn, struct q_coef, hash);
            printf("found:index=%d key=0x%02x array index=%d, addr=%p\n",j,  q->coef, q->index, q);
            temp_hn = hn;
            hn = hn->next;
        }
        while(temp_hn->next != NULL);

        /*内核自带的遍历宏由prefatch函数，没搞懂怎么去掉，所以下面的宏移植有问题，不可用
        hlist_for_each(hn, &hashtbl[j])
        {
            q = container_of(hn, struct q_coef, hash);
            printf("found:index=%d key=0x%02x array index=%d, addr=%p\n",j,  q->coef, q->index, q);
        }*/
    }
}

int main(void)
{
   hash_init();
   hash_test();
}
