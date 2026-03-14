#include "Individual.h"
#include "const.h"

#include <iostream>
#include <random>
#include <sstream>

Individual::Individual()
    : genotype_(),
      fitness_(0.0),
      evaluator_(nullptr),
      rng_(random_device{}()) {}

Individual::Individual(mt19937& rng)
    : genotype_(),
      fitness_(0.0),
      evaluator_(nullptr),
      rng_(rng) {}

Individual::Individual(const evPtr evaluator)
    : genotype_(),
      fitness_(0.0),
      evaluator_(evaluator),
      rng_(random_device{}()) {

    if (evaluator_.getPtr() != nullptr) {
        int numLocations = evaluator_->getNumLocations();
        genotype_.resize(numLocations);

        genotype_[Config::DEPOT_LOCATION] = 0;

        uniform_int_distribution<int> dist(0, evaluator_->getNumGroups() - 1);
        for (int i = Config::FIRST_CUSTOMER; i < numLocations; i++) {
            genotype_[i] = dist(rng_);
        }
    }
}

Individual::Individual(const Individual& other)
    : genotype_(other.genotype_),
      fitness_(other.fitness_),
      evaluator_(other.evaluator_),
      rng_(other.rng_) {}

Individual& Individual::operator=(const Individual& other) {
    if (this != &other) {
        genotype_ = other.genotype_;
        fitness_ = other.fitness_;
        evaluator_ = other.evaluator_;
        rng_ = other.rng_;
    }
    return *this;
}

void Individual::randomInitialize(int numGenes, int numGroups) {
    genotype_.resize(numGenes);

    genotype_[Config::DEPOT_LOCATION] = 0;

    uniform_int_distribution<int> dist(0, numGroups - 1);
    for (int i = Config::FIRST_CUSTOMER; i < numGenes; i++) {
        genotype_[i] = dist(rng_);
    }
}

pair<Individual, Individual> Individual::crossover(const Individual& other, double crossProb) {
    uniform_real_distribution<double> realDist(0.0, 1.0);

    if (realDist(rng_) < crossProb) {
        Individual child1(*this);
        Individual child2(other);

        uniform_int_distribution<int> cutDist(0, genotype_.size() - 1);
        size_t cutPoint = cutDist(rng_);

        for (size_t i = cutPoint; i < genotype_.size(); i++) {
            child1.genotype_[i] = other.genotype_[i];
            child2.genotype_[i] = genotype_[i];
        }

        child1.fitness_ = 0.0;
        child2.fitness_ = 0.0;

        return make_pair(child1, child2);
    }

    return make_pair(*this, other);
}

void Individual::mutate(double mutProb, int numGroups) {
    uniform_real_distribution<double> probDist(0.0, 1.0);
    uniform_int_distribution<int> groupDist(0, numGroups - 1);

    for (size_t i = Config::FIRST_CUSTOMER; i < genotype_.size(); i++) {
        if (probDist(rng_) < mutProb) {
            genotype_[i] = groupDist(rng_);
        }
    }
}

double Individual::evaluate() {
    if (evaluator_.getPtr() == nullptr) {
        return Config::WRONG_SOL;
    }

    double evaluationResult = evaluator_->evaluate(*this);
    setFitness(evaluationResult);
    return evaluationResult;
}

string Individual::toString() const {
    ostringstream oss;

    for (size_t i = 0; i < genotype_.size(); i++) {
        oss << genotype_[i];
        if (i < genotype_.size() - 1) {
            oss << " ";
        }
    }

    return oss.str();
}

double Individual::getFitness() const {
    return fitness_;
}

const vector<int>& Individual::getGenotype() const {
    return genotype_;
}

void Individual::setFitness(double fitness) {
    fitness_ = fitness;
}