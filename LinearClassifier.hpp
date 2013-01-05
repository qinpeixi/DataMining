#ifndef LINEAR_CLASSIFIER_HPP
#define LINEAR_CLASSIFIER_HPP

#include "Vector.hpp"

class LinearClassifier
{
    Vector V;
    Vector C;
    double vn;
    double eta;
    /* set V and vn such as they represent an hyperplane of origin C and normal V */
    void set_weights(const Vector &_C, const Vector &N);
    /* return the signed distance of point X to the hyperplane (V, vn) */
    double signed_dist(const Vector &X) const;

public:
    Vector &GetV() { return V; }
    double GetVn() { return vn; }
    double GetEta() { return eta; }

    /* return true if X is on the positive side, false if X is on
     * the negative side of the hyperplane */
    bool classify(const Vector &X) const;

    /* Updates (V, vn) */
    void update_weights(const Vector &X, bool inSide);

    /* Initialze the classifier with null C and random N */
    void reset(Vector &_C, size_t n, double _eta);

};

/* Generate normally distributed values */
void Box_Muller(double rand_data[], size_t n);

#endif
