/*************************************************************************/
/* Copyright (C) DataMining Experiment -USTC, 2012                       */
/*                                                                       */
/*  File Name              :  Median.cpp                                 */
/*  Pricipal Author        :  qinpeixi(SA12226212)                       */
/*  Subsystem Name         :  Exp 1                                      */
/*  Module Name            :  Median                                     */
/*  Language               :  C++                                        */
/*  Target Environment     :  Any                                        */
/*  Created Time           :  Thu 20 Dec 2012 01:35:13 PM CST            */
/*  Description            :  Implementation of Median                   */
/*************************************************************************/

#include<iostream>
#include<vector>
#include<algorithm>
#include"Median.hpp"
using namespace std;

void Median::reset()
{
    data.clear();
}

void Median::add(double value)
{
    data.push_back(value);
}

double Median::get_median()
{
    sort(data.begin(), data.end());

    if ((data.size() % 2) != 0)
    {
        int index = data.size() / 2;
        return data[index];
    }
    else
    {
        int index = data.size() / 2 - 1;
        return (data[index] + data[index + 1]) / 2;
    }
            
}


