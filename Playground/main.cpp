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

//points_info points[510000];
point points_origin[510000];
std::vector<std::pair<std::string, pugi::xml_node> > points_name;
std::map<uint32_t,uint32_t> points_map;
int points_num;

std::vector<double> road_infoa[510000];
std::vector<uint32_t> road_infob[510000];

qtreenode qtree;
qtreenode qtree_point;
kdtreenode kdtree[1100];
kdpoint interest_point[1100];
int point_set[20][1100];
int kdpoint_number[20];
int sort_dimension;
std::set<kdpoint> kdans;

char map_window[] = "Draw a simple map";
int pixel_size;
double zoom_scale;
cv::Mat map_image;

const char *source = "/Users/caojingchen/proj/cpp/streetmap/Playground/shanghai_map.xml";
const char *path = "/Users/caojingchen/proj/cpp/streetmap/Playground/map.jpeg";

int init();
void output();
void find_name(char *);
void find_way(uint32_t, uint32_t, int);
void find_taxi(char *);
void find_one_interest(uint32_t, uint32_t);
void find_range(uint32_t, uint32_t);

int main()
{
    init();
    output();
    system("open /Users/caojingchen/proj/cpp/streetmap/Playground/map.jpeg");
    while (1)
    {
        int opcode;
        system("clear");
        printf("Please input an opcode.\n");
        printf("0 : initial\n");
        printf("1 : request a name\n");
        printf("2 : request the shortest path\n");
        printf("3 : request the kth shortest path\n");
        printf("4 : request the nearest interested points\n");
        printf("5 : request interested points within a range\n");
        printf("6 : show taxi route\n");
        printf("9 : quit\n");
        scanf("%d", &opcode);
        
        if (opcode == 0)
        {
            init();
            output();
            system("clear");
        }
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
            find_way(start, end, 0);
        }
        else if (opcode == 3)
        {
            uint32_t temp;
            uint32_t start, end;
            int kth_state;
            
            printf("Please input start point's id :");
            scanf("%u", &temp);
            start = points_map[temp];
            printf("Please input end point's id :");
            scanf("%u",&temp);
            end = points_map[temp];
            printf("Please input k :");
            scanf("%d", &kth_state);
            find_way(start, end, kth_state);
        }
        else if (opcode == 4)
        {
            uint32_t temp;
            
            printf("Please input point's id :");
            scanf("%u", &temp);
            point to_request = points_origin[points_map[temp]];
            find_one_interest(to_request.first, to_request.second);
        }
        else if (opcode == 5)
        {
            uint32_t temp;
            
            printf("Please input point's id :");
            scanf("%u", &temp);
            point to_request = points_origin[points_map[temp]];
            find_range(to_request.first, to_request.second);
        }
        else if (opcode == 6)
        {
            char taxi_label[30];
            
            printf("Please input taxi number:\n");
            scanf("%s", taxi_label);
            find_taxi(taxi_label);
            init();
        }
        else if (opcode == 9)
        {
            break;
        }
    }
    
    return 0;
}
