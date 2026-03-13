cmake_minimum_required(VERSION 4.0)
project(LcVRP)

set(CMAKE_CXX_STANDARD 26)

add_executable(LcVRP main.cpp
        smartPointer/MySmartPointer.h
        Evaluator.h
        Individual.h
        Evaluator.cpp
        Individual.cpp
        const.h
        GeneticAlgorithm.cpp
        GeneticAlgorithm.h
)

