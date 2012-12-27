/*************************************************************************/
/* Copyright (C) DataMining Experiment -USTC, 2012                       */
/*                                                                       */
/*  File Name              :  Kmeans.cpp                                 */
/*  Pricipal Author        :  qinpxi(SA12226212)                         */
/*  Subsystem Name         :  Exp 2                                      */
/*  Module Name            :  Kmeans                                     */
/*  Language               :  C++                                        */
/*  Target Environment     :  Any                                        */
/*  Created Time           :  Fri 21 Dec 2012 07:34:29 PM CST            */
/*  Description            :  Implementation of Kmeans.                  */
/*************************************************************************/

#include<iostream>
#include<iomanip>
#include<vector>
#include<ctime>
#include<cstdlib>
#include<iterator>
#include<fstream>
#include"Vector.hpp"
#include"Kmeans.hpp"
using namespace std;

Kmeans::Kmeans(list<Vector> &p, int K)
    :points(p), k(K), cluster_centers(K, p.front().get_size())  
{
}

int Kmeans::get_closest_center_id(const Vector &X)
{
    Vector tmp(cluster_centers[0]);
    double d;
    int index = 0;

    tmp.sub(X);
    d = tmp.norm();
    for (int i=1; i < cluster_centers.size(); i++)
    {
        tmp = cluster_centers[i];
        tmp.sub(X);
        double dd = tmp.norm();
        if (d > dd)
        {
            d = dd;
            index = i;
        }
    }

    return index;
}

void Kmeans::init_point_indexes()
{
    int t;
    srand(time(0));
    for (int i=0; i<points.size(); i++)
    {
        t = rand() % k;
        point_ids.push_back(t);
    }
}

bool Kmeans::update_centers()
{
    vector<Vector> oldCenters(cluster_centers.begin(), cluster_centers.end());
    list<Vector> l;
    int n;
    srand(time(0));
    for (int i=0; i<k; i++)
    {
        n = 0;
        for (list<Vector>::iterator j = points.begin();
                j != points.end(); j++, n++)
        {
            if (point_ids[n] == i)
                l.push_back(*j);
        }
        /* if cluster is empty, randomly choose a point as it's center */
        if (l.size() == 0)
        {
            int no = rand() % points.size();
            list<Vector>::iterator j = points.begin();
            while (no != 0)
                no--, j++;
            cluster_centers[i] = *j;
        }
        else
            cluster_centers[i] = vector_mean(l); 
        l.clear();
    }

    vector<Vector>::iterator i = cluster_centers.begin();
    vector<Vector>::const_iterator j = oldCenters.begin();

    for (; i != cluster_centers.end(); i++, j++)
    {
        if (*i != *j)
            return true;
    }

    return false;
}

int Kmeans::clusterize()
{
    init_point_indexes();

    while (update_centers())
    {
        int n;
        n = 0;
        for (list<Vector>::const_iterator i = points.begin();
                i != points.end(); i++, n++)
            point_ids[n] = get_closest_center_id(*i);
    } 
    
}

void Kmeans::write_data(const char *fname)
{
    ofstream file(fname);
    int n = 0;
    
    for (list<Vector>::const_iterator i = points.begin();
            i != points.end(); i++, n++)
    {
        for (size_t j=0; j<i->get_size(); j++)
        {
            file << setprecision(18) << i->get(j) << " ";
        }
        file << get_point_id(n);
        file << endl;
    }

    file.close();
}
