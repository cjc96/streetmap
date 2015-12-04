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

const char *source = "/Users/caojingchen/proj/cpp/Playground/Playground/map.xml";

pugi::xml_document shmap;
double minlat,minlon,maxlat,maxlon;

typedef std::pair<uint32_t,uint32_t> point;
std::map<uint32_t,point> points;

char map_window[] = "Draw a simple map";
int pixel_size;
double zoom_scale;
cv::Mat map_image;

#endif /* streetmap_all_h */
