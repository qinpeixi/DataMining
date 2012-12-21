#ifndef KAHANSUM_HPP
#define KAHANSUM_HPP

class KahanSum
{
    double sum;
    double c, y, t;
public:
    KahanSum():sum(0.0), c(0.0) {}

    void reset();
    void add(double v);
    double get_sum();
};

#endif
