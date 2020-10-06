//
// Created by jakub on 02.09.2020.
//

#ifndef GENETICS_TSP_H
#define GENETICS_TSP_H

#include <random>
#include <vector>
#include <iterator>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <utility>
#include <algorithm>
using namespace std;

class TSP {
public:
    TSP(string);
    void init();
    void display_population();
    void display_evaluations();
    void target_function();
    void select_parents_first();
    void select_parents_second();
    void display_parents();
private:
    vector<pair<int,int>> parents;
    double* values;
    vector<int>* paths;
    int number_of_cities;
    int population_size;
    int mutation_propability;
    unordered_map<int, pair<int,int>> coordinates;
};



#endif //GENETICS_TSP_H
