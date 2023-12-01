//
// Created by logan on 11/29/2023.
//
#include <map>
#ifndef HELLOSFML_ALGORITHMMANAGER_H
#define HELLOSFML_ALGORITHMMANAGER_H
#include <string>

class AlgorithmManager {
public:
    AlgorithmManager();
    int getRunCount(string algorithm);
    float getAverage(string algorithm);
    void updateAlgo(string algorithm, float runtime);
private:
    int total_run;
    map<string, pair<int, float>> algos;
};


#endif //HELLOSFML_ALGORITHMMANAGER_H
