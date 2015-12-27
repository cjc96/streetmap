//
//  streetmap_all.h
//  Playground
//
//  Created by 曹景辰 on 15/12/3.
//  Copyright © 2015年 曹景辰. All rights reserved.
//

#ifndef streetmap_all_h
#define streetmap_all_h

#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <map>
#include <set>
#include <string>
#include <queue>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include <pugixml.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv/cvaux.hpp>

typedef std::pair<uint32_t, uint32_t> point;

struct kdtreenode
{
    int dim, index;
    bool left, right;
};

struct asses_node
{
    double f, g;
    uint32_t v;
    asses_node(int a, int b,int c):f(a),g(b),v(c){}
    bool operator < (const asses_node& a) const
    {
        return a.f < f;
    }
};

typedef struct
{
    point loc;
    std::vector<pugi::xml_node> way;
    pugi::xml_node *node;
} points_info;

struct qtreenode
{
    bool leaf;
    uint32_t node_id;
    std::string anemity_name;
    point upper_limit, lower_limit;
    qtreenode *son1, *son2, *son3, *son4;
};

struct kdpoint
{
    uint32_t p[2];
    std::string name;
};

extern pugi::xml_document shmap;
extern double minlat,minlon,maxlat,maxlon;

extern points_info points[510000];
extern point points_origin[510000];
extern std::vector<std::pair<std::string, pugi::xml_node> > points_name;
extern std::map<uint32_t,uint32_t> points_map;
extern int points_num;
extern std::vector<double> road_infoa[510000];
extern std::vector<uint32_t> road_infob[510000];

extern char map_window[];
extern int pixel_size;
extern double zoom_scale;
extern cv::Mat map_image;

extern qtreenode qtree;
extern qtreenode qtree_point;
extern kdtreenode kdtree[1100];
extern kdpoint interest_point[1100];
extern int kdpoint_number[20];
extern int point_set[20][1100];
extern int sort_dimension;
extern std::set<kdpoint> kdans;

#endif /* streetmap_all_h */
