//
//  find.cpp
//  Playground
//
//  Created by 曹景辰 on 15/12/17.
//  Copyright © 2015年 曹景辰. All rights reserved.
//

#include "find.hpp"

void output();
void draw_line(cv::Mat, cv::Point, cv::Point, cv::Scalar, int);

void map_pause()
{
    while (1)
    {
        char temp_key;
        
        printf("Continue?(y/n)\n");
        scanf("%c",&temp_key);
        if (temp_key == 'y')
            return;
        else if (temp_key == 'n')
        {
            system("clear");
            exit(0);
        }
    }
}

bool check(char *x, std::string y)
{
    for (int i = 0; i < strlen(x); i++)
        if (x[i] != y[i])
            return 0;
    return 1;
}

void find_name(char *str)
{
    pugi::xml_node useless;
    long temp_start = std::lower_bound(points_name.begin(), points_name.end(), std::make_pair(std::string(str), useless)) - points_name.begin(), temp_end = temp_start;
    
    while (check(str, points_name[temp_end].first))
        temp_end++;
    
    if (temp_start == temp_end)
    {
        printf("Nothing found.\n");
        output();
        map_pause();
    }
    else
    {
        
        for (long temp_loop = temp_start;temp_loop < temp_end; temp_loop++)
        {
            point joint1,joint2;
            
            pugi::xml_node now = points_name[temp_loop].second;
            if (strcmp(now.name(),"node") == 0)
            {
                uint32_t temp_points_loc = points_map[now.attribute("id").as_uint()];
                point temp_point = points_origin[temp_points_loc];
                cv::circle(map_image, cv::Point(temp_point.first, temp_point.second), 2, BLUE, -1);
            }
            else
            {
                pugi::xml_node temp_last = now.last_child();
                for (pugi::xml_node temp = now.first_child(); temp != temp_last; temp = temp.next_sibling())
                {
                    if (strcmp(temp.name(),"nd") == 0)
                    {
                        uint32_t temp_points_loc = points_map[temp.attribute("ref").as_uint()];
                        
                        if (temp == now.first_child())
                        {
                            joint1 = points_origin[temp_points_loc];
                            continue;
                        }
                        else
                        {
                            joint2 = points_origin[temp_points_loc];
                            draw_line(map_image, cv::Point(joint1.first,joint1.second), cv::Point(joint2.first,joint2.second), BLUE, 2);
                            joint1 = joint2;
                        }
                        
                    }
                }
            }
        }
        
        output();
        map_pause();
        
        for (long temp_loop = temp_start;temp_loop < temp_end; temp_loop++)
        {
            point joint1,joint2;
            pugi::xml_node now = points_name[temp_loop].second;
            if (strcmp(now.name(),"node") == 0)
            {
                uint32_t temp_points_loc = points_map[now.attribute("id").as_uint()];
                point temp_point = points_origin[temp_points_loc];
                cv::circle(map_image, cv::Point(temp_point.first, temp_point.second), 2, WHITE, -1);
            }
            else
            {
                pugi::xml_node temp_last = now.last_child();
                for (pugi::xml_node temp = now.first_child(); temp != temp_last; temp = temp.next_sibling())
                {
                    if (strcmp(temp.name(),"nd") == 0)
                    {
                        uint32_t temp_points_loc = points_map[temp.attribute("ref").as_uint()];
                        
                        if (temp == now.first_child())
                        {
                            joint1 = points_origin[temp_points_loc];
                            continue;
                        }
                        else
                        {
                            joint2 = points_origin[temp_points_loc];
                            draw_line(map_image, cv::Point(joint1.first,joint1.second), cv::Point(joint2.first,joint2.second), WHITE, 2);
                            joint1 = joint2;
                        }
                        
                    }
                }
            }
        }
        
    }
}

void output_shortest_way(uint32_t *former_ans, uint32_t s, uint32_t end)
{
    point joint1, joint2;
    joint1 = points_origin[end];
    uint32_t now = end;
    while (now != s)
    {
        joint2 =points_origin[former_ans[now]];
        draw_line(map_image, cv::Point(joint1.first,joint1.second), cv::Point(joint2.first, joint2.second), BLUE, 2);
        joint1 = joint2;
        now = former_ans[now];
    }
    output();
    joint1 = points_origin[end];
    now = end;
    while (now != s)
    {
        joint2 =points_origin[former_ans[now]];
        draw_line(map_image, cv::Point(joint1.first,joint1.second), cv::Point(joint2.first, joint2.second), WHITE, 2);
        joint1 = joint2;
        now = former_ans[now];
    }
}

void spfa(uint32_t s, uint32_t end, double *dist, time_t *running_time)
{
    uint32_t now;
    std::queue<int> d;
    static bool v[510000];
    static uint32_t former_ans[510000];
    
    memset(v, 0, sizeof(v));
    memset(former_ans, 0, sizeof(former_ans));
    dist[s] = 0; v[s] = 1;
    d.push(s);
    
    while (!d.empty())
    {
        now = d.front();
        d.pop();
        
        if (dist[now] <= dist[end])
            for (int i = 0; i < road_infob[now].size(); i++)
                if (dist[road_infob[now][i]] > dist[now] + road_infoa[now][i])
                {
                    dist[road_infob[now][i]] = dist[now] + road_infoa[now][i];
                    former_ans[road_infob[now][i]] = now;
                    if (!v[road_infob[now][i]])
                    {
                        d.push(road_infob[now][i]);
                        v[road_infob[now][i]] = 1;
                    }
                }
        v[now] = 0;
    }
    
    *running_time = clock();
    
    output_shortest_way(former_ans, s, end);
}

bool cmp_dijkstra(std::pair<double, std::pair<uint32_t, uint32_t> > x, std::pair<double, std::pair<uint32_t, uint32_t> > y)
{
    return x.first > y.first;
}

void dijkstra(uint32_t s,uint32_t end, double *dist, time_t *running_time)
{
    static bool v[510000];
    static std::pair<double, std::pair<uint32_t, uint32_t> > d[510000]; // heap
    static uint32_t former_ans[510000];
    int d_len = 0, now;
    
    memset(v, 0, sizeof(v));
    memset(d, 0, sizeof(d));
    memset(former_ans, 0, sizeof(former_ans));
    for (int i = 0; i < road_infob[s].size(); i++)
        d[d_len++] = std::make_pair(road_infoa[s][i], std::make_pair(s, road_infob[s][i]));
    make_heap(d, d + d_len, cmp_dijkstra);
    v[s] = 1;
    
    while (!v[end])
    {
        std::pair<double, std::pair<uint32_t, uint32_t> > temp_e = d[0];
        pop_heap(d, d + d_len--, cmp_dijkstra);
        now = temp_e.second.second;
        if (v[now])
            continue;
        v[now] = 1;
        dist[now] = temp_e.first;
        former_ans[now] = temp_e.second.first;
        for (int i = 0; i < road_infob[now].size(); i++)
        {
            d[d_len++] = std::make_pair(road_infoa[now][i] + dist[now],std:: make_pair(now, road_infob[now][i]));
            push_heap(d, d + d_len, cmp_dijkstra);
        }
    }
    
    *running_time = clock();
    
    output_shortest_way(former_ans, s, end);
}

bool cmp_astar(std::pair<double, uint32_t> x, std::pair<double, uint32_t> y)
{
    return x.first > y.first;
}

double asses(uint32_t x, uint32_t y)
{
    //return 0;
    point temp_x = points_origin[x], temp_y = points_origin[y];
    return std::sqrt((temp_x.first - temp_y.first) * (temp_x.first - temp_y.first) + (temp_x.second - temp_y.second) * (temp_x.second - temp_y.second));
}

void astar(uint32_t s, uint32_t end, double g[], time_t *running_time, bool dijk)
{
    static short open_set[510000], closed_set[510000];
    static std::pair<double, uint32_t> d[510000]; // heap
    static uint32_t former_ans[510000];
    static double h[510000],f[510000];
    int open_len = points_num, closed_len = 0, d_len = 0;
    
    memset(open_set, 0, sizeof(open_set));
    memset(former_ans, 0, sizeof(former_ans));
    memset(closed_set, 0, sizeof(closed_set));
    memset(d, 0, sizeof(d));
    memset(h, 0, sizeof(h));
    memset(f, 0, sizeof(f));
    g[s] = 0;
    h[s] = dijk ? 0 : asses(s,end);
    f[s] = h[s];
    d[d_len++] = std::make_pair(f[s], s);
    while (open_len != 0)
    {
        std::pair<double, uint32_t> temp = d[0];
        uint32_t now = temp.second;
        pop_heap(d, d + d_len--, cmp_astar);
        
        if (now == end)
        {
            *running_time = clock();
            output_shortest_way(former_ans, s, end);
            return;
        }
        
        if (!open_set[now])
        {
            open_set[now] = 1;
            open_len--;
        }
        //else
            //continue;
        if (!closed_set[now])
        {
            closed_set[now] = 1;
            closed_len++;
        }
        //else
            //continue;
        
        for (int i = 0; i < road_infob[now].size(); i++)
        {
            int y = road_infob[now][i];
            bool tentative_is_better;
            
            if (closed_set[y])
                continue;
            double tentative_g = g[now] + road_infoa[now][i];
            if (open_set[y])
            {
                open_set[y] = 0;
                open_len++;
                tentative_is_better = 1;
            }
            else if (tentative_g < g[y])
                tentative_is_better = 1;
            else
                tentative_is_better = 0;
            
            if (tentative_is_better)
            {
                g[y] = tentative_g;
                h[y] = dijk ? 0 : asses(y,end);
                f[y] = g[y] + h[y];
                d[d_len++] = std::make_pair(f[y], y);
                push_heap(d, d + d_len, cmp_astar);
                former_ans[y] = now;
            }
        }
    }
    
    *running_time = clock();
    
    output_shortest_way(former_ans, s, end);
}

double A_Star(double *dist, uint32_t start,uint32_t end, int k)
{
    std::priority_queue<asses_node> asses_que;
    static uint32_t former_ans[510000];
    static int cnt[510000];
    
    memset(cnt, 0, sizeof(cnt));
    memset(former_ans, 0, sizeof(former_ans));
    asses_que.push(asses_node(dist[start],0,start));
    asses_node next(0,0,0);
    while(!asses_que.empty())
    {
        asses_node now = asses_que.top();
        asses_que.pop();
        cnt[now.v]++;
        if(cnt[end] == k)
            return now.f;
        if(cnt[now.v] > k)
            continue;
        for(int i = 0; i < road_infob[now.v].size(); i++)
        {
            next.v = road_infob[now.v][i];
            next.g = now.g + road_infoa[now.v][i];
            next.f = next.g + dist[next.v];
            asses_que.push(next);
        }
    }
    return -1;
}

void find_way(uint32_t start, uint32_t end, int kth_state)
{
    double dist[510000];
    time_t time_start = clock();
    time_t time_end;
    
    if (!kth_state)
    {
        for (int i = 0; i < 510000; i++)
            dist[i] = 1e+20;
        spfa(start, end, dist, &time_end);
        printf("Time consumed(SPFA) : %f\n",(float)(time_end - time_start)/CLOCKS_PER_SEC );
        printf("Total length : %lf meters\n\n",dist[end] * 111000 / zoom_scale / BIGINT);
        map_pause();
        
        for (int i = 0; i < 510000; i++)
            dist[i] = 1e+20;
        time_start = clock();
        //dijkstra(start, end, dist, &time_end);
        astar(start, end, dist, &time_end, 1);
        printf("Time consumed(Dijkstra) : %f\n",(float)(time_end - time_start)/CLOCKS_PER_SEC );
        printf("Total length : %lf meters\n\n",dist[end] * 111000 / zoom_scale / BIGINT);
        map_pause();

        for (int i = 0; i < 510000; i++)
            dist[i] = 1e+20;
        time_start = clock();
        astar(start, end, dist, &time_end, 0);
        printf("Time consumed(A*) : %f\n",(float)(time_end - time_start)/CLOCKS_PER_SEC );
        printf("Total length : %lf meters\n\n",dist[end] * 111000 / zoom_scale / BIGINT);
        map_pause();
    }
    else
    {
        for (int i = 0; i < 510000; i++)
            dist[i] = 1e+20;
        time_start = clock();
        astar(start, end, dist, &time_end, 1);
        printf("The kth shortest length is %lf\n",A_Star(dist, start, end, kth_state) * 111000 / zoom_scale / BIGINT / 2);
        time_end = clock();
        printf("Time consumed(A*) : %f\n",(float)(time_end - time_start)/CLOCKS_PER_SEC );
        map_pause();
    }

}

void find_taxi(char label[])
{
    char taxi_source[200] = "/Users/caojingchen/Fudan/My projects/cpp/streetmap/Playground/Taxi_Data/";
    char taxi_info[200];
    double taxi_dist = 0, total_dist = 0, total_income = 0;
    int pa_num = 0;
    unsigned long source_len = strlen(taxi_source);
    for (int i = 0; i <= strlen(label); i++)
        taxi_source[source_len + i] = label[i];
    
    FILE *fp = fopen(taxi_source,"r");
    point joint1, joint2;
    bool flag = 0;
    while (!feof(fp))
    {
        int bp1 = -1, bp2 = -1, bp3 = -1;
        
        fgets(taxi_info, 200, fp);

        unsigned long temp_end = strlen(taxi_info), commer = 0;
        for (int i = 0; i < temp_end; i++)
        {
            if (taxi_info[i] == ',')
            {
                commer++;
                if (commer == 3)
                    bp1 = i;
                else if (commer == 4)
                    bp2 = i;
                else if (commer == 5)
                {
                    bp3 = i;
                    break;
                }
            }
        }
        if (commer == 0)
            continue;
        std::string bp;
        double temp_lon, temp_lat;
        int temp_state, former_state;
        bp.clear();
        for (int i = bp1 + 1; i < bp2; i++)
            bp += taxi_info[i];
        bp += '\0';
        sscanf(&bp[0], "%lf", &temp_lon);
        bp.clear();
        for (int i = bp2 + 1; i < bp3; i++)
            bp += taxi_info[i];
        bp += '\0';
        sscanf(&bp[0], "%lf", &temp_lat);
        bp.clear();
        bp += taxi_info[bp3 + 1];
        bp += '\0';
        sscanf(&bp[0], "%d", &temp_state);
        
        if (!flag)
        {
            flag = 1;
            joint1.first = (temp_lon - minlon) * BIGINT * zoom_scale;
            joint1.second = (temp_lat - minlat) * BIGINT * zoom_scale;
            former_state = temp_state;
        }
        else
        {
            joint2.first = (temp_lon - minlon) * BIGINT * zoom_scale;
            joint2.second = (temp_lat - minlat) * BIGINT * zoom_scale;
            total_dist += std::sqrt((joint2.first - joint1.first) * (joint2.first - joint1.first) + (joint2.second - joint1.second) * (joint2.second - joint1.second)) * 111000 / BIGINT / zoom_scale;
            if (!former_state)
            {
                double payment;
                
                taxi_dist += std::sqrt((joint2.first - joint1.first) * (joint2.first - joint1.first) + (joint2.second - joint1.second) * (joint2.second - joint1.second)) * 111000 / BIGINT / zoom_scale;
                if (temp_state)
                {
                    if (taxi_dist < 3000)
                        payment = 14;
                    else if (taxi_dist < 15000)
                        payment = 14 + (taxi_dist - 3000) / 1000 * 2.5;
                    else
                        payment = 44 + (taxi_dist - 15000) / 1000 * 3.6;
                
                    printf("Passenger No. %d : %.2lf yuan\n", ++pa_num, payment);
                    cv::circle(map_image, cv::Point(joint2.first, joint2.second), 5, ORANGE, -1);
                    total_income += payment;
                    taxi_dist = 0;
                }
            }
            
            draw_line(map_image, cv::Point(joint1.first,joint1.second), cv::Point(joint2.first, joint2.second), temp_state ? GREEN : BLUE, 2);
            joint1 = joint2;
            former_state = temp_state;
        }
    }
    printf("Total distance : %.2lf meters\n", total_dist);
    printf("Total distance : %.2lf yuan\n", total_income);
    output();
    map_pause();
    fclose(fp);
}

void find_one_qtree(qtreenode *now, uint32_t requested_lon, uint32_t requested_lat)
{
    if (!now->leaf)
    {
        uint32_t middle_lon = (now->lower_limit.first + now->upper_limit.first) / 2, middle_lat = (now->lower_limit.second + now->upper_limit.second) / 2;
        
        if (requested_lon >= middle_lon && requested_lat >= middle_lat)
            find_one_qtree(now->son1, requested_lon, requested_lat);
        else if (requested_lon >= middle_lon && requested_lat < middle_lat)
            find_one_qtree(now->son2, requested_lon, requested_lat);
        else if (requested_lon < middle_lon && requested_lat >= middle_lat)
            find_one_qtree(now->son3, requested_lon, requested_lat);
        else
            find_one_qtree(now->son4, requested_lon, requested_lat);
    }
    /*else
    {
        point to_draw = points_origin[points_map[now->node_id]];
        cv::circle(map_image, cv::Point(to_draw.first, to_draw.second), 4, BLUE, -1);
        printf("Interested point's name : %s\n",&now->anemity_name[0]);
        output();
        map_pause();
        cv::circle(map_image, cv::Point(to_draw.first, to_draw.second), 4, WHITE, -1);
    }*/
}

void find_one_qtree_point(qtreenode *now, uint32_t requested_lon, uint32_t requested_lat)
{
    if (now->node_id)
    {
        qtreenode *temp;
        uint32_t middle_lon = points_origin[points_map[now->node_id]].first, middle_lat = points_origin[points_map[now->node_id]].second;
        
        if (middle_lat == requested_lat && middle_lon == requested_lon)
        {
            /*point to_draw = points_origin[points_map[now->node_id]];
            cv::circle(map_image, cv::Point(to_draw.first, to_draw.second), 4, BLUE, -1);
            printf("Interested point's name : %s\n",&now->anemity_name[0]);
            output();
            map_pause();
            cv::circle(map_image, cv::Point(to_draw.first, to_draw.second), 4, WHITE, -1);*/
            return;
        }
        if (requested_lon >= middle_lon && requested_lat >= middle_lat)
            temp = now->son1;
        else if (requested_lon >= middle_lon && requested_lat < middle_lat)
            temp = now->son2;
        else if (requested_lon < middle_lon && requested_lat >= middle_lat)
            temp = now->son3;
        else
            temp = now->son4;
    }
}

kdpoint search_point;
bool operator < (const kdpoint &a, const kdpoint &b)
{
    return (a.p[0] - search_point.p[0]) * (a.p[0] - search_point.p[0]) + (a.p[1] - search_point.p[1]) * (a.p[1] - search_point.p[1]) < (b.p[0] - search_point.p[0]) * (b.p[0] - search_point.p[0]) + (b.p[1] - search_point.p[1]) * (b.p[1] - search_point.p[1]);
}

void find_kdtree(int x, uint32_t request_lon, uint32_t request_lat)
{
    kdpoint temp_add_ans = interest_point[kdtree[x].index];
    kdans.insert(temp_add_ans);
    if (kdans.size() > 10)
    {
        std::set<kdpoint>::reverse_iterator it = kdans.rbegin();
        kdans.erase(*it);
    }
    
    if(!kdtree[x].left && !kdtree[x].right)
    {
        return;
    }
    
    bool search_flag = false;
    
    search_point.p[0] = request_lon; search_point.p[1] = request_lat;
    int dist1 = search_point.p[kdtree[x].dim] - interest_point[kdtree[x].index].p[kdtree[x].dim];
    dist1 *= dist1;
    if (search_point.p[kdtree[x].dim] < interest_point[kdtree[x].index].p[kdtree[x].dim])
    {
        if (kdtree[x].left)
        {
            find_kdtree(x << 1, request_lon, request_lat);
        }
        if(kdtree[x].right)
        {
            if (kdans.size() < 10)
            {
                search_flag = true;
            }
            else
            {
                std::set<kdpoint>::reverse_iterator it = kdans.rbegin();
                kdpoint temp = *it;
                int dist2 = (temp.p[0] - search_point.p[0]) * (temp.p[0] - search_point.p[0]) + (temp.p[1] - search_point.p[1]) * (temp.p[1] - search_point.p[1]);
                if (dist1 <= dist2)
                {
                    search_flag = true;
                }
            }
            if (search_flag)
            {
                find_kdtree((x << 1) + 1,request_lon, request_lat);
            }
        }
    }
    else
    {
        if(kdtree[x].right)
        {
            find_kdtree((x << 1) + 1,request_lon, request_lat);
        }
        if(kdtree[x].left)
        {
            if(kdans.size() < 10)
            {
                search_flag = true;
            }
            else
            {
                std::set<kdpoint>::reverse_iterator it = kdans.rbegin();
                kdpoint temp = *it;
                 int dist2 = (temp.p[0] - search_point.p[0]) * (temp.p[0] - search_point.p[0]) + (temp.p[1] - search_point.p[1]) * (temp.p[1] - search_point.p[1]);
                if(dist1 <= dist2)
                {
                    search_flag = true;
                }
            }
            if(search_flag)
            {
                find_kdtree(x << 1,request_lon, request_lat);
            }
        }
    }
}

void find_one_interest(uint32_t requested_lon, uint32_t requested_lat)
{
    time_t time;
    
    kdans.clear();
    time = clock();
    find_one_qtree(&qtree, requested_lon, requested_lat);
    printf("%f\n",(float)(clock() - time) / CLOCKS_PER_SEC );
    
    /*time = clock();
    find_one_qtree_point(&qtree_point, requested_lon, requeseted_lat);
    printf("%f\n",(float)(clock() - time) / CLOCKS_PER_SEC );*/
    
    time = clock();
    find_kdtree(1, requested_lon, requested_lat);
    printf("%f\n",(float)(clock() - time) / CLOCKS_PER_SEC );
    std::set<kdpoint>::iterator to_draw = kdans.begin();
    cv::circle(map_image, cv::Point(to_draw->p[0], to_draw->p[1]), 4, BLUE, -1);
    printf("Amenity type : %s\n", &(to_draw->name[0]));
    double temp_dist = std::sqrt((to_draw->p[0] - requested_lon) * (to_draw->p[0] - requested_lon) + (to_draw->p[1] - requested_lat) * (to_draw->p[1] - requested_lat)) * 111000 / BIGINT / zoom_scale;
    printf("Amenity type : %s\tdistance : %lf meters\n", &(to_draw->name[0]), temp_dist);

    output();
    cv::circle(map_image, cv::Point(to_draw->p[0], to_draw->p[1]), 4, WHITE, -1);
    map_pause();
}

void find_range_kdtree(uint32_t requested_lon, uint32_t requested_lat)
{
    time_t time = clock();
    find_kdtree(1, requested_lon, requested_lat);
    printf("%f\n",(float)(clock() - time) / CLOCKS_PER_SEC );
    for(std::set<kdpoint>::iterator to_draw = kdans.begin(); to_draw != kdans.end(); ++to_draw)
    {
        cv::circle(map_image, cv::Point(to_draw->p[0], to_draw->p[1]), 4, BLUE, -1);
        double dist = std::sqrt((to_draw->p[0] - requested_lon) * (to_draw->p[0] - requested_lon) + (to_draw->p[1] - requested_lat) * (to_draw->p[1] - requested_lat)) * 111000 / BIGINT / zoom_scale;
        printf("Amenity type : %s\t%lf meters\n", &(to_draw->name[0]), dist);
    }
    output();
    for(std::set<kdpoint>::iterator to_draw = kdans.begin(); to_draw != kdans.end(); ++to_draw)
    {
        cv::circle(map_image, cv::Point(to_draw->p[0], to_draw->p[1]), 4, WHITE, -1);
    }
    map_pause();
}

void find_range(uint32_t requested_lon, uint32_t requested_lat)
{
    kdans.clear();
    find_range_kdtree(requested_lon, requested_lat);
}