/*************************************************************************/
/* Copyright (C) DataMining Experiment-USTC, 2012                        */
/*                                                                       */
/*  File Name              :  KahanSum.cpp                               */
/*  Pricipal Author        :  qinpeixi(SA12226212)                       */
/*  Subsystem Name         :  Exp 1                                      */
/*  Module Name            :  KahanSum                                   */
/*  Language               :  C++                                        */
/*  Target Environment     :  Any                                        */
/*  Created Time           :  Thu 20 Dec 2012 12:44:34 PM CST            */
/*  Description            :  Implementaion of KahanSum                  */
/*************************************************************************/

#include<iostream>
#include"KahanSum.hpp"
using namespace std;


void KahanSum::reset()
{
    sum = 0.0;
    c = 0.0;
}

void KahanSum::add(double v)
{
    y = v - c;
    t = sum + y;
    c = (t - sum) - y;
    sum = t;
}

double KahanSum::get_sum()
{
    return sum;
}
