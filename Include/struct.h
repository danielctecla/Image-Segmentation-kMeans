#pragma once

#include <cmath>


struct medianClust{
    long double b, g, r;

    medianClust() : b(0), g(0), r(0) {}
};

struct cluster{
    double b;
    double g;
    double r;
    int numPixels;
    struct medianClust median;

    cluster() : b(0), g(0), r(0), numPixels(0), median() {}
    cluster(double b, double g, double r) : b(b), g(g), r(r), numPixels(0), median() {}
    
    double oldCentroidDistance(cluster &clst) const {
        return sqrt((clst.b - b)*(clst.b - b) + 
                    (clst.g - g)*(clst.g - g) + 
                    (clst.r - r)*(clst.r - r));
    }
};

struct bitRGB{
    int ble;
    int grn;
    int red;
    int clust;
    double min_dist;

    bitRGB() : ble(0), grn(0), red(0), clust(-1), min_dist(__DBL_MAX__) {}
    bitRGB(int b, int g, int r) : ble(b), grn(g), red(r), clust(-1), min_dist(__DBL_MAX__) {}

    double distance(cluster &clst) const {
        return(clst.b - static_cast<double>(ble))*(clst.b - static_cast<double>(ble)) + 
              (clst.g - static_cast<double>(grn))*(clst.g - static_cast<double>(grn)) + 
              (clst.r - static_cast<double>(red))*(clst.r - static_cast<double>(red));
    }
};