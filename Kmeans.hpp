#ifndef KMEANS_HPP
#define KEMANS_HPP


class Kmeans
{
    /* origin points */
    const list<Vector> &points;
    /* number of clusters */
    int k;
    /* the center of each cluster */
    vector<Vector> cluster_centers;
    /* the cluster index of each point */
    vector<int> point_ids;

    /* return the index of the cluster center that closest to the point X */
    int get_closest_center_id(const Vector& X);
    /* initialize the point_ids */
    void init_point_indexes();
    /* update cluster_centers, if center is changed return true, otherwise false */
    bool update_centers();
public:
    Kmeans(list<Vector> &p, int K);
    /* get the ith cluster center */
    Vector get_cluster_center(int i) { return cluster_centers[i]; }
    /* get the cluster index of point i */
    int get_point_id(int i) { return point_ids[i]; }
    /* write data into a file */
    void write_data(const char *fname);

    /* clusterize the points, return the number of iterations */
    int clusterize();
    /* compute the Dunn index of the clusters */
    double DunnIndex();
    /* compute the Davies-Bouldin index of the clusters */
    double DaviesBouldinIndex();
};

#endif
