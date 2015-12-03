//
//  streetmap_all.h
//  Playground
//
//  Created by 曹景辰 on 15/12/3.
//  Copyright © 2015年 曹景辰. All rights reserved.
//

#include "streetmap_all.h"

const char *source = "/Users/caojingchen/proj/cpp/Playground/Playground/map.xml";

pugi::xml_document shmap;
double minlat,minlon,maxlat,maxlon;

typedef std::pair<uint64_t,uint64_t> point;
std::map<uint64_t,point> points;

char atom_window[] = "Drawing a simple map";
cv::Mat atom_image = cv::Mat::zeros(w, w, CV_8UC3);

void get_bounds(pugi::xml_node *now)
{
    minlat = now->attribute("minlat").as_double();
    minlon = now->attribute("minlon").as_double();
    maxlat = now->attribute("maxlat").as_double();
    maxlon = now->attribute("maxlon").as_double();
}

void get_node(pugi::xml_node *now)
{
    double templat,templon;
    uint64_t temp1,temp2,id;
    point temp_point;
    
    id = now->attribute("id").as_uint();
    templat = now->attribute("lat").as_double();
    templon = now->attribute("lon").as_double();
    temp1 = (templat - minlat) * 10000000;
    temp2 = (templon - minlon) * 10000000;
    temp_point = std::make_pair(temp1, temp2);
    points[id] = temp_point;
    
    //printf("%.7lf %.7lf",templat,templon);
    //getchar();
}

void get_way(pugi::xml_node *now)
{
    for (pugi::xml_node temp = now->first_child(); temp != now->last_child(); temp = temp.next_sibling())
    {
        if (strcmp(temp.name(),"nd") == 0)
        {
            printf("%u\n",temp.attribute("ref").as_uint());
        }
    }
}

void debug()
{
    uint32_t x;
    while (scanf("%d",&x), x != 0)
        printf("%llu\n",points[x].first);
}

int main()
{
    pugi::xml_parse_result result = shmap.load_file(source);
    
    if (!result)
    {
        printf("Something wrong with input!\n");
        assert(0);
    }
    
    for (pugi::xml_node now = shmap.child("osm").first_child(); now != shmap.child("osm").last_child(); now = now.next_sibling())
    {
        if (strcmp(now.name(),"bounds") == 0)
        {
            get_bounds(&now);
        }
        else if (strcmp(now.name(),"node") == 0)
        {
            get_node(&now);
        }
        else if (strcmp(now.name(),"way") == 0)
        {
            get_way(&now);
        }
    }
    
    //debug();
    
    
    return 0;
}