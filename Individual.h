#ifndef LCVRP_INDIVIDUAL_H
#define LCVRP_INDIVIDUAL_H
#pragma once
#include <random>
#include <vector>
#include <string>

#include "Evaluator.h"
#include "smartPointer/MySmartPointer.h"

class Individual;
using namespace std;

using indPtr = MySmartPointer<Individual>;

class Individual {
public:
    Individual();
    Individual(mt19937& rng);
    Individual(const evPtr evaluator);
    Individual(const Individual& other);
    Individual& operator=(const Individual& other);

    void randomInitialize(int numGenes, int numGroups);
    double evaluate();
    pair<Individual, Individual> crossover(const Individual& other, double crossProb);
    void mutate(double mutProb, int numGroups);

    double getFitness() const;
    const vector<int>& getGenotype() const;
    string toString() const;
    void setFitness(double fitness);

private:
    vector<int> genotype_;
    double fitness_;
    evPtr evaluator_;
    mt19937 rng_;
};

#endif //LCVRP_INDIVIDUAL_H