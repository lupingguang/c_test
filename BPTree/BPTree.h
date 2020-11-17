#ifndef BPTree_h
#define BPTree_h
 
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define M (4)
#define LIMIT_M_2 (M % 2 ? (M + 1)/2 : M/2)
 
typedef struct BPlusNode *pBPlusTree,*pBPlusNode;
typedef int KeyType;
typedef unsigned char Data;
struct BPlusNode{
    int KeyNum;
    KeyType Key[M + 2]; //0不存放数据
    pBPlusNode Children[M + 2]; ////0不存放数据
    pBPlusNode Parent;  //垂直父节点
    pBPlusNode Next;    //叶子节点水平链表下一个
    Data StData[M + 2];//叶子节点使用该区域存放卫星数据
};

struct SearchResult{
    int staus; //是否找到某索引
    pBPlusNode NodePtr;
    pBPlusNode NodeParentPtr;
    pBPlusNode Next;    //叶子节点水平链表下一个
    pBPlusNode Pre;     //叶子节点水平链表上一个
    int KeyIndex; //找到时表示索引所在Key数组的位置，否则代表大于待查找索引值得上一个Key数组位置
};




//以下为实现打印用到的链表、结构体，与B+树实现无关

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

//这个函数不知道内核里面有没有，我自己加的
void node_init(struct list_head *node)
{
    node->next = node;
    node->prev = node;
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
void __list_add(struct list_head * new1, struct list_head *prev, struct list_head *next)
{
    next->prev = new1;
    new1->next = next;
    new1->prev = prev;
    prev->next = new1;      //    kernel 4.8中 这句话是 WRITE_ONCE(prev->next, new);
}   


/**
 * list_add - add a new entry
 * @new: new entry to be added
 * @head: list head to add it after
 *
 * Insert a new entry after the specified head.
 * This is good for implementing stacks.
 */
void list_add(struct list_head *new1, struct list_head *head)
{
    __list_add(new1, head, head->next); //头插
}


/**
 * list_add_tail - add a new entry
 * @new: new entry to be added
 * @head: list head to add it before
 *
 * Insert a new entry before the specified head.
 * This is useful for implementing queues.
 */
void list_add_tail(struct list_head *new1, struct list_head *head)
{
    __list_add(new1, head->prev, head); //尾插
}



/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;       //WRITE_ONCE(prev->next, next);
}


void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
    node_init(entry); //add by me
    //entry->next = LIST_POISON1;
    //entry->prev = LIST_POISON2;
}


/**
 * list_empty - tests whether a list is empty
 * @head: the list to test.
 */
int list_empty(const struct list_head *head)
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

//每一层树一个info结构体，同层树节点自左向右串成链表
struct print_info_Header {
    int TreeLevel;
    //int NodeNum;
    struct list_head *head; //存储该层兄弟节点组成的链表的的头部
};

struct print_info_Node {
    pBPlusNode NodePtr;
    struct list_head list; //存储该层兄弟节点链表节点
};

#endif
