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
#include<fstream>
#include<iomanip>
#include<vector>
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
#include"LinearClassifier.hpp"
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

void TestKmeans(int n, int lowerbound, int upperbound)
{
    const char *infile[] = {"dataset/dataset-1",
        "dataset/dataset-2","dataset/dataset-3",
        "dataset/dataset-4","dataset/dataset-5"};
    const char *outfile[] = {"dataset/myout-1",
        "dataset/myout-2","dataset/myout-3",
        "dataset/myout-4","dataset/myout-5"};
    //const int clusters[] = {-1, 4, 4, 3, 2, 11};
    list<Vector> data;
    read_data(infile[n], data);
    for (int k=lowerbound; k<=upperbound; k++)
    {
        Kmeans km(data, k);
        km.clusterize();
        //cout << k << " : " << km.DunnIndex() << endl;
        //printf("%2d : %f\n", k, km.DunnIndex());
        km.write_data("myout-3");
        printf("Cluster: %2d  DunnIndex: %f  Davies-BouldinIndex: %f\n", k, km.DunnIndex(), km.DaviesBouldinIndex());
    }
}

void read_data(const char *fname, vector<Vector> &data, vector<bool> &label)
{
    ifstream file(fname);
    Vector *v;
    string s;
    getline(file, s);
    /* number per line */
    int size = count(s.begin(), s.end(), ' ');
    file.seekg(0, ios::beg);
    
    while (!file.eof())
    {
        v = new Vector(size);
        for (size_t i=0; i<v->get_size(); i++)
        {
            double d;
            file >> d;
            //d += 735;
            v->set(i, d);
        }
        data.push_back(*v);

        int l;
        file >> l;
        assert((l==0) || (l==1));
        label.push_back(l);
    }
    data.pop_back();
    label.pop_back();

    file.close();
}

void write_data(const char *fname, vector<Vector> &data, vector<bool> &label)
{
    ofstream file(fname);
    
    for (int i=0; i<data.size(); i++)
    {
        for (size_t j=0; j<data[i].get_size(); j++)
        {
            file << setprecision(12) << data[i].get(j) << " ";
        }
        file << (label[i] ? 1 : 0);
        file << endl;
    }

    file.close();
}

void TestLinearClassifier(int n, int times, double eta)
{
    assert(n < 3);
    /*const int nbPoints = 4;
    Vector p[nbPoints];
    double cord[nbPoints][2] = {{100, 101}, {101, 100}, {100, 100}, {101, 100}};
    double offset = 100;
    //double cord[nbPoints][2] = {{-1,1}, {1,1}, {-1,-1}, {1,-1}};
    bool label[nbPoints] = {true, true, false, false};
    for (int i=0; i<nbPoints; i++)
    {
        p[i].set_size(2);
        p[i].set(0, cord[i][0]-offset);
        p[i].set(1, cord[i][1]-offset);
    }*/
    const char *infile[] = {"dataset/dataset-1",
        "dataset/dataset-2","dataset/dataset-3"};
    const char *outfile[] = {"dataset/myout-1",
        "dataset/myout-2","dataset/myout-3"};

    vector<Vector> data;
    vector<bool> label;
    read_data(infile[n], data, label);
    const int nbPoints = data.size();
    
    Vector mean(data[0].get_size());
    mean.fill(0);
    for (int i=0; i<nbPoints; i++)
    {
        mean.add(data[i]);
    }
    for (int i=0; i<data[0].get_size(); i++)
    {
        double m = mean.get(i) / nbPoints;
        mean.set(i, m);
    }
    for (int i=0; i<nbPoints; i++)
        data[i].sub(mean);

    mean.fill(0);
    LinearClassifier lc;
    lc.reset(mean, data[0].get_size(), eta);
    srand(time(0));
    for (int i=0; i<times; i++)
    {
        int j = rand() % nbPoints;
        lc.update_weights(data[j], label[j]);
    }

    int wrong = 0;
    for (int i=0; i<nbPoints; i++)
    {
        if (lc.classify(data[i]) != label[i])
            //cout << "Classify " << i << "th point wrong." << endl;
            wrong ++;
        label[i] = lc.classify(data[i]); 
    }
    write_data(outfile[n], data, label);
    printf("%d wrong of %d data, accuracy: %f\n", wrong, nbPoints, (double)(nbPoints - wrong)/nbPoints);
}

int main(int argc, char *argv[])
{
    //TestKahanSum();
    //TestMedian();
    //TestVector();

    if (argc != 4)
        cout << "argument is not enough" << endl;
    else
        //TestKmeans(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
        TestLinearClassifier(atoi(argv[1]), atoi(argv[2]), atof(argv[3]));
}
