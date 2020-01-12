#include "stdio.h"
#include "queue.h"
#include <unistd.h>
#define NAME_MAX_LEN 20
/*
struct data_info {
    char name[NAME_MAX_LEN];
    int age;
    struct list_head list ;
};

struct data_info s[] = {
        {"A", 34},
        {"B", 42},
        {"C", 36},
        {"D", 100},
        {"E", 18},
    };
*/

struct queue_info queue;    

void *read_event_handler(void*);
void *write_event_handler(void*);
/*
//此函数用于打印结点信息
void  print_node(struct list_head *node)
{
    struct data_info *pdata;
    pdata = container_of(node, struct data_info, list);
    printf("name:%s, age:%d\n",pdata->name, pdata->age);
}
*/
//此函数用于打印结点信息
void  print_event(struct list_head *node)
{
    struct event_info *pdata;
    pdata = container_of(node, struct event_info, list);
    printf("list all event event_id:%d, params[1]:%d\n",pdata->event_id, pdata->event_params[0]);
}


int main(void)
{
    LIST_HEAD(head);
    //struct list_head head;
    pthread_mutex_t mutex= PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;//init cond
    int list_type = 1;

    queue.head = &head;
    queue.mutex = mutex;
    queue.cond = cond;
    printf("input list type[1:queue,2:stack]:\n");
    scanf("%d",&list_type);
    printf("you select list type is:%d\n",list_type);
    queue.list_type = list_type;
    //event_init(&queue);
    //queue_init(&queue);
    /*
    //测试入队
    int i;

    for (i = 0; i < sizeof s/ sizeof s[0]; ++i) 
    {
        queue_push(&queue,&s[i].list);
        //printf("list [%d]= prev:%p, next:%p\n", i, &s[i].list.prev, &s[i].list.next);
        // printf("list [%d]= prev:%p, next:%p\n", i,*( &s[i].list.prev), *(&s[i].list.next));

    }
    
    printf("list [0] %x= prev:%p, next:%p\n", &s[0].list, s[0].list.prev, s[0].list.next);
    printf("list [1] %x= prev:%p, next:%p\n", &s[1].list, s[1].list.prev, s[1].list.next);
    printf("list [2] %x= prev:%p, next:%p\n", &s[2].list, s[2].list.prev, s[2].list.next);
    printf("list [3] %x= prev:%p, next:%p\n", &s[3].list, s[3].list.prev, s[3].list.next);
    printf("list [4] %x= prev:%p, next:%p\n", &s[4].list, s[4].list.prev, s[4].list.next);
    printf("queu [4] %p= prev:%p, next:%p\n", queue.head, queue.head->prev, queue.head->next);
    queue_push(&queue,&s[1].list);
    printf("list [0] %x= prev:%p, next:%p\n", &s[0].list, s[0].list.prev, s[0].list.next);
    printf("list [1] %x= prev:%p, next:%p\n", &s[1].list, s[1].list.prev, s[1].list.next);
    printf("list [2] %x= prev:%p, next:%p\n", &s[2].list, s[2].list.prev, s[2].list.next);
    printf("list [3] %x= prev:%p, next:%p\n", &s[3].list, s[3].list.prev, s[3].list.next);
    printf("list [4] %x= prev:%p, next:%p\n", &s[4].list, s[4].list.prev, s[4].list.next);
    printf("queu [4] %p= prev:%p, next:%p\n", queue.head, queue.head->prev, queue.head->next);
    
    printf("list all entry:\n");
    //测试遍历
    queue_for_each(&queue,print_node);
    printf("list top entry:\n");
    struct list_head *p_node = queue_top(&queue);
    if(p_node==NULL){
        printf("top test failed\n");
    }
    else{
        print_node(p_node);
    }
    printf("list all entry again:\n");  
    queue_for_each(&queue,print_node);
    //测试弹出队首
    printf("pop top entry:\n");
    p_node = queue_pop(&queue);
    print_node(p_node);
    printf("list all entry again:\n");  
    queue_for_each(&queue,print_node);
    */
    //测试基于list设计的消息队列/消息栈的写入与读取    
    pthread_t t_a;
    pthread_t t_b;//two thread
 
    pthread_create(&t_a,NULL,read_event_handler,(void*)NULL);
    pthread_create(&t_b,NULL,write_event_handler,(void*)NULL);//Create thread
    while(1);
}
void* read_event_handler(void* params)
{
    struct event_info event;
    while(1)
    {
        //struct list_head*  p_node = queue_pop(&queue);
        //printf("[read_thread]:\n");
        //print_node(p_node);
        //p_node = queue_top(&queue);
        //print_node(p_node);
        event_recv(&queue, &event);
        printf("[event recv],env_id=%d, env_params=%x\n",event.event_id, event.event_params[0]);
        //sleep(3);
    }
}
void* write_event_handler(void* params)
{
    int cnt =0;
    int exit = 0;
    sleep(2);
    while(1)
    {
        struct event_info l[] = {
            {1, {1,2,3}},
            {2, {4,5,6}},
            {3, {9,10,11}},
            {4, {21,22,23}},
            {5, {100}},
        };
 
        if(cnt == 5) cnt = 0;
        if(exit == 10)
        {
            event_destory(&queue);
            break;
        }
        //queue_push(&queue,&l[i].list);
        printf("[event write] send env_id=%d, env_params=%d:\n",l[cnt].event_id, l[cnt].event_params[0]);
        event_send(&queue, &l[cnt]);
        event_list_each(&queue,print_event);
        sleep(3);
        cnt++;
        exit++;

    }
}
