
# include <iostream>
# include <map>
# include <set>
# include <vector>
#include <cstdlib>
# include <ctime>
# include <cstdlib>

# include "max_k_cover.h"
# include "old_cover.h"

# include "generateData.h"
# include "timer.h"


std::pair<std::vector<unsigned int>, int> runTrial(MaxKCoverEngine& e, std::map<int, std::vector<int>> sets, int theta)
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

    std::map<int, std::vector<int>> data;
    std::pair<int, int> meta_data = DataExtractor::extract(filename, data);
    
    int theta = meta_data.first;
    int totalRRRSets = meta_data.second;

    std::cout << "number of vertices: " << data.size() << std::endl;
    std::cout << "number of sets over all vertex sets: " << totalRRRSets << std::endl;
    std::cout << "theta: " << theta << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "epsilon: " << epsilon << std::endl;

    std::cout << "lazy" << std::endl;
    MaxKCoverEngine lazyGreedy(k);
    lazyGreedy.useLazyGreedy(data);
    runTrial(lazyGreedy, data, theta);

    std::cout << "lazy-stochastic" << std::endl;
    MaxKCoverEngine stochasticLazyGreedy(k);
    stochasticLazyGreedy.useLazyGreedy(data).useStochasticGreedy(epsilon);
    runTrial(stochasticLazyGreedy, data, theta);
}