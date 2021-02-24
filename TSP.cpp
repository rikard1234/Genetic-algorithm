#include "TSP.h"
#include <math.h>
#include <queue>
#include <random>
#include <iomanip>
#include <algorithm>
#include <set>
#include <unordered_set>
using std::cout;
using std::endl;
using std::setprecision;
struct Comparator
{
    bool operator() (const std::pair<int, long double>& lhs, const std::pair<int, long double>& rhs) const
    {
        return lhs.second < rhs.second;
    }
};
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
        //this->paths = new vector<int>[this->population_size];
        this->values = new long double[2*this->population_size];
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
        vector<int> buffor_vector;
        for(int j = 0; j < this->number_of_cities; j++ ) buffor_vector.push_back(j);
        this->paths.push_back(buffor_vector);
        random_shuffle (this->paths[i].begin(), this->paths[i].end() );
    }
}

void TSP::display_population() {
    for(int i = 0; i < this->paths.size(); i++)
    {
        cout << i << "th Individual: ";
        for(int j = 0; j < this->paths[i].size(); j++) {
            cout << this->paths[i].at(j) << " ";
        }

        cout << endl;
    }
}

void TSP::target_function() {
    for (int i = 0; i < this->paths.size(); i++) {
        for (int j = 0, k = 1; k < this->number_of_cities; j++, k++) {
            unordered_map<int, pair<int, int>>::iterator next_city_it = this->coordinates.find(this->paths[i].at(k));
            unordered_map<int, pair<int, int>>::iterator city_it = this->coordinates.find(this->paths[i].at(j));
            this->values[i] += sqrt(
                    pow(next_city_it->second.first - city_it->second.first, 2) + pow(next_city_it->second.second - city_it->second.second, 2));
        }
        this->values[i] = (1 / this->values[i]);
    }
}

void TSP::display_evaluations() {
    vector<pair<int, long double>> sorted_values = sorting();
    for (int i = 0; i < sorted_values.size(); i++) cout << (1 / sorted_values[i].second) << " for  " << sorted_values[i].first << "th Individual " << endl;
}

void TSP::display_parents() {
    for(int i = 0; i < parents.size(); i++) cout << parents[i].first << " " << parents[i].second << endl;
}

void TSP::select_and_cross() {
    this->selecting_parents_1();
    //this->selecting_parents_2();
}

void TSP::crossing() {
    int k1 = this->number_of_cities * 0.3;
    int k2 = this->number_of_cities * 0.35;

    vector<int> values_1;
    vector<int> values_2;

    vector<int> swapping_values_1;
    vector<int> swapping_values_2;
    for(int i = k1; i <= k2; i++) {
        swapping_values_1.push_back(this->paths[this->parents.back().second].at(i));
        swapping_values_2.push_back(this->paths[this->parents.back().first].at(i));
    }

    vector<int> buffor_path_1;
    vector<int> buffor_path_2;
    for(int i = (k2 + 1) % this->number_of_cities; i != k2; i = (i + 1) % this->number_of_cities) {
        buffor_path_1.push_back(this->paths[this->parents.back().first].at(i));
        buffor_path_2.push_back(this->paths[this->parents.back().second].at(i));
    }

    buffor_path_1.push_back(this->paths[this->parents.back().first].at(k2));
    buffor_path_2.push_back(this->paths[this->parents.back().second].at(k2));
    for(int i = 0; i < (k2 - k1) + 1; i++) {
        buffor_path_1.erase(remove(buffor_path_1.begin(), buffor_path_1.end(), swapping_values_1[i]));
        buffor_path_2.erase(remove(buffor_path_2.begin(), buffor_path_2.end(), swapping_values_2[i]));
    }

    vector<int> children_1;
    vector<int> children_2;
    for(int i = 0; i < (k2 - k1 + 1); i++) {
        children_1.push_back(swapping_values_1[i]);
        children_2.push_back(swapping_values_2[i]);
    }

    int w1 = 0;
    for(int i = 0; children_1.size() != this->number_of_cities - k1; i++, w1++) {
        children_1.push_back(buffor_path_1[i]);
        children_2.push_back(buffor_path_2[i]);
    }

    vector<int> children_1_ins;
    vector<int> children_2_ins;
    for(int i = w1; i < this->number_of_cities; i++) {
        children_1_ins.push_back(buffor_path_1[i]);
        children_2_ins.push_back(buffor_path_2[i]);
    }

    for(int i = 0; children_1.size() != this->number_of_cities; i++) {
        children_1.insert(children_1.begin(), children_1_ins[i]);
        children_2.insert(children_2.begin(), children_2_ins[i]);
    }
    reverse(children_1.begin(), children_1.begin() + k1);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100);

    int r = distr(gen);
    if (r <= this->mutation_propability) {
        int first_swap = rand() % this->number_of_cities;
        int second_swap = rand() % this->number_of_cities;
        swap(children_1[first_swap], children_1[second_swap]);
        int x = rand()%3;
        if(x == 0) {
        }
        else if (x == 1){
            reverse(children_1.begin(), children_1.end());
            reverse(children_2.begin(), children_2.end());
        }
        else {
            random_shuffle(children_1.begin(), children_1.end());
            random_shuffle(children_2.begin(), children_2.end());
        }

    }
    if(this->population_size % 2 != 0 && this->paths.size() == 2*this->population_size - 1) {
        this->paths.push_back(children_1);
    }
    else {
        this->paths.push_back(children_1);
        this->paths.push_back(children_2);
    }
}

void TSP::final_step() {
    target_function();
    vector<pair<long double, int>> sorted_values = sorting_unique();
    vector<vector<int>> buffor_path;
    while(buffor_path.size() != this->population_size) {
        buffor_path.push_back(this->paths[sorted_values.front().second]);
        sorted_values.erase(sorted_values.begin());
    }
    this->paths = buffor_path;
    this->target_function();
}

vector<pair<long double,int>> TSP::sorting_unique() {
    vector<int> best_indexes;
    priority_queue<pair<int, double>, vector<pair<int, double>>, myComp> sorted_values_que;
    set<int> sorted_values_set;
    vector<int> sorted_values;
    unordered_map<long double, int> value_index;
    vector<pair<int, long double>> v = sorting();

    for(int i = 0; i < this->paths.size(); i++) {
        value_index[v[i].second] = v[i].first;
    }

    std::vector<std::pair<long double, int>> elems(value_index.begin(), value_index.end());
    std::sort(elems.begin(), elems.end());
    reverse(elems.begin(), elems.end());
    return elems;
}

vector<pair<int, long double>> TSP::sorting() {
    vector<int> best_indexes;
    priority_queue<pair<int, double>, vector<pair<int, double>>, myComp> sorted_values_que;
    vector<pair<int, long double>> sorted_values;

    for (int i = 0; i < this->paths.size(); i++) {
        sorted_values_que.push(make_pair(i, this->values[i]));
    }

    for (int i = 0; i < this->paths.size(); i++) {
        sorted_values.push_back(sorted_values_que.top());
        sorted_values_que.pop();
    }
    return sorted_values;
}
void TSP::start() {
    init();
    for(int i = 0; i < 1000; i++) {
        display_population();
        target_function();
        display_evaluations();
        select_and_cross();
        final_step();
    }
}

void TSP::selecting_parents_1() {
    vector<pair<int, long double>> sorted_values = sorting();

    double *probs = new double[this->population_size];
    for (int i = 0; i < this->population_size; i++) {
        probs[i] = 2 - 2 + (2 * (2 - 1)) * (this->population_size - i - 1)/(this->population_size - 1);
    }
    double lower_bound = 0;
    double upper_bound = 2;
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::default_random_engine re;
    double r = unif(re);
    vector<int> candidates;
    for(int i = 0; i < this->population_size; i++) {
        if(probs[i] >= r) candidates.push_back(sorted_values[i].first);
    }
    while(candidates.size() > 1) {
        int index_1 = -1;
        int index_2 = -1;
        while(index_1 == index_2) {
            index_1 = rand() % candidates.size();
            index_2 = rand() % candidates.size();
        }
        int val1= candidates[index_1];
        int val2 = candidates[index_2];
        this->parents.push_back(make_pair(candidates[index_1], candidates[index_2]));
        candidates.erase( remove (candidates.begin(), candidates.end(), val1), candidates.end() );
        candidates.erase( remove (candidates.begin(), candidates.end(), val2), candidates.end() );
        crossing();
    }
    this->parents.clear();
}

void TSP::selecting_parents_2() {
    vector<pair<int, long double>> sorted_values = sorting();
    double *probs = new double[this->population_size];
    probs[0] = sorted_values[0].second;
    for (int i = 1; i < this->population_size; i++) {
        probs[i] = probs[i - 1] + sorted_values[i].second;
    }

    double lower_bound = 0;
    double upper_bound = probs[this->population_size - 1];
    std::uniform_real_distribution<double> unif(lower_bound, upper_bound);
    std::default_random_engine re;

    while (this->paths.size() != 2 * this->population_size) {
        double r = unif(re);
        int i = 0;
        int pick_first;
        int pick_second;

        while (true) {
            if (r <= probs[i]) {
                pick_first = sorted_values[i].first;
                break;
            } else i = (i + 1) % this->population_size;
        }

        i = 0;
        r = unif(re);

        while (true) {
            if (r <= probs[i] && sorted_values[i].first != pick_first) {
                pick_second = sorted_values[i].first;
                break;
            } else i = (i + 1) % this->population_size;
        }
        this->parents.push_back(make_pair(pick_first, pick_second));
        crossing();
    }
    this->parents.clear();
}















