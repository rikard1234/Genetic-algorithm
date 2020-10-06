#include "TSP.h"
int main() {
    TSP tsp("data2");
    tsp.init();
    tsp.display_population();
    tsp.target_function();
    tsp.display_evaluations();
    tsp.select_parents_second();
    tsp.display_parents();
}