/*************************************************************************/
/* Copyright (C) DataMining Experiment -USTC, 2012                       */
/*                                                                       */
/*  File Name              :  test.cpp                                   */
/*  Pricipal Author        :  qinpxi                                     */
/*  Subsystem Name         :  Exp 1                                      */
/*  Module Name            :  Test                                       */
/*  Language               :  C++                                        */
/*  Target Environment     :  Any                                        */
/*  Created Time           :  Thu 20 Dec 2012 01:14:45 PM CST            */
/*  Description            :  Test all of the classes and functions      */
/*************************************************************************/

#include"stdio.h"
#include<iostream>
#include<ctime>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include<list>
#include<cassert>
#include"KahanSum.hpp"
#include"Median.hpp"
#include"Vector.hpp"
#include"Kmeans.hpp"
using namespace std;

void TestKahanSum()
{
    KahanSum ks;
    double sum = 0;
    for (double v=-9.9; v<10.0; v+=0.01)
    {
        ks.add(v);
        sum += v;
    }
    cout << "Normal sum : " << sum << endl;
    cout << "KahanSum   : " << ks.get_sum() << endl;
}

void TestMedian()
{
    Median me;
    const int size = 10001;
    vector<double> t;

    srand(time(0));
    for (int i=0; i<size; i++)
    {
        double v = double(rand() % 10000) / 100.0;
        t.push_back(v);
        me.add(v);
        //cout << v << " ";
    }
    cout << endl;
    sort(t.begin(), t.end());
    //for (vector<double>::iterator i = t.begin();
     //       i != t.end(); i++)
      //  cout << *i << " ";
    cout << endl;
    cout << t[t.size()/2] << " " << t[t.size()/2+1] << endl;
    cout << me.get_median() << endl;
}

class VectorTest : public Vector
{
public:
    VectorTest(size_t n):Vector(n) {}
    void operator=(Vector &vec)
    {
        assert(n == vec.get_size());

        for (size_t i=0; i<n; i++)
            data[i] = vec.get(i);
    }
    void printall()
    {
        for (size_t i=0; i<n; i++)
            cout << data[i] << " ";
        cout << endl;
    }
};

void TestVector()
{
    const size_t size = 10;
    VectorTest a(size), b(size);

    a.fill(1.1);
    for (int i=0; i<size; i++)
    {
        b.set(i, 2.2);
        cout << b.get(i) << " ";
    }
    cout << endl;

    a.add(b); // 3.3
    a.printall();

    b.sub(a); // -1.1
    b.printall();

    a.mul(-1); // -3.3
    a.printall();

    a.inc_mul(b, -4); // 1.1
    a.printall();
    
    // 1.1 * -1.1 = -1.21
    // -1.21 * 10 = -12.1
    cout << "The dot product is " << a.dot(b) << endl;

    // -1.1 * -1.1 = 1.21
    // 1.21 * 10 = 12.1
    cout << "The squared norm is " << b.square_norm() << endl;
    cout << "The norm is " << b.norm() << endl;

    // 11
    cout << "The sum is " << a.sum() << endl; 

    a.set(0, -12.1);
    cout << "The min is " << a.min() << endl;

    a.set(size-1, 12.1);
    cout << "The max is " << a.max() << endl;

    list<Vector> l;
    l.push_back(a);
    l.push_back(b);
    write_data("data.txt", l);

    l.clear();
    read_data("data.txt", l);
    cout << "Read data:" << l.size() << endl;
    VectorTest lt(size);
    for (list<Vector>::iterator i = l.begin();
            i != l.end(); i++)
    {
        lt = *i;
        lt.printall();
    }

    Vector c(size), d(size);
    a.fill(1.1);
    b.fill(7.7);
    c.fill(3.3);
    l.clear();
    l.push_back(a);
    l.push_back(b);
    l.push_back(c);
    d = vector_mean(l);
    lt = d;
    lt.printall();
    d = vector_median(l);
    lt = d;
    lt.printall();
    Vector e(c);
    lt = e;
    cout << "E is ";
    lt.printall();
}

void TestKmeans(int n, int less, int more)
{
    const char *infile[] = {"../dm-xp-2/dataset/dataset-1",
        "../dm-xp-2/dataset/dataset-2","../dm-xp-2/dataset/dataset-3",
        "../dm-xp-2/dataset/dataset-4","../dm-xp-2/dataset/dataset-5"};
    const char *outfile[] = {"../dm-xp-2/dataset/myout-1",
        "../dm-xp-2/dataset/myout-2","../dm-xp-2/dataset/myout-3",
        "../dm-xp-2/dataset/myout-4","../dm-xp-2/dataset/myout-5"};
    const int clusters[] = {-1, 4, 4, 3, 2, 11};
    list<Vector> data;
    read_data(infile[n], data);
    for (int k=clusters[n]-less; k<clusters[n]+more; k++)
    {
        Kmeans km(data, k);
        km.clusterize();
        //cout << k << " : " << km.DunnIndex() << endl;
        //printf("%2d : %f\n", k, km.DunnIndex());
        printf("%2d : %f\n", k, km.DaviesBouldinIndex());
    }
    //km.write_data(outfile[n]);
}

int main(int argc, char *argv[])
{
    //TestKahanSum();
    //TestMedian();
    //TestVector();
    if (argc != 4)
        cout << "argument is not enough" << endl;
    else
        TestKmeans(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
}
