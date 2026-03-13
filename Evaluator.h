#ifndef LCVRP_EVALUATOR_H
#define LCVRP_EVALUATOR_H
#pragma once
#include <string>
#include <vector>
#include <fstream>

class Individual;
class Evaluator;

#include "smartPointer/MySmartPointer.h"

using namespace std;
using evPtr = MySmartPointer<Evaluator>;

struct Coordinate {
private:
    double x_;
    double y_;
public:
    Coordinate();
    Coordinate(double x, double y);
    double getX() const;
    double getY() const;
};

class Evaluator {
public:
    Evaluator(int numGroups);
    Evaluator(const Evaluator& other);
    ~Evaluator() = default;

    bool loadInstance(const string& filePath, string& errorMessage);
    double evaluate(const Individual& individual) const;

    int getNumLocations() const;
    int getNumGroups() const;
    int getCapacity() const;
    bool isLoaded() const;

private:
    int numLocations_;
    int numGroups_;
    int capacity_;
    vector<vector<double>> distanceMatrix_;
    vector<int> permutation_;
    vector<int> demands_;
    bool instanceLoaded_;
    vector<Coordinate> coords_;

    vector<vector<double>> buildMatrix(const vector<Coordinate>& coords);
    void parseNodeCoordSection(ifstream& file);
    void parseDemandSection(ifstream& file);
    double calculateDistance(const Coordinate& c1, const Coordinate& c2) const;
};

#endif //LCVRP_EVALUATOR_H