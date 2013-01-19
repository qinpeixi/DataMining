/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              :  MLP.h                                      */
/*  Pricipal Author        :  qinpxi                                     */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Fri 11 Jan 2013 07:15:57 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#ifndef MLP_HPP
#define MLP_HPP

#include "Vector.hpp"

class MLP
{
    int m_N;
    int m_K;
    Vector *V;
    Vector vn;
    Vector W;
    double wn;
    Vector Y;
    Vector Yp;
    double z;
    double zp;
    Vector E;
    double eta;
public:
    MLP(int N, int K, double _eta);
    void set_hyperplane_weights(size_t i, Vector &C, const Vector &N);
    void forward(const Vector &X);
    void backward(const Vector &X, double c);
    bool classify(const Vector &X);
    void update(const Vector &X, bool inClass);
    void reset();
    void print();

};

#endif

