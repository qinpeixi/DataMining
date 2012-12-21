#ifndef VECTOR_HPP
#define VECTOR_HPP

#include<vector>
#include<list>
using namespace std;

class Vector
{
protected:
    vector<double> data;
    size_t n;
public:
    Vector(size_t n);
    void fill(double value);
    double get(int i) const;
    void set(int i, double value);
    void add(const Vector& v);
    void sub(const Vector& v);
    void mul(double k);
    void inc_mul(const Vector& v, double k);
    double dot(const Vector& v) const;
    double square_norm() const;
    double norm() const;
    double sum() const;
    double min() const;
    double max() const;

    size_t get_size() const { return n; }
};

void write_data(const char *fname, list<Vector>& data);
void read_data(const char *fname, list<Vector>& data, size_t size);
Vector vector_mean(list<Vector>& data);
Vector vector_median(list<Vector>& data);

#endif
