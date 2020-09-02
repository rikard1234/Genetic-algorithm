#include "TSP.h"
#include <math.h>
TSP::TSP(string path) {

    fstream stream;
    stream.open(path);

    if(stream.good() == true) {
        stream >> this->number_of_populations;
        stream >> this->mutation_propability;
        stream >> this->number_of_cities;
        cout << "------------------------------------" << endl;
        cout << "PARAMETERS : " << endl;
        cout << "Number of populations: " << this->number_of_populations << endl;
        cout << "Mutation propability: " << this->mutation_propability << endl;
        cout << "Number of cities: " << this->number_of_cities << endl;
        cout << "------------------------------------" << endl;
        this->paths = new vector<int>[this->number_of_populations];
        this->values = new double[this->number_of_populations];
        for(int i = 0; i < this->number_of_populations; i++) this->values[i] = 0;
        while(!stream.eof()) {
            int buffor_city_id;
            stream >> buffor_city_id;
            pair<int, int> buffor_coordinate;
            stream >> buffor_coordinate.first >> buffor_coordinate.second;
            this->coordinates.insert({buffor_city_id, buffor_coordinate});
        }
    }
    else {
        cout << "ERROR";
    }
    stream.close();
}

void TSP::init() {
    for(int i = 0; i < this->number_of_populations; i++)
    {
        vector<int> available_ids(this->number_of_cities);
        for(int i = 0; i < this->number_of_cities; i++) available_ids[i] = i;
        while(available_ids.size()) {
            int n = available_ids.size();
            int index = rand () % n;
            int buffor_id = available_ids[index] + 1;
            this->paths[i].push_back(buffor_id);
            swap(available_ids[index], available_ids[n-1]);
            available_ids.pop_back();
        }
    }
}

void TSP::display_population() {
    for(int i = 0; i < this->number_of_populations; i++)
    {
        cout << i << "th population: ";
        for(int j = 0; j < this->paths[i].size(); j++) {
            cout << this->paths[i].at(j) << " ";
        }
        cout << endl;
    }
}

void TSP::target_function() {
    for (int i = 0; i < this->number_of_populations; i++) {
        for (int j = 0, k = 1; k < this->number_of_cities; j++, k++) {
            unordered_map<int, pair<int, int>>::iterator it = this->coordinates.find(this->paths[i].at(k));
            unordered_map<int, pair<int, int>>::iterator IT = this->coordinates.find(this->paths[i].at(j));
            if (it != this->coordinates.end() && IT != this->coordinates.end()) {
                this->values[i] += sqrt(
                        pow(it->second.first - IT->second.first, 2) + pow(it->second.second - IT->second.second, 2));
            }
        }
    }
}

void TSP::display_evaluations() {
    for (int i = 0; i < this->number_of_populations; i++) cout << this->values[i] << " for  " << i << "th population ";
}