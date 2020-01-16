#define _RBTREE_IMPL_EXPORT_

#include <stdlib.h>
#include "rbtree_ex.h"

struct custom_node_t *_node_search(struct rb_root *root, int key)
{
    struct rb_node *rbnode = root->rb_node;

    while (rbnode != NULL)
    {
        struct custom_node_t *_node = rb_entry(rbnode, struct custom_node_t, rb_node);

        if (key < _node->c_key)
            rbnode = rbnode->rb_left;
        else if (key > _node->c_key)
            rbnode = rbnode->rb_right;
        else
            return _node;
    }

    return NULL;
}

int _node_insert(struct rb_root *root, int key)
{
    struct rb_node **_tmp_rb = &(root->rb_node), *parent = NULL;

    /* Figure out where to put new node */
    while (*_tmp_rb)
    {
        struct custom_node_t *_node_tmp = rb_entry(*_tmp_rb, struct custom_node_t, rb_node);

        parent = *_tmp_rb;
        if (key < _node_tmp->c_key)
            _tmp_rb = &((*_tmp_rb)->rb_left);
        else if (key > _node_tmp->c_key)
            _tmp_rb = &((*_tmp_rb)->rb_right);
        else
            return -1;
    }
    struct custom_node_t *tmp = malloc(sizeof(struct custom_node_t));
    tmp->c_key = key;
    /* Add new node and rebalance tree. */
    rb_link_node(&tmp->rb_node, parent, _tmp_rb);
    rb_insert_color(&tmp->rb_node, root);

    return 0;
}

void _node_delete(struct rb_root *root, int key)
{
    struct custom_node_t *_node;

    if ((_node = _node_search(root, key)) == NULL)
        return;

    rb_erase(&_node->rb_node, root);
    free(_node);
}


void _node_destory(struct rb_root *tree)
{
    struct rb_node *node;  

    //after run _node_delete, the next node tun to first node
    for(node = rb_first(tree); node; node = rb_first(tree))
    {
        _node_delete(tree, rb_entry(node, struct custom_node_t, rb_node)->c_key);
    }
}

static void print_rbtree(struct rb_node *tree, int  key, int direction)
{
    if(tree != NULL)
    {   
        if(direction==0)    // tree是根节点
           printf("%2d(B) is root\n", key);
        else                // tree是分支节点
           printf("%2d(%s) is %2d's %6s child\n", key, rb_is_black(tree)?"B":"R", key, direction==1?"right" : "left");
        if (tree->rb_left)
           print_rbtree(tree->rb_left, rb_entry(tree->rb_left, struct custom_node_t, rb_node)->c_key, -1);
        if (tree->rb_right)
           print_rbtree(tree->rb_right,rb_entry(tree->rb_right, struct custom_node_t, rb_node)->c_key,  1); 
    }   
}
void _node_printf(struct rb_root *tree)  
{  
   // if (tree!=NULL && tree->rb_node!=NULL)
   //     print_rbtree(tree->rb_node, rb_entry(tree->rb_node, struct custom_node_t, rb_node)->c_key,  0); 
    
   printf("\n"); 
   struct rb_node *node; 
   for(node = rb_first(tree); node; node = rb_next(node))
   {
        printf("%d ", rb_entry(node, struct custom_node_t, rb_node)->c_key);  
   }
   printf("\n");

}  
 
