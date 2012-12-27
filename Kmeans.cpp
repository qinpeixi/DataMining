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
#include<cfloat>
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
    /* kmeans initialization */
    /*for (int i=0; i<points.size(); i++)
    {
        t = rand() % k;
        point_ids.push_back(t);
    }
    update_centers();*/

    /* Kmeans++ initialization */
    point_ids.resize(points.size());
    for (int i=1; i<k; i++)
    {
        int j = 0;
        double mindcx[points.size()];
        // for each point j, get mindcx[j], which is the distance to 
        //  the nearest cluster 
        for (list<Vector>::const_iterator index = points.begin();
                index != points.end(); index++, j++)
        {
            mindcx[j] = DBL_MAX;
            for (int ii=0; ii<i; ii++)
            {
                Vector tmp = *index; 
                tmp.sub(cluster_centers[ii]);
                double d = tmp.norm();
                if (mindcx[j] > d)
                    mindcx[j] = d;
            }
        }

        // compute the probability for each point j
        // to be picked as cluster_center[i]
        double sum = 0;
        for (j=0; j<points.size(); j++)
            sum += mindcx[j];
        double p[points.size()];
        for (j=0; j<points.size(); j++)
            p[j] = mindcx[j] / sum;

        // choose cluster_center[i] by probability 
        double prob = (double)rand() / RAND_MAX;
        for (j=0; prob >0; j++)
            prob -= p[j];
        list<Vector>::const_iterator index = points.begin();
        while (j != 0)
            j --, index ++;
        cluster_centers[i] = *index;
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
        for (list<Vector>::const_iterator j = points.begin();
                j != points.end(); j++, n++)
        {
            if (point_ids[n] == i)
                l.push_back(*j);
        }
        /* if cluster is empty, randomly choose a point as it's center */
        if (l.size() == 0)
        {
            int no = rand() % points.size();
            list<Vector>::const_iterator j = points.begin();
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

    do
    {
        int ii = 0;
        for (list<Vector>::const_iterator i = points.begin();
                i != points.end(); i++, ii++)
            point_ids[ii] = get_closest_center_id(*i);
    } while (update_centers());
    
}

double Kmeans::DunnIndex()
{
    double dcc[k][k];
    double min_dcc = DBL_MAX;
    double m[k];
    double max_m = DBL_MIN;

    /* compute the distance between each pair of cluster_centers */
    for (int i=0; i<k; i++)
    {
        for (int j=i+1; j<k; j++)
        {
            Vector tmp(cluster_centers[i]);
            tmp.sub(cluster_centers[j]);
            dcc[i][j] = tmp.norm();
            if (min_dcc > dcc[i][j])
                min_dcc = dcc[i][j];
        }
    }

    /* compute the maximum distance of members of same cluster */
    for (int i=0; i<k; i++)
        m[i] = DBL_MIN;
    int ii = 0;
    for (list<Vector>::const_iterator i = points.begin();
            i != points.end(); i++, ii++)
    {
        int jj = ii;
        for (list<Vector>::const_iterator j = i;
                j != points.end(); j++, jj++)
        {
            if (point_ids[ii] != point_ids[jj])
                continue;
            Vector tmp(*i);
            tmp.sub(*j);
            double d = tmp.norm();
            if (d > m[point_ids[ii]])
                m[point_ids[ii]] = d;
            if (d > max_m)
                max_m = d;
        }
    }

    return min_dcc / max_m;
}

double Kmeans::DaviesBouldinIndex()
{
    double m[k];
    for (int i=0; i<k; i++)
    {
        int cnt = 0;
        double sum = 0;
        int jj = 0;
        for (list<Vector>::const_iterator j = points.begin();
                j != points.end(); j++, jj++)
        {
            if (i != point_ids[jj])
                continue;
            Vector tmp(*j);
            tmp.sub(cluster_centers[i]);
            sum += tmp.norm();
            cnt ++;
        }

        m[i] = sum / cnt;
    }

    double dcc[k][k];
    double ratio[k][k];
    double sum = 0;
    /* compute the distance between each pair of cluster_centers */
    for (int i=0; i<k; i++)
    {
        for (int j=i+1; j<k; j++)
        {
            Vector tmp(cluster_centers[i]);
            tmp.sub(cluster_centers[j]);
            dcc[i][j] = dcc[j][i] = tmp.norm();
        }
        double max = DBL_MIN;
        for (int j=0; j<k; j++)
        {
            if (i == j)
                continue;
            ratio[i][j] = (m[i] + m[j]) / dcc[i][j];
            if (max < ratio[i][j])
                max = ratio[i][j];
        }
        sum += max;
    }

    return sum / k;
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
