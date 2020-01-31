#include "stdio.h"
#include "list_map.h"
#include <unistd.h>

struct tiny_point points[] = 
{
    {   .point_name = "KA1", .status = 1},
    {   .point_name = "KA2", .status = 1},
    {   .point_name = "KA3", .status = 1},
    {   .point_name = "KB1", .status = 0},
    {   .point_name = "KB2", .status = 1},
    {   .point_name = "KB3", .status = 1},
    {   .point_name = "KC1", .status = 1}, 
    {   .point_name = "KC2", .status = 0},
};

struct tiny_station stations[] = 
{
    { .station_name = "A0"}, { .station_name = "A1"}, { .station_name = "A2"},  { .station_name = "A3"},
    { .station_name = "B0"}, { .station_name = "B1"}, { .station_name = "B2"},  { .station_name = "B3"},
    { .station_name = "C0"}, { .station_name = "C1"}, { .station_name = "C2"},  { .station_name = "C3"}, { .station_name = "C4"},
};

struct tiny_path paths[] = 
{
    {.path_name="A0+KA1+KA2+A1"}, {.path_name="A1+KA3+A2"}, {.path_name="A2+NULL+A3"}, {.path_name="A2+NULL+B2"},
    {.path_name="B0+KB1+B1"}, {.path_name="B1+KB2+B2"}, {.path_name="B2+NULL+B3"}, {.path_name="B1+NULL+C1"}, {.path_name="B1+NULL+C3"},
    {.path_name="C0+NULL+C1"},{.path_name="C1+NULL+C2"},{.path_name="C2+KC1+C3"},  {.path_name="C3+KC2+C4"},
};

void print_point(struct list_head *node)
{
    struct tiny_point *pdata;
    pdata = container_of(node, struct tiny_point, list);
    printf("\nlist all point point_name:%s, status[1]:%d,addr:%p\n",pdata->point_name, pdata->status, pdata);
}

void print_station(struct list_head *node)
{
    struct tiny_station *pdata;
    struct tiny_path *pdata1;
    struct list_head *pos = NULL;
    pdata = container_of(node, struct tiny_station, list);
    printf("\nlist all station station_name: %s, addr:%p\n", pdata->station_name, pdata);

    list_for_each(pos, &pdata->as_source_head)
    {
         pdata1 = container_of(pos, struct tiny_path, list_for_src_station);
         printf("\npath %s 's src station is %s\n", pdata1->path_name, pdata->station_name);

    }
    pos = NULL;
    list_for_each(pos, &pdata->as_dist_head)
    {
         pdata1 = container_of(pos, struct tiny_path, list_for_dist_station);
         printf("\npath %s 's dist station is %s\n", pdata1->path_name, pdata->station_name);

    }
}

void print_path(struct list_head *node)
{
    struct tiny_path *pdata;
    struct tiny_point *pdata1;
    struct list_head *pos = NULL;
    pdata = container_of(node, struct tiny_path, list);
    printf("\nlist all path path_name: %s, src: %s, dist: %s\n", pdata->path_name, pdata->src->station_name,pdata->dist->station_name);
    list_for_each(pos, &pdata->pointof_path_head)
    {
         pdata1 = container_of(pos, struct tiny_point, list_for_path);
         printf("\npoint %s is between path %s\n", pdata1->point_name, pdata->path_name);
 
    }

}

int main(void)
{
    struct tiny_map map;
    //printf("point_list_head %p\n", &map.point_list_head);    
    map.map_name = "a simple map";
    //LIST_HEAD用于定义并初始化一个链表头
    //LIST_HEAD(head);    
    //map.point_list_head = head;
    /*此处这样用不行，因为map中的list不是指针，如果直接将head赋值给他会出现如下问题：
      
      head 0x7ffc7b82fd10
      point_list_head 0x7ffc7b82fd68
      1addr:pre 0xd87550 next 0x7ffc7b82fd10 pos 0x7ffc7b82fd68

      list all point point_name:(null), status[1]:0,addr:0x7ffc7b82fd00
      2addr:pre 0x7ffc7b82fd10 next 0xd87430 pos 0x7ffc7b82fd10

      list all point point_name:KA1, status[1]:1,addr:0xd87420
      2addr:pre 0x7ffc7b82fd10 next 0xd87460 pos 0xd87430

      list all point point_name:KA2, status[1]:1,addr:0xd87450
      2addr:pre 0xd87430 next 0xd87490 pos 0xd87460

      list all point point_name:KB1, status[1]:1,addr:0xd87480
      2addr:pre 0xd87460 next 0xd874c0 pos 0xd87490

      list all point point_name:KB2, status[1]:1,addr:0xd874b0
      2addr:pre 0xd87490 next 0xd874f0 pos 0xd874c0

      list all point point_name:KB3, status[1]:1,addr:0xd874e0
      2addr:pre 0xd874c0 next 0xd87520 pos 0xd874f0

      list all point point_name:KC1, status[1]:1,addr:0xd87510
      2addr:pre 0xd874f0 next 0xd87550 pos 0xd87520

      list all point point_name:KC2, status[1]:1,addr:0xd87540
      2addr:pre 0xd87520 next 0x7ffc7b82fd68 pos 0xd87550
    
      即出现了两个头指针....
    */
    //需使用INIT_LIST_HEAD将已有结构体中的list成员做初始化
    INIT_LIST_HEAD(&map.point_list_head);
    INIT_LIST_HEAD(&map.station_list_head);
    INIT_LIST_HEAD(&map.path_list_head); 

    //1. add point to list    
    for(int i = 0; i< sizeof(points)/sizeof(struct tiny_point); i++)
    {
       point_add(&map, &points[i]);
    }
    //2. add station to list, but station list have no relative path info
    for(int i = 0; i< sizeof(stations)/sizeof(struct tiny_station); i++)
    {
       station_add(&map, &stations[i]);
    }
    //print_for_each(&map, print_station, 2);

    //1.add path to list:
    //(1. add relative point to own list
    //(2. point to src &dist station struct 
    //(3. add relative path to station list
    for(int i = 0; i< sizeof(paths)/sizeof(struct tiny_path); i++)
    {
       path_add(&map, &paths[i]);
    }

    // print_for_each(&map, print_point, 3);
    // print_for_each(&map, print_path, 1);
    // print_for_each(&map, print_station, 2); 
    
    char input_station[10];
    int index;
    char input_point[10];
    int input_status;
    while(1)
    {
         printf("\n\ninput station[1 name]|\nmodify point status[2 name]|\nquit[3]|\n");
         scanf("%d", &index); 
         if(index == 1)
         {   
             printf("input station name [name]: \n");
             scanf("%s",&input_station);
             printf("you select station is:%s\n", input_station);
         } 
         else if (index == 2)
         {
             printf("input point name [name]: \n");
             scanf("%s", &input_point);
             printf("input point status [status]: \n");
             scanf("%d", &input_status);
             fix_point(&map, input_point, input_status);
             printf("point :%s modify status to %d\n", input_station, input_status);

         }
         else if (index == 3) 
         {
             point_destory(&map);
             station_destory(&map);
             path_destory(&map);
             break;
         }

         research_traffic_situation(&map, input_station);
    }
}
