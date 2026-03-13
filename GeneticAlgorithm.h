#ifndef LCVRP_GENETICALGORITHM_H
#define LCVRP_GENETICALGORITHM_H
#pragma once

#include "Evaluator.h"
#include "Individual.h"
#include <random>
#include <algorithm>

using namespace std;

class GeneticAlgorithm {
public:
    GeneticAlgorithm(int popSize, double crossProb, double mutProb, int maxIter, int numGroups, evPtr evaluator);
    GeneticAlgorithm(int popSize, double crossProb, double mutProb, int maxIter, int numGroups);

    void initializePopulation();
    void performEvaluation();
    void performCrossover();
    void performMutation();
    void run();

    indPtr getBestSolution() const;
    double getBestFitness() const;

private:
    int popSize_;
    double crossProb_;
    double mutProb_;
    int maxIter_;
    int numGroups_;

    vector<Individual> population_;
    evPtr eval_;
    indPtr bestIndividual_;
    double bestFitness_;
    mt19937 rng_;

    Individual selectParent();
};

#endif //LCVRP_GENETICALGORITHM_H