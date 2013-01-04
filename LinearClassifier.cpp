/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              :  LinearClassifier.cpp                       */
/*  Pricipal Author        :  qinpxi                                     */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Thu 03 Jan 2013 08:16:56 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#include<iostream>
#include<cassert>
#include<cmath>
#include<random>
#include"LinearClassifier.hpp"
using namespace std;

void LinearClassifier::set_weights(const Vector &C, const Vector &N)
{
    V = N;
    vn = - V.dot(C);
    assert(V == N);
}

double LinearClassifier::signed_dist(const Vector &X) const
{
    return V.dot(X) + vn;
}

bool LinearClassifier::classify(const Vector &X) const
{
    return signed_dist(X) > 0;
}

void LinearClassifier::update_weights(const Vector &X, bool inSide)
{
    cout << "Update point:" << X.get(0) << " " << X.get(1) << endl;

    double tanh_dist = tanh(signed_dist(X));
    double derv_f = 1 - tanh_dist * tanh_dist;
    double e =  (2 * inSide - 1) - tanh_dist;
    //double e = inSide - tanh_dist;
    double coeff = eta * (e) * derv_f;
    printf("%f, %f, %f\n", signed_dist(X), e, coeff);
    Vector tmp(X);
    tmp.mul(coeff);
    V.add(tmp);
    V.normalize();
    vn += coeff;

    cout << "Complete update:" << V.get(0) << " " << V.get(1) << " " << vn << endl << endl;;
}

void LinearClassifier::reset(Vector &_C, size_t n, double _eta)
{
    Vector C(_C);
    Vector N(n);

    cout << C.get(0) << " " << C.get(1) << endl;;

    double rand_data[n];
    double data2[n];
    Box_Muller(rand_data, n);

    for (int i=0; i<n; i++)
        N.set(i, rand_data[i]);
    N.normalize();
    cout << "N is " << N.get(0) << " " << N.get(1) << endl;;

    set_weights(C, N);
    eta = _eta;
}

void Box_Muller(double rand_data[], size_t n)
{
    default_random_engine generator;
    normal_distribution<double> distribution(0, 1);

    for (int i=0; i<n; i++)
    {
        rand_data[i] = distribution(generator);
    }
}
