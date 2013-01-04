/*************************************************************************/
/* Copyright (C) DataMining Expriment -USTC, 2012                        */
/*                                                                       */
/*  File Name              :  Vector.cpp                                 */
/*  Pricipal Author        :  qinpeixi(SA12226212)                       */
/*  Subsystem Name         :  Exp 1                                      */
/*  Module Name            :  Vector                                     */
/*  Language               :  C++                                        */
/*  Target Environment     :  Any                                        */
/*  Created Time           :  Thu 20 Dec 2012 02:57:28 PM CST            */
/*  Description            :  Implementaion of Vector                    */
/*************************************************************************/

#include<iostream>
#include<algorithm>
#include<cassert>
#include<cmath>
#include<cfloat>
#include<fstream>
#include<cassert>
#include"Vector.hpp"
#include"KahanSum.hpp"
#include"Median.hpp"
using namespace std;

Vector::Vector(size_t n)
{
    data.resize(n);
    this->n = n;
}

Vector::Vector(const Vector &v)
{
    data.resize(v.get_size());
    this->n = v.get_size();

    vector<double>::iterator i = data.begin();
    vector<double>::const_iterator j = v.data.begin();
    for ( ; i != data.end() ; i++, j++)
    {
        *i = *j;
    }
}

Vector& Vector::operator = (const Vector &v)
{
    data.resize(v.get_size());
    this->n = v.get_size();

    vector<double>::iterator i = data.begin();
    vector<double>::const_iterator j = v.data.begin();
    for ( ; i != data.end() ; i++, j++)
    {
        *i = *j;
    }
}

bool Vector::operator == (const Vector &v)
{
    if (n != v.n)
        return false;

    vector<double>::const_iterator i = data.begin();
    vector<double>::const_iterator j = v.data.begin();
    for (; i != data.end(); i++, j++)
    {
        if (*i != *j)
            return false;
    }

    return true;
}

bool Vector::operator != (const Vector &v)
{
    return !(*this == v);
}

void Vector::fill(double value)
{
    data.assign(data.size(), value);
}

void Vector::normalize()
{
    double sum = 0;
    for (int i=0; i<n; i++)
        sum += data[i] * data[i];
    sum = sqrt(sum);
    for (int i=0; i<n; i++)
        data[i] /= sum;
}

double Vector::get(int i) const
{
    return data[i];
}

void Vector::set(int i, double value)
{
    data[i] = value;
}

void Vector::add(const Vector& v)
{
    assert(data.size() == v.data.size());

    vector<double>::iterator i = data.begin();
    vector<double>::const_iterator j = v.data.begin();
    for ( ; i != data.end() ; i++, j++)
    {
        *i += *j;
    }
}

void Vector::sub(const Vector& v)
{
    assert(data.size() == v.data.size());

    vector<double>::iterator i = data.begin();
    vector<double>::const_iterator j = v.data.begin();
    for ( ; i != data.end() ; i++, j++)
    {
        *i -= *j;
    }
}

void Vector::mul(double k)
{
    vector<double>::iterator i = data.begin();
    for ( ; i != data.end() ; i++)
    {
        *i *= k;
    }
}

void Vector::inc_mul(const Vector& v, double k)
{
    assert(data.size() == v.data.size());

    vector<double>::iterator i = data.begin();
    vector<double>::const_iterator j = v.data.begin();
    for ( ; i != data.end() ; i++, j++)
    {
        *i += k * (*j);
    }
}

double Vector::dot(const Vector& v) const
{
    assert(data.size() == v.data.size());

    KahanSum ks;

    vector<double>::const_iterator i = data.begin();
    vector<double>::const_iterator j = v.data.begin();
    for ( ; i != data.end() ; i++, j++)
    {
        ks.add((*i) * (*j));
    }

    return ks.get_sum();
}

double Vector::square_norm() const
{
    return dot(*this);
}

double Vector::norm() const
{
    return sqrt(square_norm());
}

double Vector::sum() const
{
    KahanSum ks;
    vector<double>::const_iterator i = data.begin();

    for ( ; i != data.end() ; i++)
    {
        ks.add(*i);
    }
    
    return ks.get_sum();
}

double Vector::min() const
{
    double min = DBL_MAX;
    vector<double>::const_iterator i = data.begin();

    for ( ; i != data.end() ; i++)
    {
        if (min > *i)
            min = *i;
    }

    return min;
}

double Vector::max() const
{
    double max = DBL_MIN;
    vector<double>::const_iterator i = data.begin();

    for ( ; i != data.end() ; i++)
    {
        if (max < *i)
            max = *i;
    }

    return max;
}

// one point per vector
void write_data(const char *fname, list<Vector>& data)
{
    ofstream file(fname);
    
    for (list<Vector>::const_iterator i = data.begin();
            i != data.end(); i++)
    {
        for (size_t j=0; j<i->get_size(); j++)
        {
            file << i->get(j) << " ";
        }
        file << endl;
    }

    file.close();
}

void read_data(const char *fname, list<Vector>& data)
{
    ifstream file(fname);
    Vector *v;
    string s;
    getline(file, s);
    /* number per line */
    int size = count(s.begin(), s.end(), ' ') + 1;
    file.seekg(0, ios::beg);
    
    while (!file.eof())
    {
        v = new Vector(size);
        for (size_t i=0; i<v->get_size(); i++)
        {
            double d;
            file >> d;
            v->set(i, d);
        }
        data.push_back(*v);
    }
    data.pop_back();

    file.close();
}

Vector vector_mean(list<Vector>& data)
{
    Vector v(data.begin()->get_size());
    KahanSum ks;
    for (size_t i=0; i<v.get_size(); i++)
    {
        for (list<Vector>::const_iterator j = data.begin();
                j != data.end(); j++)
        {
            ks.add(j->get(i));
        }

        double mean = ks.get_sum() / data.size();
        v.set(i, mean);
        ks.reset();
    }

    return v;
}

Vector vector_median(list<Vector>& data)
{
    Vector v(data.begin()->get_size());
    Median me;
    for (size_t i=0; i<v.get_size(); i++)
    {
        for (list<Vector>::const_iterator j = data.begin();
                j != data.end(); j++)
        {
            me.add(j->get(i));
        }

        double median = me.get_median();
        v.set(i, median);
        me.reset();
    }

    return v;
}
