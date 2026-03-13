#include "GeneticAlgorithm.h"
#include "const.h"

#include <iostream>
#include <random>

GeneticAlgorithm::GeneticAlgorithm(int popSize, double crossProb, double mutProb, int maxIter, int numGroups, evPtr evaluator)
    : eval_(evaluator),
      popSize_(popSize),
      crossProb_(crossProb),
      mutProb_(mutProb),
      maxIter_(maxIter),
      numGroups_(numGroups),
      bestIndividual_(nullptr),
      bestFitness_(Config::WRONG_SOL),
      rng_(random_device{}()) {}

GeneticAlgorithm::GeneticAlgorithm(int popSize, double crossProb, double mutProb, int maxIter, int numGroups)
    : eval_(new Evaluator(numGroups)),
      popSize_(popSize),
      crossProb_(crossProb),
      mutProb_(mutProb),
      maxIter_(maxIter),
      numGroups_(numGroups),
      bestIndividual_(nullptr),
      bestFitness_(Config::WRONG_SOL),
      rng_(random_device{}()) {}

void GeneticAlgorithm::initializePopulation() {
    population_.clear();
    population_.reserve(popSize_);

    for (int i = 0; i < popSize_; i++) {
        population_.emplace_back(eval_);
        population_.back().randomInitialize(eval_->getNumLocations(), numGroups_);
    }
}

void GeneticAlgorithm::performEvaluation() {
    double currentBestFitness = Config::WRONG_SOL;
    int bestIndex = -1;

    for (size_t i = 0; i < population_.size(); i++) {
        double fitness = population_[i].evaluate();

        if (bestIndex == -1 || (fitness > 0.0 && fitness < currentBestFitness)) {
            currentBestFitness = fitness;
            bestIndex = i;
        }
    }

    if (bestIndex != -1) {
        if (bestIndividual_.getPtr() == nullptr || currentBestFitness < bestFitness_) {
            bestFitness_ = currentBestFitness;
            bestIndividual_ = new Individual(population_[bestIndex]);
        }
    }
}

Individual GeneticAlgorithm::selectParent() {
    uniform_int_distribution<int> dist(0, popSize_ - 1);

    Individual best = population_[dist(rng_)];

    for (int i = 1; i < Config::TOURNAMENT_SIZE; i++) {
        Individual candidate = population_[dist(rng_)];

        if (candidate.getFitness() > 0.0 &&
            (best.getFitness() <= 0.0 || candidate.getFitness() < best.getFitness())) {
            best = candidate;
        }
    }

    return best;
}

void GeneticAlgorithm::performCrossover() {
    vector<Individual> newGeneration;
    newGeneration.reserve(popSize_);

    vector<pair<double, int>> fitnessIndex;
    for (int i = 0; i < popSize_; i++) {
        if (population_[i].getFitness() > 0.0) {
            fitnessIndex.push_back(make_pair(population_[i].getFitness(), i));
        }
    }

    sort(fitnessIndex.begin(), fitnessIndex.end());

    for (int i = 0; i < Config::ELITE_SIZE && i < fitnessIndex.size(); i++) {
        newGeneration.push_back(population_[fitnessIndex[i].second]);
    }

    while (newGeneration.size() < popSize_) {
        Individual parent1 = selectParent();
        Individual parent2 = selectParent();

        pair<Individual, Individual> offspring = parent1.crossover(parent2, crossProb_);

        newGeneration.push_back(offspring.first);

        if (newGeneration.size() < popSize_) {
            newGeneration.push_back(offspring.second);
        }
    }

    population_ = newGeneration;
}

void GeneticAlgorithm::performMutation() {
    for (auto& individual : population_) {
        individual.mutate(mutProb_, numGroups_);
    }
}

void GeneticAlgorithm::run() {
    initializePopulation();
    performEvaluation();

    for (int iteration = 0; iteration < maxIter_; iteration++) {
        performCrossover();
        performMutation();
        performEvaluation();

        if (iteration % Config::PRINT_INTERVAL == 0) {
            cout << iteration
                 << ": Best solution: " << getBestSolution()->toString()
                 << ", Fitness: " << getBestFitness() << '\n';
        }
    }

    cout << "Best Solution: " << getBestSolution()->toString() << '\n';
    cout << "Best Fitness: " << getBestFitness() << endl;
}

indPtr GeneticAlgorithm::getBestSolution() const {
    return bestIndividual_;
}

double GeneticAlgorithm::getBestFitness() const {
    return bestFitness_;
}