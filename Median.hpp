#ifndef MEDIAN_HPP
#define MEDIAN_HPP

#include<vector>

class Median
{
    std::vector<double> data;
public:
    void reset();
    void add(double value);  
    double get_median();
};

#endif
