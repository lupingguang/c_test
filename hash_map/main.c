#include "hash_list.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct q_coef
{
    unsigned char coef;
    unsigned char index;
    struct hlist_node hash;
};

#define HASH_NUMBER 10
unsigned char coef[15] = {
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

static struct q_coef* hash_query(unsigned char ckey)
{
        struct q_coef *q;
        struct hlist_node *hn;
	struct hlist_node *temp_hn = NULL;
        struct q_coef *hash_node = NULL;
	int j = hash_func(ckey);
        hn = hashtbl[j].first;
        
        do
        {
               q = container_of(hn, struct q_coef, hash);
               if(q->coef == ckey)
               {
			//printf("find ckey =%d, in bucket index %d\n", ckey, j);
                        hash_node = q;
			break;
	       }
               temp_hn = hn;
               hn = hn->next;
        }
        while(temp_hn->next != NULL);
        return hash_node;
}

int hash_delete(unsigned char key)
{
	struct q_coef* hash_node = hash_query(key);
	if(hash_node != NULL)
	{
        	printf("delete key =0x%02x, in bucket\n", hash_node->coef);
		hlist_del_init(&(hash_node->hash));
		return 1;
	}
	else
	{
     		printf("cant find key in bucket\n");
		return -1;
	}
}

int hash_add(unsigned char key, unsigned char index)
{
	int j = hash_func(key);
        if(q_coef_list[j].coef == 0xff)
        {
                q_coef_list[j].coef = key;
                q_coef_list[j].index = index;
                hlist_add_head(&q_coef_list[j].hash, &hashtbl[j]);
                printf("add key to bucket %d,addr:%p\n",j, &q_coef_list[j]);
        }
        else
        {
                struct q_coef * dy_q_coef = (struct q_coef *)malloc(sizeof(struct q_coef));
                dy_q_coef->coef = key;
                dy_q_coef->index = index;
                INIT_HLIST_NODE(&(dy_q_coef->hash));
                printf("add key to bucket %d list, key=0x%02x, index=%d,addr:%p\n",j, key, index, (dy_q_coef));
                hlist_add_head(&(dy_q_coef->hash), &hashtbl[j]);

        }
	return 1;

}

static void hash_init(void)
{
    int i, j;
    struct q_coef *dy_q_coef = NULL;
    for (i = 0 ; i < HASH_NUMBER ; i++) {
        INIT_HLIST_HEAD(&hashtbl[i]);
        INIT_HLIST_NODE(&q_coef_list[i].hash);
        q_coef_list[i].coef = 0xff;
        q_coef_list[i].index = 0;
    }
    for (i = 0 ; i < 15 ; i++) {
        j = hash_func(coef[i]);
        if(q_coef_list[j].coef == 0xff)
        {
	        q_coef_list[j].coef = coef[i];
	        q_coef_list[j].index = i;
                hlist_add_head(&q_coef_list[j].hash, &hashtbl[j]);
		//printf("j1= %d, i =%d addr:%p\n",j,i, &q_coef_list[j]);
	}
        else
	{
                dy_q_coef = (struct q_coef *)malloc(sizeof(struct q_coef));
                dy_q_coef->coef = coef[i];
                dy_q_coef->index = i;
                INIT_HLIST_NODE(&(dy_q_coef->hash));
	 	//printf("j2= %d, i =%d add:%p\n",j,i,(dy_q_coef));
		hlist_add_head(&(dy_q_coef->hash), &hashtbl[j]);

	}
     }
    
}

static void hash_test(void)
{
    int i, j;
    struct q_coef *q;
    struct hlist_node *hn;

    //桶定义为10大小，哈希函数计算得到的key冲突
    for (i = 0 ; i < 15 ; i++) 
    {
        j = hash_func(coef[i]);
        //printf("key =%d, value=%x member index=%d\n",j, coef[i],i);
        //不冲突的key，那么hashtbl只会指向一个node，node的next为NULL
        //若冲突，那么hashtbl指向以该key为key的首个node，node的next继续指向下一个相同key的元素
        //针对同一个tbl位置(key冲突)存储多个value情况，会打印出该key对应的所有value(该tbl位置存储的链表上所有的元素)
        //判断链表结束的条件是:node节点的next成员指向NULL(后续无节点)
        hn = hashtbl[j].first; 
        
	{
        	struct hlist_node *temp_hn = NULL;
        	do
        	{
            		q = container_of(hn, struct q_coef, hash);
            		printf("bucket-index=%d key=0x%02x value=%d addr:%p\n",j,  q->coef, q->index, q);
            		temp_hn = hn;
            		hn = hn->next;
        	}
        	while(temp_hn->next != NULL);
        }
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
   	struct q_coef * hash_node = hash_query(0x3a);
   	if(hash_node != NULL)
   	{
		printf("query ckey =0x%02x, in bucket\n", hash_node->coef);
   	}
   	hash_delete(0x3a);
	printf("after delete key: 0x3a,show data\n");
   	hash_test();
	hash_add(0x3a, 100);
   	printf("after add key: 0x3a,show data\n");
   	hash_test();

}
