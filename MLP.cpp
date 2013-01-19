/*************************************************************************/
/* Copyright (C) Network Programming -USTC, 2012                         */
/*                                                                       */
/*  File Name              :  MLP.c                                      */
/*  Pricipal Author        :  qinpxi                                     */
/*  Subsystem Name         :                                             */
/*  Module Name            :                                             */
/*  Language               :                                             */
/*  Target Environment     :                                             */
/*  Created Time           :  Fri 11 Jan 2013 07:15:51 PM CST            */
/*  Description            :                                             */
/*************************************************************************/

#include<iostream>
#include<cassert>
#include<cmath>
#include<random>
#include "MLP.hpp"

MLP::MLP(int N, int K, double _eta)
{
    m_N = N;
    m_K = K;
    eta = _eta;
    V = new Vector[K];
    for (int i=0; i<K; i++)
        V[i].set_size(N);
    vn.set_size(K);
    W.set_size(K);
    Y.set_size(K);
    Yp.set_size(K);
    E.set_size(K);
}


void MLP::set_hyperplane_weights(size_t i, Vector &C, const Vector &N)
{
    C.printall("C");
    N.printall("N");
    V[i] = N;
    vn.set(i, -V[i].dot(C));
}

void MLP::forward(const Vector &X)
{
    for (int i=0; i<m_K; i++)
    {
        Y.set(i, tanh(V[i].dot(X) + vn.get(i)));
        Yp.set(i, 1 - Y.get(i) * Y.get(i));
    }
    z = tanh(W.dot(Y) + wn);
    zp = 1 - z * z;
}

void MLP::backward(const Vector &X, double c)
{
    double e = zp * (z - c);
    for (int i=0; i<m_K; i++)
        E.set(i, W.get(i) * e * Yp.get(i));
    for (int i=0; i<m_K; i++)
        W.set(i, W.get(i) + eta * Y.get(i) * e);
    wn += eta * e;
    for (int i=0; i<m_K; i++)
    {
        V[i].inc_mul(X, eta * E.get(i));
        vn.set(i, vn.get(i) + eta * E.get(i));
    }
}

bool MLP::classify(const Vector &X)
{
    forward(X);
    /* z > 0 will give opposite, very strange */
    return z < 0;
}

void MLP::update(const Vector &X, bool inClass)
{
    forward(X);
    backward(X, inClass ? 1 : -1);
}

void MLP::reset()
{
    Vector N;
    Vector C;
    double rand_data[m_N];

    N.set_size(m_N);
    C.set_size(m_N);
    for (size_t i=0; i<m_K; i++)
    {
        Box_Muller(rand_data, m_N);
        for (int j=0; j<m_N; j++)
            N.set(j, rand_data[j]);
        Box_Muller(rand_data, m_N);
        for (int j=0; j<m_N; j++)
            C.set(j, rand_data[j]);
        set_hyperplane_weights(i, C, N);
    }
    Box_Muller(rand_data, m_K);
    for (int i=0; i<m_K; i++)
        W.set(i, rand_data[i]);
    wn = 0.1;
}

void MLP::print()
{
    for (int i=0; i<m_K; i++)
        V[i].printall("V ");
    vn.printall("vn");
    W.printall("W ");
    cout << "wn: " << wn << endl;
    Y.printall("Y ");
    Yp.printall("Yp");
    cout << "z : " << z << " zp: " << zp << endl;
    E.printall("E ");
}
