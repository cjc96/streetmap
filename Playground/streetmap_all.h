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
#include <pugixml.hpp>
#include <assert.h>
#include <map>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/nonfree.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <opencv/cvaux.hpp>
#include <vector>

typedef std::pair<uint32_t,uint32_t> point;

extern pugi::xml_document shmap;
extern double minlat,minlon,maxlat,maxlon;

extern point points[510000];
extern std::map<uint32_t,uint32_t> points_map;
extern int points_num;

extern char map_window[];
extern int pixel_size;
extern double zoom_scale;
extern cv::Mat map_image;


#endif /* streetmap_all_h */
