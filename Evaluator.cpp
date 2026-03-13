#include "Evaluator.h"
#include "Individual.h"
#include "const.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>

Coordinate::Coordinate() : x_(0.0), y_(0.0) {}

Coordinate::Coordinate(double x, double y) : x_(x), y_(y) {}

double Coordinate::getX() const {
    return x_;
}

double Coordinate::getY() const {
    return y_;
}

Evaluator::Evaluator(int numGroups)
    : numLocations_(0),
      numGroups_(numGroups),
      capacity_(0),
      distanceMatrix_(),
      permutation_(),
      demands_(),
      instanceLoaded_(false),
      coords_() {}

Evaluator::Evaluator(const Evaluator& other)
    : numLocations_(other.numLocations_),
      numGroups_(other.numGroups_),
      capacity_(other.capacity_),
      distanceMatrix_(other.distanceMatrix_),
      permutation_(other.permutation_),
      demands_(other.demands_),
      instanceLoaded_(other.instanceLoaded_),
      coords_(other.coords_) {}

bool Evaluator::loadInstance(const string& filePath, string& errorMessage) {
    ifstream file(filePath);
    if (!file.is_open()) {
        errorMessage = "Nie mozna otworzyc pliku: " + filePath;
        return false;
    }

    string line;
    bool hasNodeCoords = false;
    bool hasDemands = false;
    bool hasPermutation = false;

    while (getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("DIMENSION") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                try {
                    int dimension = stoi(line.substr(colonPos + 1));
                    if (dimension <= 0) {
                        errorMessage = "DIMENSION musi byc dodatnie";
                        file.close();
                        return false;
                    }
                    numLocations_ = dimension;
                } catch (...) {
                    errorMessage = "Blad parsowania DIMENSION";
                    file.close();
                    return false;
                }
            }
        }
        else if (line.find("CAPACITY") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                try {
                    int capacity = stoi(line.substr(colonPos + 1));
                    if (capacity <= 0) {
                        errorMessage = "CAPACITY musi byc dodatnie";
                        file.close();
                        return false;
                    }
                    capacity_ = capacity;
                } catch (...) {
                    errorMessage = "Blad parsowania CAPACITY";
                    file.close();
                    return false;
                }
            }
        }
        else if (line.find("NODE_COORD_SECTION") != string::npos) {
            if (numLocations_ == 0) {
                errorMessage = "DIMENSION musi byc zdefiniowany przed NODE_COORD_SECTION";
                file.close();
                return false;
            }
            parseNodeCoordSection(file);
            distanceMatrix_ = buildMatrix(coords_);
            hasNodeCoords = true;
        }
        else if (line.find("DEMAND_SECTION") != string::npos) {
            if (numLocations_ == 0) {
                errorMessage = "DIMENSION musi byc zdefiniowany przed DEMAND_SECTION";
                file.close();
                return false;
            }
            parseDemandSection(file);
            hasDemands = true;
        }
        else if (line.find("PERMUTATION") != string::npos) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                vector<int> permutation;
                string permData = line.substr(colonPos + 1);
                istringstream iss(permData);
                int customerId;
                while (iss >> customerId) {
                    if (customerId < 0 || customerId > numLocations_) {
                        errorMessage = "Nieprawidlowy ID klienta w PERMUTATION: " + to_string(customerId);
                        file.close();
                        return false;
                    }
                    permutation.push_back(customerId - 1);
                }
                permutation_ = permutation;
                hasPermutation = true;
            }
        }
        else if (line.find("EOF") != string::npos) {
            break;
        }
    }

    file.close();

    if (!hasNodeCoords) {
        errorMessage = "Brak NODE_COORD_SECTION w pliku";
        return false;
    }
    if (!hasDemands) {
        errorMessage = "Brak DEMAND_SECTION w pliku";
        return false;
    }
    if (!hasPermutation) {
        errorMessage = "Brak PERMUTATION w pliku";
        return false;
    }

    instanceLoaded_ = true;
    return true;
}

void Evaluator::parseNodeCoordSection(ifstream& file) {
    coords_.resize(numLocations_);

    for (int i = 0; i < numLocations_; i++) {
        int nodeId;
        double x, y;
        file >> nodeId >> x >> y;
        if (nodeId > 0 && nodeId <= numLocations_) {
            coords_[nodeId - 1] = Coordinate(x, y);
        }
    }
}

void Evaluator::parseDemandSection(ifstream& file) {
    demands_.resize(numLocations_);

    for (int i = 0; i < numLocations_; i++) {
        int nodeId, demand;
        file >> nodeId >> demand;
        if (nodeId > 0 && nodeId <= numLocations_) {
            demands_[nodeId - 1] = demand;
        }
    }
}

double Evaluator::calculateDistance(const Coordinate& c1, const Coordinate& c2) const {
    double dx = c1.getX() - c2.getX();
    double dy = c1.getY() - c2.getY();
    return sqrt(dx * dx + dy * dy);
}

vector<vector<double>> Evaluator::buildMatrix(const vector<Coordinate>& coords) {
    vector<vector<double>> matrix(numLocations_, vector<double>(numLocations_, 0.0));

    for (int i = 0; i < numLocations_; i++) {
        for (int j = 0; j < numLocations_; j++) {
            if (i == j) {
                matrix[i][j] = 0.0;
            } else {
                matrix[i][j] = calculateDistance(coords[i], coords[j]);
            }
        }
    }

    return matrix;
}

double Evaluator::evaluate(const Individual& individual) const {
    const vector<int>& genotype = individual.getGenotype();

    if (genotype.empty() || genotype.size() != numLocations_) {
        return Config::WRONG_SOL;
    }

    double totalDistance = 0.0;

    for (int groupId = 0; groupId < numGroups_; groupId++) {
        double routeLength = 0.0;
        int lastLocation = Config::DEPOT_LOCATION;
        int currentLoad = 0;

        for (size_t i = 0; i < permutation_.size(); i++) {
            int locationIndex = permutation_[i];

            if (locationIndex < 0 || locationIndex >= numLocations_) {
                return Config::WRONG_SOL;
            }

            if (genotype[locationIndex] == groupId) {
                if (lastLocation >= distanceMatrix_.size() ||
                    locationIndex >= distanceMatrix_[lastLocation].size()) {
                    return Config::WRONG_SOL;
                }

                routeLength += distanceMatrix_[lastLocation][locationIndex];
                currentLoad += demands_[locationIndex];
                lastLocation = locationIndex;
            }
        }

        if (currentLoad > capacity_) {
            return Config::WRONG_SOL;
        }

        if (lastLocation != Config::DEPOT_LOCATION) {
            if (lastLocation < distanceMatrix_.size() &&
                Config::DEPOT_LOCATION < distanceMatrix_[lastLocation].size()) {
                routeLength += distanceMatrix_[lastLocation][Config::DEPOT_LOCATION];
            }
        }

        totalDistance += routeLength;
    }

    return totalDistance;
}

int Evaluator::getNumLocations() const {
    return numLocations_;
}

int Evaluator::getNumGroups() const {
    return numGroups_;
}

int Evaluator::getCapacity() const {
    return capacity_;
}

bool Evaluator::isLoaded() const {
    return instanceLoaded_;
}