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
#include <set>
using namespace std;
struct myComp {
    constexpr bool operator()(
        pair<int, double> const& a,
        pair<int, double> const& b)
        const noexcept
    {
        return a.second < b.second;
    }
};
class TSP {
public:
    TSP(string);
    void init();
    void display_population();
    void display_evaluations();
    void target_function();
    void select_and_cross();
    void display_parents();
    void crossing();
    void final_step();
    void start();
    void selecting_parents_1();
    void selecting_parents_2();
    vector<pair<int, long double>> sorting();
    vector<pair<long double,int>> sorting_unique();
private:
    vector<pair<int,int>> parents;
    long double* values;
    double* buffor_values;
    vector<vector<int>> paths;
    int number_of_cities;
    int population_size;
    int mutation_propability;
    int h1, h2;
    unordered_map<int, pair<int,int>> coordinates;
};



#endif //GENETICS_TSP_H
