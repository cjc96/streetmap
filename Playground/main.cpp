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

point points[510000];
std::map<uint32_t,uint32_t> points_map;
int points_num;

char map_window[] = "Draw a simple map";
int pixel_size;
double zoom_scale;
cv::Mat map_image;

const char *source = "/Users/caojingchen/proj/cpp/streetmap/Playground/shanghai_map.xml";
const char *path = "/Users/caojingchen/proj/cpp/streetmap/Playground/shanghai_map.jpeg";

int init();
void output();
void find_name(char *);
void find_way(point, point);

int main()
{
    init();
    output();
    
    while (1)
    {
        int opcode;
        printf("Please input an opcode.\n0 : exit\n1 : request a name\n2:request short paths\n");
        scanf("%d", &opcode);
        if (opcode == 1)
        {
            char temp[100];
            
            printf("Please input the name : ");
            scanf("%s", temp);
            find_name(temp);
        }
        else if (opcode == 2)
        {
            double temp1, temp2;
            point start, end;
            
            printf("Please input start point's location(lat, lon):");
            scanf("%lf %lf",&temp1, &temp2);
            start = std::make_pair((temp1 - minlat) * BIGINT / zoom_scale, (temp2 - minlon) * BIGINT / zoom_scale);
            printf("Please input end point's location(lat, lon):");
            scanf("%lf %lf",&temp1, &temp2);
            end = std::make_pair((temp1 - minlat) * BIGINT / zoom_scale, (temp2 - minlon) * BIGINT / zoom_scale);
            find_way(start, end);
        }
        else
        {
            break;
        }
    }
    
    return 0;
}
