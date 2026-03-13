#include <string>
#include <iostream>

#include "Evaluator.h"
#include "GeneticAlgorithm.h"
#include "const.h"

using namespace std;

int main() {
    string filePath = "/data/lcvrp/Vrp-Set-A/A-n34-k5.lcvrp";

    evPtr evaluator = new Evaluator(Config::NUM_GROUPS);

    string errorMessage;
    bool loaded = evaluator->loadInstance(filePath, errorMessage);

    if (!loaded) {
        cerr << "Blad wczytywania instancji: " << errorMessage << endl;
        return 1;
    }

    cout << "Instancja wczytana pomyslnie." << endl;
    cout << "Liczba lokalizacji: " << evaluator->getNumLocations() << endl;
    cout << "Liczba grup: " << evaluator->getNumGroups() << endl;
    cout << "Pojemnosc pojazdu: " << evaluator->getCapacity() << endl;
    cout << endl;

    GeneticAlgorithm ga(
        Config::POP_SIZE,
        Config::CROSS_PROB,
        Config::MUT_PROB,
        Config::MAX_ITER,
        Config::NUM_GROUPS,
        evaluator
    );

    ga.run();

    return 0;
}