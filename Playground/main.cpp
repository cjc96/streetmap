//
//  main.cpp
//  Playground
//
//  Created by 曹景辰 on 15/12/3.
//  Copyright © 2015年 曹景辰. All rights reserved.
//


#include "streetmap_all.h"
#include "map_const.h"

pugi::xml_document shmap;
double minlat,minlon,maxlat,maxlon;

points_info points[510000];
point points_origin[510000];
std::vector<std::pair<std::string, pugi::xml_node> > points_name;
std::map<uint32_t,uint32_t> points_map;
int points_num;
std::vector<double> road_infoa[510000];
std::vector<uint32_t> road_infob[510000];

char map_window[] = "Draw a simple map";
int pixel_size;
double zoom_scale;
cv::Mat map_image;

const char *source = "/Users/caojingchen/proj/cpp/streetmap/Playground/shanghai_map.xml";
const char *path = "/Users/caojingchen/proj/cpp/streetmap/Playground/map.jpeg";

int init();
void output();
void find_name(char *);
void find_way(uint32_t, uint32_t);
void find_taxi(char *);

int main()
{
    init();
    output();
    
    while (1)
    {
        int opcode;
        printf("Please input an opcode.\n");
        printf("0 : initial\n");
        printf("1 : request a name\n");
        printf("2 : request the shortest path\n");
        printf("3 : request kth nearest points\n");
        printf("4 : request a range\n");
        printf("5 : show taxi route\n");
        printf("others : quit\n");
        scanf("%d", &opcode);
        
        if (opcode == 0)
            init();
        else if (opcode == 1)
        {
            char temp[100];
            
            printf("Please input the name : ");
            scanf("%s", temp);
            find_name(temp);
        }
        else if (opcode == 2)
        {
            uint32_t temp;
            uint32_t start, end;
            
            printf("Please input start point's id:");
            scanf("%d", &temp);
            start = points_map[temp];
            printf("Please input end point's id:");
            scanf("%d",&temp);
            end = points_map[temp];
            find_way(start, end);
        }
        else if (opcode == 3)
        {
            
        }
        else if (opcode == 4)
        {
            
        }
        else if (opcode == 5)
        {
            char taxi_label[30];
            
            printf("Please input taxi number:\n");
            scanf("%s", taxi_label);
            find_taxi(taxi_label);
            init();
        }
        else
        {
            break;
        }
    }
    
    return 0;
}
