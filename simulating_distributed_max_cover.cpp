
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


int main(int argc, char** argv) {
    srand(time(NULL));

    int k = atoi(argv[1]);
    double epsilon = std::atof(argv[2]);
    int m = atoi(argv[3]);
    std::string filename = argv[4];

    std::unordered_map<int, std::unordered_set<int>> data;
    std::pair<int, int> meta_data = DataExtractor::extract(filename, data);
    
    int theta = meta_data.first;
    int totalRRRSets = meta_data.second;

    std::cout << "number of vertices: " << data.size() << std::endl;
    std::cout << "number of sets over all vertex sets: " << totalRRRSets << std::endl;
    std::cout << "theta: " << theta << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "epsilon: " << epsilon << std::endl;

    // build subsets, do sequentially
    std::vector<std::unordered_map<int, std::unordered_set<int>>> data_subsets(m);
    for (const auto & s : data)
    {
        data_subsets[s.first % m].insert({s.first, *(new std::unordered_set<int>(s.second.begin(),s.second.end()))});
    }

    // do in parallel, run max_k_cover on all subsets to aquire solutions
    std::vector<std::vector<int>> results_from_subset_covers(m);

    # pragma omp parallel 
    {
        # pragma omp for
        for (int i = 0; i < m; i++) {
            MaxKCoverEngine cEngine(k);
            cEngine.useLazyGreedy(data_subsets[i]);
            std::pair<std::vector<unsigned int>, int> res = cEngine.run_max_k_cover(data_subsets[i], theta);
            results_from_subset_covers[i] = *(new std::vector<int>(res.first.begin(), res.first.end()));
        }
    }
    // // verify output if required. 
    // for (int i = 0; i < m; i++) {
    //     std::cout << "process i) ";
    //     for (const auto & r : results_from_subset_covers[i]) {
    //         std::cout << r << ", ";
    //     }   
    //     std::cout << std::endl;
    // }

    // build global seed set
    std::unordered_map<int, std::unordered_set<int>> global_data; 
    for (const auto & l : results_from_subset_covers) {
        for (const auto & s : l) {
            global_data[s] = *(new std::unordered_set<int>(data[s].begin(), data[s].end()));
        }
    }

    MaxKCoverEngine gEngine(k);
    gEngine.useLazyGreedy(global_data);
    std::pair<std::vector<unsigned int>, int> res = gEngine.run_max_k_cover(global_data, theta);

    for (const auto & s : res.first){
        std::cout << s << ", ";
    }
    std::cout << std::endl;
}