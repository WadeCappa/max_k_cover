
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


std::pair<std::vector<unsigned int>, int> runTrial(MaxKCoverEngine& e, std::unordered_map<int, std::unordered_set<int>> sets, int theta)
{
    Timer t;

    t.startTimer();
    std::pair<std::vector<unsigned int>, int> res = e.run_max_k_cover(sets, theta);
    t.endTimer();

    for (const auto & i : res.first) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    std::cout << res.second << std::endl;

    std::cout << t.resolveTimer() << " ms" << std::endl;

    return res;
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int k = atoi(argv[1]);
    double epsilon = std::atof(argv[2]);
    std::string filename = argv[3];

    std::string save_output_file;

    std::unordered_map<int, std::unordered_set<int>> data;
    std::pair<int, int> meta_data = DataExtractor::extract(filename, data);
    
    int theta = meta_data.first;
    int totalRRRSets = meta_data.second;

    std::cout << "number of vertices: " << data.size() << std::endl;
    std::cout << "number of sets over all vertex sets: " << totalRRRSets << std::endl;
    std::cout << "theta: " << theta << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "epsilon: " << epsilon << std::endl;

    // MaxKCoverEngine naiveGreedy(k);
    // naiveGreedy.useNaiveGreedy(data);
    // runTrial(naiveGreedy, data, theta);

    // OldEngine oldEngine;    
    // Timer t;

    // t.startTimer();
    // auto res = oldEngine.max_cover(data, k, theta);
    // t.endTimer();

    // for (const auto & i : res.first) {
    //     std::cout << i << ", ";
    // }
    // std::cout << std::endl;
    
    // std::cout << res.second << std::endl;
    // std::cout << t.resolveTimer() << " ms" << std::endl;

    std::cout << "lazy" << std::endl;
    MaxKCoverEngine lazyGreedy(k);
    lazyGreedy.useLazyGreedy(data);
    runTrial(lazyGreedy, data, theta);

    // OldEngine lazyEngine;    
    // Timer t2;

    // t2.startTimer();
    // auto res2 = lazyEngine.max_cover_lazy_greedy(data, k, theta);
    // t2.endTimer();

    // for (const auto & i : res2.first) {
    //     std::cout << i << ", ";
    // }
    // std::cout << std::endl;
    
    // std::cout << res2.second << std::endl;
    // std::cout << t2.resolveTimer() << " ms" << std::endl;

    // MaxKCoverEngine stochasticNaive(k);
    // stochasticNaive.useStochasticGreedy(epsilon)->useNaiveGreedy(data);
    // runTrial(stochasticNaive, data, theta);

    // std::cout << "lazy stochastic" << std::endl;
    // MaxKCoverEngine stochasticLazy(k);
    // stochasticLazy.useStochasticGreedy(epsilon)->useLazyGreedy(data);
    // runTrial(stochasticLazy, data, theta);

    // std::cout << "naive bitmap" << std::endl;
    // MaxKCoverEngine bitmapNaive(k);
    // bitmapNaive.useNaiveBitmapGreedy(data, theta);
    // runTrial(bitmapNaive, data, theta);
}