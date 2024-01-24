#pragma once
#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
#include <vector>

class Point
{
private:
    std::vector<double> features; // point features [features.size = dimensions]

public:
    Point();
    Point(const std::vector<double> &f);
    Point(const Point &p);

    std::vector<double> getFeatures() const;
    double getFeatures(const int i) const;
    int getFeatureSize() const;

    void setFeatures(const std::vector<double> &f);
    void setFeatures(const int i, const double val);
};

class Observation : public Point
{
    int clusterID;

public:
    Observation();
    Observation(const std::vector<double> &f, const int id);

    int getClusterID() const;
    void setClusterID(const int id);
    Point getPoint() const;

    friend std::ostream &operator<<(std::ostream &os, const Observation &observation);
};

static double distance(const Point &p1, const Point &p2);
std::ostream &operator<<(std::ostream &os, const Observation &observation);

using ObservationsWithIterations = std::pair<std::vector<Observation>, int>;

#endif // __POINT_H__
