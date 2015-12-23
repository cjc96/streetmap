//
//  init.cpp
//  Playground
//
//  Created by 曹景辰 on 15/12/16.
//  Copyright © 2015年 曹景辰. All rights reserved.
//

#include "init.hpp"

bool cmp_points_name(std::pair<std::string, pugi::xml_node> x,std::pair<std::string, pugi::xml_node> y)
{
    return x.first < y.first;
}

bool cmp1(points_info x, points_info y)
{
    if (x.loc.first < y.loc.first)
        return 1;
    else if (x.loc.first > y.loc.first)
        return 0;
    else if (x.loc.second < y.loc.second)
        return 1;
    else
        return 0;
}

bool cmp2(points_info x, points_info y)
{
    if (x.loc.second < y.loc.second)
        return 1;
    else if (x.loc.second > y.loc.second)
        return 0;
    else if (x.loc.first < y.loc.first)
        return 1;
    else
        return 0;
}

void make_kdtree(points_info *start, points_info *end, int temp, int points_time)
{
    if (temp > 1)
    {
        if (points_time)
            std::nth_element(start, start + temp / 2, end, cmp1);
        else
            std::nth_element(start, start + temp / 2, end, cmp2);
        make_kdtree(start, start + temp / 2, temp / 2, 1 - points_time);
        make_kdtree(start + temp/2, end, temp - temp / 2, 1 - points_time);
    }
}

void draw_line(cv::Mat img, cv::Point start, cv::Point end, cv::Scalar colour, int thickness)
{
    int lineType = 8;
    cv::line(img,start,end,colour,thickness,lineType);
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
    points_map[id] = points_num;
    points[points_num].node = now;
    points_origin[points_num] = temp_point;
    points[points_num++].loc = temp_point;
    
    pugi::xml_node temp_last = now->last_child();
    for (pugi::xml_node temp = now->first_child(); temp != temp_last; temp = temp.next_sibling())
    {
        if (strcmp(temp.name(),"tag") == 0)
        {
            std::string temp_string = temp.attribute("k").as_string();
            
            if (temp_string == "name" || temp_string == "name:en")
            {
                std::string temp_put_into_vector = temp.attribute("v").as_string();
                
                for (int i = 0; i < temp_put_into_vector.length(); i++)
                {
                    std::pair<std::string, pugi::xml_node > points_name_temp;
                    
                    points_name_temp = std::make_pair((&temp_put_into_vector[0]) + i, *now);
                    points_name.push_back( points_name_temp );
                }
            }
        }
    }
}

void get_way(pugi::xml_node *now)
{
    point joint1,joint2;
    
    pugi::xml_node temp_last = now->last_child();
    uint32_t s1 = -1, s2;
    
    for (pugi::xml_node temp = now->first_child(); temp != temp_last; temp = temp.next_sibling())
    {
        if (strcmp(temp.name(),"nd") == 0)
        {
            uint32_t temp_points_loc = points_map[temp.attribute("ref").as_uint()];
            
            points[temp_points_loc].way.push_back(temp);
            
            if (temp == now->first_child())
            {
                joint1 = points[temp_points_loc].loc;
                s1 = temp_points_loc;
                continue;
            }
            else
            {
                joint2 = points[temp_points_loc].loc;
                s2 = temp_points_loc;
                
                double temp_distance = std::sqrt((joint2.first - joint1.first) * (joint2.first - joint1.first) + (joint2.second - joint1.second) * (joint2.second - joint1.second));
                road_infoa[s1].push_back(temp_distance);
                road_infob[s1].push_back(s2);
                road_infoa[s2].push_back(temp_distance);
                road_infob[s2].push_back(s1);
                
                draw_line(map_image, cv::Point(joint1.first,joint1.second), cv::Point(joint2.first,joint2.second), RED, 4);
                joint1 = joint2;
                s1 = s2;
            }
            
        }
        else if (strcmp(temp.name(),"tag") == 0)
        {
            std::string temp_string = temp.attribute("k").as_string();
            
            if (temp_string == "name" || temp_string == "name:en")
            {
                std::string temp_put_into_vector = temp.attribute("v").as_string();
                
                for (int i = 0; i < temp_put_into_vector.length(); i++)
                {
                    std::pair<std::string, pugi::xml_node > points_name_temp;
                    
                    points_name_temp = std::make_pair((&temp_put_into_vector[0]) + i, *now);
                    points_name.push_back(points_name_temp);
                }
            }
        }
    }
}

void redraw(pugi::xml_node *now)
{
    point joint1,joint2;
    
    pugi::xml_node temp_last = now->last_child();
    
    for (pugi::xml_node temp = now->first_child(); temp != temp_last; temp = temp.next_sibling())
    {
        if (strcmp(temp.name(),"nd") == 0)
        {
            uint32_t temp_points_loc = points_map[temp.attribute("ref").as_uint()];
            
            if (temp == now->first_child())
            {
                joint1 = points[temp_points_loc].loc;
                continue;
            }
            else
            {
                joint2 = points[temp_points_loc].loc;
                draw_line(map_image, cv::Point(joint1.first,joint1.second), cv::Point(joint2.first,joint2.second), WHITE, 2);
                joint1 = joint2;
            }
            
        }
    }
}

void output()
{
    cv::Mat show_image;
    //rotate90(map_image);
    cv::flip(map_image, show_image, 0);
    cv::imwrite(path, show_image);
    //system("open /Users/caojingchen/proj/cpp/streetmap/Playground/map.jpeg");
    //cv::imshow( map_window, map_image );
    //cv::waitKey();
}

void init()
{
    //time_t time = std::time(NULL);
    clock_t time = clock();
    
    memset(points, 0, sizeof(points));
    memset(points_origin, 0, sizeof(points_origin));
    points_name.clear();
    points_map.clear();
    points_num = 0;
    
    pugi::xml_parse_result result = shmap.load_file(source);
    
    if (!result)
    {
        printf("Something wrong with input!\n");
        assert(0);
    }
    
    pugi::xml_node now = shmap.child("osm");
    pugi::xml_node lastpoint = now.last_child();
    pugi::xml_node redraw_way_start, redraw_way_end;
    bool xml_flag = 0;
    for (now = now.first_child(); now != lastpoint; now = now.next_sibling())
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
            if (!xml_flag)
            {
                redraw_way_start = now;
                xml_flag = 1;
            }
            get_way(&now);
            redraw_way_end = now;
        }
    }
    for (now = redraw_way_start; xml_flag; now = now.next_sibling())
    {
        if (now == redraw_way_end)
            xml_flag = 0;
        redraw(&now);
    }
    printf("%f\n",(float)(clock() - time)/CLOCKS_PER_SEC );
    
    time = clock();
    //make_kdtree(points, points + points_num, points_num, 1);
    //printf("%f\n",(float)(clock() - time)/CLOCKS_PER_SEC );
    
    time = clock();
    std::sort(points_name.begin(), points_name.end(), cmp_points_name);
    printf("%f\n",(float)(clock() - time)/CLOCKS_PER_SEC );

}
