
# include <iostream>
# include <unordered_map>
# include <unordered_set>
# include <vector>
#include <cstdlib>
# include <ctime>
# include <cstdlib>

# include "max_k_cover.h"
# include "old_cover.h"

# include "generateData.h"
# include "timer.h"

#include <cstdlib>


int main(int argc, char** argv) {
    srand(time(NULL));

    int k = atoi(argv[1]);
    double epsilon = std::atof(argv[2]);
    int m = atoi(argv[3]);
    std::string filename = argv[4];

    // std::pair<int,int> metaData = DataExtractor::getMetaData(filename);
    int number_of_lines = 3072441;
    int theta = 212288;

    srand((unsigned) time(NULL));

    std::vector<unsigned int> vertex_mapping(number_of_lines);
    for (auto & vertex_map : vertex_mapping)
    {
        vertex_map = (rand() % 55001) % m;
    }

    std::cout << "number of vertices: " << number_of_lines << std::endl;
    // std::cout << "number of sets over all vertex sets: " << totalRRRSets << std::endl;
    std::cout << "theta: " << theta << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "epsilon: " << epsilon << std::endl;

    // do in parallel, run max_k_cover on all subsets to aquire solutions
    std::vector<std::vector<unsigned int>> results_from_subset_covers(m);

    # pragma omp parallel 
    {
        # pragma omp for 
        for (int i = 0; i < m; i++) {
            std::map<int, std::vector<int>> data;
            auto filter_function = [&vertex_mapping, &i](int index){ return vertex_mapping[index] == i; };
            std::pair<int, int> meta_data = DataExtractor::extract_subset(filename, data, filter_function);

            MaxKCoverEngine cEngine(k);
            cEngine.useLazyGreedy(data);
            std::pair<std::vector<unsigned int>, int> res = cEngine.run_max_k_cover(data, theta);
            results_from_subset_covers[i] = *(new std::vector<unsigned int>(res.first.begin(), res.first.end()));
        }
    }

    std::unordered_set<unsigned int> allSeeds;
    for (const auto & ss : results_from_subset_covers)
    {
        for (const auto & s : ss) 
        {
            allSeeds.insert(s);
        }
    }

    std::map<int, std::vector<int>> global_data;
    auto filter_function = [&allSeeds](int index){ return allSeeds.find(index) != allSeeds.end(); };
    std::pair<int, int> meta_data = DataExtractor::extract_subset(filename, global_data, filter_function);

    MaxKCoverEngine gEngine(k);
    gEngine.useLazyGreedy(global_data);
    std::pair<std::vector<unsigned int>, int> res = gEngine.run_max_k_cover(global_data, theta);

    for (const auto & s : res.first){
        std::cout << s << ", ";
    }
    std::cout << std::endl;
}