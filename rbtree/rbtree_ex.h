//
//  rbtree_ex.h
//
//  Created on 4/14.
//  Copyright (c) 2016 All rights reserved.
//

#ifndef _CORE_LINUX_RBTREE_EX_H_
#define _CORE_LINUX_RBTREE_EX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "rbtree.h"

struct custom_node_t
{
    struct rb_node rb_node;
    int c_key;
    char c_data[28];
};

struct custom_node_t *_node_search(struct rb_root *root, int key);

int _node_insert(struct rb_root *root, int key);

void _node_delete(struct rb_root *root, int key);

void _node_destory(struct rb_root *tree);

void _node_printf(struct rb_root *tree);  
#ifdef __cplusplus
}
#endif

#endif // _CORE_LINUX_RBTREE_EX_H_
