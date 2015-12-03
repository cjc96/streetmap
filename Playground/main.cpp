#include <stdint.h>
#include <stdio.h>
#include <pugixml.hpp>
#include <iostream>
#include <assert.h>
#include <map>

const char *source = "/Users/caojingchen/proj/cpp/Playground/Playground/map.xml";

pugi::xml_document shmap;
double minlat,minlon,maxlat,maxlon;

typedef std::pair<uint32_t,uint32_t> point;
std::map<uint32_t,point> points;

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
    uint32_t temp1,temp2,id;
    point temp_point;
    
    id = now->attribute("id").as_uint();
    templat = now->attribute("lax").as_double();
    templon = now->attribute("lon").as_double();
    temp1 = (templat - minlat) * 1000000;
    temp2 = (templon - minlon) * 1000000;
    temp_point = std::make_pair(temp1, temp2);
    points[id] = temp_point;
}

void get_way(pugi::xml_node *now)
{
    
}

int main()
{
    pugi::xml_parse_result result = shmap.load_file(source);
    
    if (!result)
    {
        std::cout << "Something wrong with input" << std::endl;
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
    
    
    
    return 0;
}