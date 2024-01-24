#include "../include/point.h"

#include <iostream>
#include <vector>
#include <math.h>



Point::Point() {}

Point::Point(const std::vector<double> &f) : features(f) {}

Point::Point(const Point& p) : features(p.features) {}

std::vector<double> Point::getFeatures() const { return features; }

double Point::getFeatures(const int i) const { return features.at(i); }

int Point::getFeatureSize() const { return features.size(); }

void Point::setFeatures(const std::vector<double> &f) { features = f; }

void Point::setFeatures(const int i, const double val) { features.at(i) = val; }



Observation::Observation() : Point(), clusterID(-1) {}

Observation::Observation(const std::vector<double> &f, const int id) : Point(f), clusterID(id) {}

int Observation::getClusterID() const { return clusterID; }

void Observation::setClusterID(const int id) { clusterID = id; }

Point Observation::getPoint() const { return *this; }



double distance(const Point &p1, const Point &p2)
{
    // Euclidean distance
    double sum = 0.0;
    for (size_t i = 0; i < p1.getFeatureSize(); ++i) {
        sum += std::pow(p1.getFeatures(i) - p2.getFeatures(i), 2);
    }
    return std::sqrt(sum);
}

std::ostream &operator<<(std::ostream &os, const Observation &ob)
{
    os << "[";
    for (size_t i = 0; i < ob.getFeatureSize(); ++i) {
        os << ob.getFeatures(i);
        if (i != ob.getFeatureSize() - 1) {
			os << ",";
		}
    } 
    os << "]  id = " << ob.getClusterID();
    return os;
}
