
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "max_k_cover.h"
#include "old_cover.h"

#include <ctime>
#include <cstdlib>

#include "generateData.h"

int k = 15; 
double epsilon = 0.13;

int main() {
    srand(time(NULL));

    std::unordered_map<int, std::unordered_set<int>> data;
    int theta = DataExtractor::extract("sets.txt", data);

    MaxKCoverEngine engine(k);
    engine.useLazyGreedy(data);

    std::cout << "data size: " << data.size() << ", theta: " << theta << std::endl;

    std::pair<std::vector<unsigned int>, int> res = engine.run_max_k_cover(data, theta);

    for (const auto & i : res.first) {
        std::cout << i << std::endl;
    }

    // OldEngine oe;
    // std::pair<std::vector<unsigned int>, int> res2 = oe.max_cover(data, k, theta);

    // std::cout << "verifying..." << std::endl;

    // for (const auto & i : res2.first) {
    //     std::cout << i << std::endl;
    // }
}