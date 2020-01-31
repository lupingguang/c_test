#include "list_map.h"
#include "stdio.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
//as_dist_head list_for_src_station
static int left_traffic_situation(struct tiny_map* map, char *station)
{
    struct list_head *pos = NULL;
    struct tiny_station *pdata; 
    int flag = 0;
    int ret  = 0;
    //find specific station in station list
    list_for_each(pos, &map->station_list_head)
    {
         pdata = container_of(pos, struct tiny_station, list);
         
         if(strcmp(pdata->station_name, station) == 0)
         {
             printf("find station : %s\n", pdata->station_name);
             flag = 1;
             break; // found station in list jump out the loop
         }
    }

    pos = NULL;
    if(flag == 1)
    {    
         //if station is not every path's dist station, means its the most left station
         //there is no point in the left of most left station, return success 
         if(list_empty(&pdata->as_dist_head))
         { 
            printf("station %s reach to most left,no need check\n",pdata->station_name); 
            return 1;
         }
         
         struct tiny_path *pdata1;
         struct tiny_point *pdata2;
         struct list_head *pos1 = NULL;
         //list each path which using the sattion as dist station
         list_for_each(pos,  &pdata->as_dist_head)
         {
             pdata1 = container_of(pos, struct tiny_path, list_for_dist_station);
             //if path have no point at all, set ret init value to success
             //because list_for_each(pos1, &pdata1->pointof_path_head) loop will not run
             if(list_empty(&pdata1->pointof_path_head))
             {
                 ret = 1;
             }

             list_for_each(pos1, &pdata1->pointof_path_head)
             {
                 pdata2 = container_of(pos1, struct tiny_point, list_for_path);
                 if(pdata2->status != 1)
                 {
                    ret = 0;
                    printf("ret =%d status %d\n",ret, pdata2->status);
                    goto pass;//if path have one breaking point,mark fail and no need check the rest point so goto pass,end the loop
                 }
                 else
                 {
                    ret = 1;
                 }
                 //printf("path_name %s point_name %s point_status %d\n", pdata1->path_name, pdata2->point_name, pdata2->status);
             }
pass:        // if first level path connected to the station is ok, recursive checking the next level path
             if(ret == 1) 
             {
                 //this level path's src station is the next level path's dist station
                 ret = left_traffic_situation(map, pdata1->src->station_name);
                 //next level path is checking failed, return to check the rest of current level path(if exist the rest path) 
                 if (ret == 0)
                    continue;
                 //found a completely successful path once upon, ret success and ignore the rest path
                 else 
                    return ret;
             }
             //printf("----------------path_name %s \n", pdata1->path_name);
         }
         return ret;         
    }
    else
    {
        // printf("\nstation: %s not be found! \n", pdata->station_name);
        return 0;
    }
    //return 0;
}

static int right_traffic_situation(struct tiny_map* map, char *station)
{
    struct list_head *pos = NULL;
    struct tiny_station *pdata; 
    int flag = 0;
    int ret  = 0;
    //find specific station in station list
    list_for_each(pos, &map->station_list_head)
    {
         pdata = container_of(pos, struct tiny_station, list);
         //find point in established point list
         if(strcmp(pdata->station_name, station) == 0)
         {
             printf("find station : %s\n", pdata->station_name);
             flag = 1;
             break;
         }
    }

    pos = NULL;
    if(flag == 1)
    {    
         
         if(list_empty(&pdata->as_source_head))
         { 
            printf("station %s reach to most right,no need check\n",pdata->station_name); 
            return 1;
         }
         
         struct tiny_path *pdata1;
         struct tiny_point *pdata2;
         struct list_head *pos1 = NULL;
         list_for_each(pos,  &pdata->as_source_head)
         {
             pdata1 = container_of(pos, struct tiny_path, list_for_src_station);
             //find point in established point list
             if(list_empty(&pdata1->pointof_path_head))
             {
                 ret = 1;
             //    goto pass;
             }

             list_for_each(pos1, &pdata1->pointof_path_head)
             {
                 pdata2 = container_of(pos1, struct tiny_point, list_for_path);
                 if(pdata2->status != 1)
                 {
                    ret = 0;
                    printf("ret =%d status %d\n",ret, pdata2->status);
                    goto pass;
                    //break;
                 }
                 else
                 {
                    ret = 1;
                 }
                 //printf("path_name %s point_name %s point_status %d\n", pdata1->path_name, pdata2->point_name, pdata2->status);
             }
pass:
             if(ret == 1) 
             {
                 ret = right_traffic_situation(map, pdata1->dist->station_name);
                 if (ret == 0)
                    continue;
                 else 
                    return ret;
             }
             //printf("----------------path_name %s \n", pdata1->path_name);
         }
         return ret;         
    }
    else
    {
        // printf("\nstation: %s not be found! \n", pdata->station_name);
        return 0;
    }
}

int research_traffic_situation(struct tiny_map* map, char *station)
{
    int ret;
    ret = left_traffic_situation(map, station);
    if (ret == 0) 
    {
        printf("station:%s left research failed\n",station);
        return 0;
    }
    else 
    {
        printf("station:%s left research success\n",station);
        ret = right_traffic_situation(map, station);
        if (ret == 0) 
        {
            printf("station:%s right research failed\n",station);
            return 0;
        }
        else 
        {
            printf("station:%s right research success\n",station);
        } 

    }
}

int fix_point(struct tiny_map *map, char *point, int status)
{
    struct list_head *pos = NULL;
    list_for_each( pos, &map->point_list_head)
    {
          struct tiny_point *pdata = container_of(pos, struct tiny_point, list);
          //find point in established point list
          if(strcmp(pdata->point_name, point) == 0)
          {
              pdata->status = status; 
              printf("\nset point : %s status to %d\n", pdata->point_name, status);
              break;
          }
    }
    return 0;
}
int print_for_each(struct tiny_map* map, void (*todo)(struct list_head *node), char list_type)
{
    struct list_head*  head;
    switch(list_type)
    {
        case 1:
        { 
            head = &map->path_list_head;
            break;
        }
        case 2:
        {
            head = &map->station_list_head;
            break;
        }
        case 3:
        {
            head = &map->point_list_head;
            break;
        }

    }
    //printf("1addr:pre %p next %p pos %p\n", head->prev, head->next, head);
    
    if(list_empty(head)){
        printf("the list is empty\n");
        return -1;
    }
    else{
        struct list_head *pos = NULL;
        list_for_each(pos, head)
        { 
          todo(pos);
          //printf("2addr:pre %p next %p pos %p\n", pos->prev,pos->next, pos);
        } 
        return 0;
    }
}


int point_add(struct tiny_map* map, struct tiny_point* point)
{
   struct tiny_point* point_new = (struct tiny_point*)malloc(sizeof(struct tiny_point));
   memcpy(point_new, point, sizeof(struct tiny_point));
   list_add_tail(&point_new->list, &map->point_list_head);
}

void point_destory(struct tiny_map *map)
{
    struct tiny_point  *point;

    if(list_empty(&map->point_list_head)){
        printf("the point list  is empty, no need destory\n");
    }
    else{
        struct list_head *pos = NULL;
        struct list_head *n = NULL;
        list_for_each_safe(pos, n, &map->point_list_head)
        {
            point = container_of(pos, struct tiny_point, list);
            printf("free point: %s\n",point->point_name);
            free(point);
        }
    }
    INIT_LIST_HEAD(&map->point_list_head);

}

int station_add(struct tiny_map* map, struct tiny_station* station)
{
   struct tiny_station* station_new = (struct tiny_station*)malloc(sizeof(struct tiny_station));
   memcpy(station_new, station, sizeof(struct tiny_station));
   INIT_LIST_HEAD(&station_new->as_source_head);
   INIT_LIST_HEAD(&station_new->as_dist_head);
   list_add_tail(&station_new->list, &map->station_list_head);
}

void station_destory(struct tiny_map *map)
{
    struct tiny_station  *station;

    if(list_empty(&map->station_list_head)){
        printf("the station list  is empty, no need destory\n");
    }
    else{
        struct list_head *pos = NULL;
        struct list_head *n = NULL;
        list_for_each_safe(pos, n, &map->station_list_head)
        {
            station = container_of(pos, struct tiny_station, list);
            printf("free station: %s\n",station->station_name);
            free(station);
        }
    }
   INIT_LIST_HEAD( &map->station_list_head);
}


int path_add(struct tiny_map* map, struct tiny_path* path)
{  
   struct list_head *pos = NULL;
   struct list_head *n = NULL;
   struct tiny_point *pdata;
   struct tiny_station *pdata1;
   struct tiny_path* path_new = (struct tiny_path*)malloc(sizeof(struct tiny_path));
   memcpy(path_new, path, sizeof(struct tiny_path));
   INIT_LIST_HEAD(&path_new->pointof_path_head);
   
   //splite sub string by token +   
   char path_name[100] = {0};
   memcpy(path_name, path_new->path_name, strlen(path_new->path_name));
   //printf("\npath_name %s------------\n",path_name);
   char *index = strtok(path_name, "+");
   char *src_station = index;
   char *dist_station;
   while(index != NULL)
   {
      //printf("%s  ", index);
      dist_station = index;
      list_for_each(pos, &map->point_list_head)
      {
          pdata = container_of(pos, struct tiny_point, list);
          //find point in established point list
          if(strcmp(pdata->point_name, index) == 0)
          {
              //insert point node into path's list head  memeber 
              list_add_tail(&pdata->list_for_path, &path_new->pointof_path_head);
              //printf("\nfind point : %s, add it into path %s list head\n", index, path_new->path_name);
              break;
          }
          else
          {
              //printf("\ncant find point : %s, maybe it isnt a point\n", index);
	  } 
      }
      
      index = strtok(NULL, "+");
   }
   
   // find src and dist station from established list for cur adding path
   list_for_each(pos, &map->station_list_head)
   {
          pdata1 = container_of(pos, struct tiny_station, list);
          
          if(strcmp(pdata1->station_name, src_station) == 0)
          {
              //path mark its src station
              path_new->src = pdata1;
              //station's  as_source_head  remember path's list_for_src_station 
              list_add_tail(&path_new->list_for_src_station, &pdata1->as_source_head);
              //printf("\nfind src station : %s,  path %s mark it\n", src_station, path_new->path_name);
          }
          if(strcmp(pdata1->station_name, dist_station) == 0)
          {
              //path mark its dist station
              path_new->dist = pdata1;
              //station's  as_dist_head  remember path's  list_for_dist_station 
              list_add_tail(&path_new->list_for_dist_station, &pdata1->as_dist_head);
              //printf("\nfind dist station : %s, path %s mark it\n", dist_station, path_new->path_name);
          }
    }

    //insert new path into map list head
    list_add_tail(&path_new->list, &map->path_list_head);
}

void path_destory(struct tiny_map *map)
{
    struct tiny_path  *path;

    if(list_empty(&map->path_list_head)){
        printf("the path list  is empty, no need destory\n");
    }
    else{
        struct list_head *pos = NULL;
        struct list_head *n = NULL;
        list_for_each_safe(pos, n, &map->path_list_head)
        {
            path = container_of(pos, struct tiny_path, list);
            printf("free station: %s\n",path->path_name);
            free(path);
        }
    }
   INIT_LIST_HEAD( &map->path_list_head);
}

    
