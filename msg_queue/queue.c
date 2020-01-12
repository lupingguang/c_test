#include "queue.h"
#include "stdio.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <sys/inotify.h>
#include <stdlib.h>
#include <errno.h>

void queue_init(struct queue_info *info)
{
    //info->head = head;
    //info->mutex = mutex;
    //info->cond = cond;
    //node_init(info->head); //头结点的next和prev都指向自身
    //info->push = queue_push;
    //info->pop = queue_pop;
    //info->top = queue_top;
    //info->is_empty = queue_is_empty;
    //info->for_each = queue_for_each;
}

int queue_is_empty(struct queue_info *info)
{

    return  list_empty(info->head);

}

void queue_push(struct queue_info *info, struct list_head *new_node)
{
    pthread_mutex_lock(&info->mutex);
    //printf("lock:%s\n",strerror(errno));
    if(info->list_type == 1)
        list_add_tail(new_node, info->head);
    else
	list_add(new_node,info->head);
    pthread_cond_signal(&info->cond);
    pthread_mutex_unlock(&info->mutex);
}

struct list_head* queue_top(struct queue_info *info)
{
    if (queue_is_empty(info)) {
        return NULL; //队列为空
    }
    else{
        return info->head->next;
    }   
}

struct list_head* queue_pop(struct queue_info *info)                   
{
    pthread_mutex_lock(&info->mutex);
    if (queue_is_empty(info)) {
        pthread_cond_wait(&info->cond, &info->mutex);
        struct list_head *temp = info->head->next;
        list_del(temp); //删除队列的首元素
        pthread_mutex_unlock(&info->mutex);
        printf("[read thread] wake by cond\n");
        return temp;
    }
    else
    {
        struct list_head *temp = info->head->next;
        list_del(temp); //删除队列的首元素
        pthread_mutex_unlock(&info->mutex);
        printf("[read thread] wake immidate\n");
        return temp;
    }

}

int queue_for_each(struct queue_info *info, void (*todo)(struct list_head *node))
{
    if(queue_is_empty(info)){
        printf("the queue is empty\n");
        return -1;
    }
    else{
        struct list_head *pos = NULL;
        struct list_head *n = NULL;
        list_for_each_safe(pos, n, info->head)
            todo(pos);
        return 0;
    }
}

void queue_destroy(struct queue_info *info)
{
}
//---------------------------------------------------------------------------------------------
int event_send(struct queue_info* queue, struct event_info* env)
{
   struct event_info* event = (struct event_info*)malloc(sizeof(struct event_info));
   event->event_id = env->event_id;
   memcpy(event->event_params, env->event_params, sizeof(int) * 10);
   queue_push(queue, &event->list);
}

int event_recv(struct queue_info* queue, struct event_info* env)
{
    struct event_info *pevent;
    struct list_head* node = queue_pop(queue);

    pevent = container_of(node, struct event_info, list);
    env->event_id = pevent->event_id;
    memcpy(env->event_params, pevent->event_params,sizeof(int) * 10);
    free(pevent);
    //printf("name:%s, age:%d\n",pdata->name, pdata->age);

}

int event_list_each(struct queue_info *info, void (*todo)(struct list_head *node))
{
    if(queue_is_empty(info)){
        printf("the event queue is empty\n");
        return -1;
    }
    else{
        struct list_head *pos = NULL;
        struct list_head *n = NULL;
        list_for_each_safe(pos, n, info->head)
            todo(pos);
        return 0;
    }
}

void event_init(struct queue_info *info)
{
    //inode_init(info->head); //头结点的next和prev都指向自身
}

void event_destory(struct queue_info *info)
{
    struct event_info *pevent;
    
    if(queue_is_empty(info)){
        printf("[event destory]the event queue is empty, no need destory\n");
        return;
    }
    else{
        struct list_head *pos = NULL;
        struct list_head *n = NULL;
        list_for_each_safe(pos, n, info->head)
        {
            pevent = container_of(pos, struct event_info, list);
            printf("[event destory]event_id:%d, params:%d\n",pevent->event_id, pevent->event_params[0]);
            free(pevent);
        }
        return;
    }
}

