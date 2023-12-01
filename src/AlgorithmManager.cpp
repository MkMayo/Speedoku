//
// Created by logan on 11/29/2023.
//
#include <string>
using namespace std;
#include "AlgorithmManager.h"

AlgorithmManager::AlgorithmManager() {
    std::pair<int, float> default_pair;
    default_pair.first = 0;
    default_pair.second = 0.0;
    this->algos.emplace("backtracking", default_pair);
    this->algos.emplace("crosshatching", default_pair);
}

void AlgorithmManager::updateAlgo(string algorithm, float runtime){
    this->algos[algorithm].first += 1;
    this->algos[algorithm].second += runtime;
}

float AlgorithmManager::getAverage(string algorithm){ //returns average runtime for given algorithm

    float avg = algos[algorithm].second / algos[algorithm].first;
    return avg;
}

int AlgorithmManager::getRunCount(string algorithm){
    return this->algos[algorithm].first;
}
