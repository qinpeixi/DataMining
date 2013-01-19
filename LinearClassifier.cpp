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
#define debug(...) ; //printf
using namespace std;

void LinearClassifier::set_weights(const Vector &_C, const Vector &N)
{
    V = N;
    //C = _C;
    //V.set_size(N.get_size());
    //V.othogonalize(N);
    vn = - V.dot(_C);
    //C.printall("C");
    //V.printall("V");
    //cout << "vn:" << vn << endl;
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
    debug("Update point: %f %f\n", X.get(0), X.get(1));

    double tanh_dist = tanh(signed_dist(X));
    double derv_f = 1 - tanh_dist * tanh_dist;
    double e =  (2 * inSide - 1) - tanh_dist;
    //double e = inSide - tanh_dist;
    double coeff = eta * (e) * derv_f;
    debug("%f, %f, %f\n", signed_dist(X), tanh_dist, coeff);
    V.inc_mul(X, coeff);
    //V.normalize();
    //vn += coeff * X.sum() / X.get_size();
    vn += coeff;

    //C.printall("C");
    //V.printall("V");
    debug("vn:%f\n\n", vn);
}

void LinearClassifier::reset(Vector &_C, size_t n, double _eta)
{
    //Vector C(_C);
    C = _C;
    Vector N(n);

    //cout << C.get(0) << " " << C.get(1) << endl;;

    double rand_data[n];
    double data2[n];
    Box_Muller(rand_data, n);

    for (int i=0; i<n; i++)
        N.set(i, rand_data[i]);
    //N.normalize();

    set_weights(C, N);
    eta = _eta;
    debug("C: %f %f V: %f %f vn: %f\n\n", C.get(0), C.get(1), V.get(0), V.get(1), vn);
}
