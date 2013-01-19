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
    Vector() {}
    Vector(size_t n);
    Vector(const Vector &inVector);
    Vector& operator = (const Vector &v);
    bool operator == (const Vector &v);
    bool operator != (const Vector &v);
    void fill(double value);
    double get(int i) const;
    void normalize();
    Vector& othogonalize(const Vector& v);
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

    void set_size(size_t _n) { n = _n; data.resize(n);}
    size_t get_size() const { return n; }
    void printall(const char *title) const;
};

void write_data(const char *fname, list<Vector>& data);
void read_data(const char *fname, list<Vector>& data);
Vector vector_mean(list<Vector>& data);
Vector vector_median(list<Vector>& data);
void Box_Muller(double rand_data[], size_t n);

#endif
