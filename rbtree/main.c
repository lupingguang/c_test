#include <stdio.h>

#include <time.h>
#include <stdlib.h>
#include "rbtree.h"
#include "rbtree_ex.h"
#define RBTREE_MAX_NUM 10

int main(int argc, char *argv[])  
{  
    struct custom_node_t *tmp = NULL;
    struct rb_root mytree = RB_ROOT; 
    
    srand((unsigned int)time(NULL));

    unsigned int i = 0;
    printf("[print original key]\n");
    for(i = 0; i < RBTREE_MAX_NUM; ++i)
    {
        
        int key = rand()%100;
        
        printf("%d ", key);
        
        int ret = _node_insert(&mytree, key);  
        if (ret < 0) 
        {  
            fprintf(stderr, "The %d already exists\n", key);  
        }
    }
    printf("\n");    
    printf("[print node from three in orde]");
    _node_printf(&mytree); 
    
    char buf[10];
    
    while(1)
    {
        printf("\n****************************************");
        printf("\n* +number to insert :");
        printf("\n* -number to delete :");
        printf("\n*       q to quit   :");
        printf("\n****************************************\n");
        
        fgets(buf, sizeof(buf), stdin);

        if(buf[0] == 'q')
        {
            break;
        }
        else if(buf[0] == '+')
        {
           
            tmp = _node_search(&mytree, atoi(&buf[1]));     
            if (tmp == NULL)
            {
                int ret = _node_insert(&mytree, atoi(&buf[1]));
            }
            else
            {
                 printf("The %d already exists\n", atoi(&buf[1]));
            }
        }
        else if(buf[0] == '-')
        {
            tmp = _node_search(&mytree, atoi(&buf[1]));
            if (tmp == NULL)
            {
                printf("The %d no exists\n", atoi(&buf[1]));
            }
            else
            {
		_node_delete(&mytree, atoi(&buf[1]));
            }

        }
        else
        {
            continue;
        }
        
        _node_printf(&mytree); 

    }

    _node_destory(&mytree);

    return 0;  
}
