#ifndef __QUEUE_H__
#define __QUEUE_H__
#include <pthread.h>
/*
    /usr/src/linux-headers-4.8.0-36-generic/include/linux/stddef.h 
*/

//求偏移量
#define offsetof(TYPE, MEMBER)  ((size_t)&((TYPE *)0)->MEMBER)


/*
        /usr/src/linux-headers-4.8.0-36-generic/include/linux/kernel.h 
*/
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:   the type of the container struct this is embedded in.
 * @member: the name of the member within the struct.
 *
 */

//小指针转大指针
#define container_of(ptr, type, member) ({          \
    const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})


/*
    /usr/src/linux-headers-4.8.0-36-generic/include/linux/types.h
*/
struct list_head {
    struct list_head *next, *prev;
};

/*
    /usr/src/linux-headers-4.8.0-36-generic/include/linux/list.h 
*/

#define LIST_HEAD_INIT(name) { &(name), &(name) }


//以下这个宏用来定义并且初始化头结点
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)


static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}
/*
list [0]= prev:0x6020a0, next:0x602098
list [1]= prev:0x6020c8, next:0x6020c0
list [2]= prev:0x6020f0, next:0x6020e8
list [3]= prev:0x602118, next:0x602110
list [4]= prev:0x602140, next:0x602138
queue head. prev:0x602138, head.next:0x602098
*/
/* kernel 3.14 */
static inline void __list_add(struct list_head *new,
                  struct list_head *prev,
                  struct list_head *next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;      //    kernel 4.8中 这句话是 WRITE_ONCE(prev->next, new);
}   


/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next); //头插
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head); //尾插
}



/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;       //WRITE_ONCE(prev->next, next);
}


static inline void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    INIT_LIST_HEAD(entry); //add by me
    //entry->next = LIST_POISON1;
    //entry->prev = LIST_POISON2;
}


/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
    //return READ_ONCE(head->next) == head;
}   



/**
 * list_for_each    -   iterate over a list
 * @pos:    the &struct list_head to use as a loop cursor.
 * @head:   the head for your list.
 */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)




/**
 * list_for_each_safe - iterate over a list safe against removal of list entry
 * @pos:    the &struct list_head to use as a loop cursor.
 * @n:      another &struct list_head to use as temporary storage
 * @head:   the head for your list.
 */
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)



/**
 * list_entry - get the struct for this entry
 * @ptr:    the &struct list_head pointer.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

/**
 * list_first_entry - get the first element from a list
 * @ptr:    the list head to take the element from.
 * @type:   the type of the struct this is embedded in.
 * @member: the name of the list_head within the struct.
 *
 * Note, that list is expected to be not empty.
 */
#define list_first_entry(ptr, type, member) \
    list_entry((ptr)->next, type, member)



/**
 * list_next_entry - get the next element in list
 * @pos:    the type * to cursor
 * @member: the name of the list_head within the struct.
 */
#define list_next_entry(pos, member) \
    list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 * list_for_each_entry  -   iterate over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_for_each_entry(pos, head, member)              \
    for (pos = list_first_entry(head, typeof(*pos), member);    \
         &pos->member != (head);                    \
         pos = list_next_entry(pos, member))    


/**
 * list_for_each_entry_safe - iterate over list of given type safe against removal of list entry
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 */
#define list_for_each_entry_safe(pos, n, head, member)          \
    for (pos = list_first_entry(head, typeof(*pos), member),    \
        n = list_next_entry(pos, member);           \
         &pos->member != (head);                    \
         pos = n, n = list_next_entry(n, member))



/**
 * list_for_each_entry_from - iterate over list of given type from the current point
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing from current position.
 */

//从pos指向的结构体开始遍历
#define list_for_each_entry_from(pos, head, member)             \
    for (; &pos->member != (head);                  \
         pos = list_next_entry(pos, member))    



/**
 * list_for_each_entry_safe_from - iterate over list from current point safe against removal
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Iterate over list of given type from current point, safe against
 * removal of list entry.
 */
#define list_for_each_entry_safe_from(pos, n, head, member)             \
    for (n = list_next_entry(pos, member);                  \
         &pos->member != (head);                        \
         pos = n, n = list_next_entry(n, member))   



/**
 * list_for_each_entry_continue - continue iteration over list of given type
 * @pos:    the type * to use as a loop cursor.
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Continue to iterate over list of given type, continuing after
 * the current position.
 */

//从pos的下一个开始遍历
#define list_for_each_entry_continue(pos, head, member)         \
    for (pos = list_next_entry(pos, member);            \
         &pos->member != (head);                    \
         pos = list_next_entry(pos, member))

/**
 * list_for_each_entry_safe_continue - continue list iteration safe against removal
 * @pos:    the type * to use as a loop cursor.
 * @n:      another type * to use as temporary storage
 * @head:   the head for your list.
 * @member: the name of the list_head within the struct.
 *
 * Iterate over list of given type, continuing after current point,
 * safe against removal of list entry.
 */
#define list_for_each_entry_safe_continue(pos, n, head, member)         \
    for (pos = list_next_entry(pos, member),                \
        n = list_next_entry(pos, member);               \
         &pos->member != (head);                        \
         pos = n, n = list_next_entry(n, member))


//维修点结构体
//简单表示只有修好与未修好两种状态
struct tiny_point
{
    unsigned char *point_name;
    unsigned int status;   //表示维修点当前状态
    struct list_head list; //用于将所有point串为链表
    struct list_head list_for_path; //供path头节点将自己链入一个链表，该链表中所有元素均是该path中的point，对应pointof_path_head
};

//站点结构体
//站点可能同时为多条path的起始站点或者目标站点，因此使用链表将所有关联path连起来
struct tiny_station
{
    unsigned char *station_name;
    struct list_head as_source_head;//用于将所有以该tiny_station作为起始站点的path串为链表
    struct list_head as_dist_head;  //用于将所有以该tiny_station作为目标站点的path串为链表
    struct list_head list;          //用于将所有tiny_station串为链表
};

//路径结构体
//路径的起始站点与目标站点时确定的，因此仅使用指针指向即可
struct tiny_path
{
   unsigned char *path_name;
   struct tiny_station* src; //指向该path的起始站点
   struct tiny_station* dist;//指向该path的目标站点
   struct list_head pointof_path_head;//将该path中所有的break point 串为链表
   struct list_head list; //用于将所有path串为链表
   struct list_head list_for_src_station; // 供station的头节点将自己链入一个链表，该链表中所有元素均以该station为始发站点，对应as_source_head
   struct list_head list_for_dist_station; // 供station的头节点将自己链入一个链表，该链表中所有元素均以该station为目标站点，对应as_dist_head

};

//道路地图结构体
//三种链表：path、station、point的起点
struct tiny_map
{
   unsigned char *map_name;
   struct list_head path_list_head; //path链表的头节点
   struct list_head station_list_head; //station链表的头节点
   struct list_head point_list_head; //point链表的头节点
   //struct list_head list; //用于将所有map串为链表，目前只测试一站图，因此去掉
};

int print_for_each(struct tiny_map* map, void (*todo)(struct list_head *node), char list_type);
int point_add(struct tiny_map* map, struct tiny_point* point);
int station_add(struct tiny_map* map, struct tiny_station* station);
int path_add(struct tiny_map* map, struct tiny_path* path);
void point_destory(struct tiny_map *map);
void station_destory(struct tiny_map *map);
void path_destory(struct tiny_map *map);
int research_traffic_situation(struct tiny_map* map, char *station);
int fix_point(struct tiny_map *map, char *point, int status);
#endif
