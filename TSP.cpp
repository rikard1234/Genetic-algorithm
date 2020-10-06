#include "TSP.h"
#include <math.h>
TSP::TSP(string path) {
    srand( time( NULL ) );
    fstream stream;
    stream.open(path);

    if(stream.good() == true) {
        stream >> this->population_size;
        stream >> this->mutation_propability;
        stream >> this->number_of_cities;
        cout << "------------------------------------" << endl;
        cout << "PARAMETERS : " << endl;
        cout << "Number of populations: " << this->population_size << endl;
        cout << "Mutation propability: " << this->mutation_propability << endl;
        cout << "Number of cities: " << this->number_of_cities << endl;
        cout << "------------------------------------" << endl;
        this->paths = new vector<int>[this->population_size];
        this->values = new double[this->population_size];
        for(int i = 0; i < this->population_size; i++) this->values[i] = 0;
        while(!stream.eof()) {
            int buffor_city_id;
            stream >> buffor_city_id;
            pair<int, int> buffor_coordinate;
            stream >> buffor_coordinate.first >> buffor_coordinate.second;
            this->coordinates.insert({buffor_city_id - 1, buffor_coordinate});
        }
    }
    else {
        cout << "ERROR";
    }
    stream.close();
}

void TSP::init() {
    for(int i = 0; i < this->population_size; i++) {
        for(int j = 0; j < this->number_of_cities; j++ ) this->paths[i].push_back(j);
        random_shuffle (this->paths[i].begin(), this->paths[i].end() );
    }
}

void TSP::display_population() {
    for(int i = 0; i < this->population_size; i++)
    {
        cout << i << "th population: ";
        for(int j = 0; j < this->paths[i].size(); j++) {
            cout << this->paths[i].at(j) << " ";
        }
        cout << endl;
    }
}

void TSP::target_function() {
    for (int i = 0; i < this->population_size; i++) {
        for (int j = 0, k = 1; k < this->number_of_cities; j++, k++) {
            unordered_map<int, pair<int, int>>::iterator next_city_it = this->coordinates.find(this->paths[i].at(k));
            unordered_map<int, pair<int, int>>::iterator city_it = this->coordinates.find(this->paths[i].at(j));
                this->values[i] += sqrt(
                        pow(next_city_it->second.first - city_it->second.first, 2) + pow(next_city_it->second.second - city_it->second.second, 2));
        }
    }
}

void TSP::display_evaluations() {
    for (int i = 0; i < this->population_size; i++) cout << this->values[i] << " for  " << i << "th population " << endl;
}

void TSP::display_parents() {
    for(int i = 0; i < parents.size(); i++) cout << parents[i].first << " " << parents[i].second << endl;
}
void TSP::select_parents_first() {
    vector<int> selected_one;
    vector<int> selected_second;
    float sum = 0;
    bool* taken = new bool [this->population_size];
    for(int i = 0; i < this->population_size; i++) sum += this->values[i];
    for(int i = 0; i < this->population_size; i++) taken[i] = false;

    for(int i = 0; i < this->population_size; i++) {
        double chance = (double)rand() / (double)RAND_MAX;
        cout << chance << " " << 1 - this->values[i] / sum << " " <<  i << " :chosen" << endl;
        if(!taken[i] && chance < 1 - this->values[i] / sum) {
            selected_one.push_back(i);
            for(int j = (i + 1) % this->population_size; j != i; j = (j + 1) % this->population_size) {
                double chance = (double)rand() / (double)RAND_MAX;
                cout << chance << " " << 1 - this->values[i] / sum << " " << j << " :chosen" << endl;
                if(!taken[j] && chance < 1 - this->values[j] / sum) {
                    selected_second.push_back(j);
                    pair<int,int> buffor_parent;
                    buffor_parent.first = selected_one.front();
                    buffor_parent.second = selected_second.front();
                    this->parents.push_back(buffor_parent);
                    taken[i] = true;
                    taken[j] = true;
                    break;
                }
            }
        }
        selected_one.clear();
        selected_second.clear();
    }
}

void TSP::select_parents_second() {
    vector<int> selected;
    float sum = 0;
    for(int i = 0; i < this->population_size; i++) sum += this->values[i];

    for(int i = 0; i < this->population_size; i++) {
        double chance = (double)rand() / (double)RAND_MAX;
        if(chance < 1 - this->values[i] / sum) selected.push_back(i);
    }

    for(int i = 0, j = 1; j < selected.size(); i+=2, j+=2) {
        pair<int, int> buffor_parent;
        buffor_parent.first = selected[i];
        buffor_parent.second = selected[j];
        this->parents.push_back(buffor_parent);
        if(this->parents.size() == 10) break;
    }
}