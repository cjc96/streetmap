//
//  main.cpp
//  Playground
//
//  Created by 曹景辰 on 15/12/3.
//  Copyright © 2015年 曹景辰. All rights reserved.
//

#include "streetmap_all.h"
#include "map_const.h"

void draw_line(cv::Mat img, cv::Point start, cv::Point end)
{
    int thickness = 2;
    int lineType = 8;
    cv::line(img,start,end,cv::Scalar( 255, 255, 255 ),thickness,lineType);
}

void draw_rectangle(cv::Mat img, cv::Point start, cv::Point end)
{
    int thickness = -1;
    int lineType = 8;
    cv::rectangle(img, start, end, BLANK, thickness, lineType);
}

void map_initial()
{
    map_image = cv::Mat::zeros(pixel_size, pixel_size, CV_8UC3);
    draw_rectangle(map_image, cv::Point(0,0), cv::Point(pixel_size,pixel_size));
}

void rotate90(cv::Mat img)
{
    cv::transpose(img, img);
    cv::flip(img,img,0);
}

void get_bounds(pugi::xml_node *now)
{
    minlat = now->attribute("minlat").as_double();
    minlon = now->attribute("minlon").as_double();
    maxlat = now->attribute("maxlat").as_double();
    maxlon = now->attribute("maxlon").as_double();
    pixel_size = maxlon - minlon > maxlat - minlat ? (maxlon - minlon) * BIGINT : (maxlat - minlat) * BIGINT;
    zoom_scale = (double)SIZE_ON_SCREEN / pixel_size;
    pixel_size = SIZE_ON_SCREEN;
    
    map_initial();
}

void get_node(pugi::xml_node *now)
{
    double templat,templon;
    uint32_t temp1,temp2,id;
    point temp_point;
    
    id = now->attribute("id").as_uint();
    templat = now->attribute("lat").as_double();
    templon = now->attribute("lon").as_double();
    temp2 = (templat - minlat) * BIGINT * zoom_scale;
    temp1 = (templon - minlon) * BIGINT * zoom_scale;
    temp_point = std::make_pair(temp1, temp2);
    points[id] = temp_point;
}

void get_way(pugi::xml_node *now)
{
    point joint1,joint2;
    
    for (pugi::xml_node temp = now->first_child(); temp != now->last_child(); temp = temp.next_sibling())
    {
        if (strcmp(temp.name(),"nd") == 0)
        {
            if (temp == now->first_child())
            {
                joint1 = points[temp.attribute("ref").as_uint()];
                continue;
            }
            joint2 = points[temp.attribute("ref").as_uint()];
            draw_line(map_image, cv::Point(joint1.first,joint1.second), cv::Point(joint2.first,joint2.second));
            
            joint1 = joint2;
        }
    }
}

void output()
{
    //rotate90(map_image);
    cv::flip(map_image,map_image,0);
    cv::imshow( map_window, map_image );
    //cv::moveWindow( map_window, 0, 600 );
    cv::waitKey();
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
    points.clear();
    output();
    
    
    return 0;
}