#ifndef LCVRP_CONST_H
#define LCVRP_CONST_H

namespace Config {
    static constexpr int POP_SIZE = 300;
    static constexpr int NUM_GROUPS = 5;
    static constexpr double MUT_PROB = 0.01;
    static constexpr double CROSS_PROB = 0.85;
    static constexpr int MIN_RANGE = 0;
    static constexpr int MAX_ITER = 10000;
    static constexpr int SEED = 69420;
    static constexpr int WRONG_SOL = 100000;
    static constexpr int PRINT_INTERVAL = 1000;
    static constexpr int DEPOT_LOCATION = 0;
    static constexpr int FIRST_CUSTOMER = 1;
    static constexpr int ELITE_SIZE = 2;
    static constexpr int TOURNAMENT_SIZE = 3;
}

#endif //LCVRP_CONST_H