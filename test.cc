
# include <iostream>
# include <unordered_map>
# include <unordered_set>
# include <vector>

# include "max_k_cover.h"
# include "old_cover.h"

# include <ctime>
# include <cstdlib>

# include "generateData.h"
# include "timer.h"

int k = 100; 
double epsilon = 0.13;


double runTrial(MaxKCoverEngine e, std::unordered_map<int, std::unordered_set<int>> sets, int theta)
{
    Timer t;

    t.startTimer();
    std::pair<std::vector<unsigned int>, int> res = e.run_max_k_cover(sets, theta);
    t.endTimer();

    for (const auto & i : res.first) {
        std::cout << i << ", ";
    }
    std::cout << std::endl;

    return t.resolveTimer();
}


int main() {
    srand(time(NULL));

    std::unordered_map<int, std::unordered_set<int>> data;
    int theta = DataExtractor::extract("sets.txt", data);

    std::cout << "number of vertices: " << data.size() << std::endl;
    std::cout << "theta: " << theta << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "epsilon: " << epsilon << std::endl;

    std::cout << "Naive Greedy: " << std::endl;
    MaxKCoverEngine naiveEngine(k);
    naiveEngine.useNaiveGreedy(data);
    std::cout << "Runtime: " << runTrial(naiveEngine, data, theta) << " ms" << std::endl;

    std::cout << "Lazy Greedy: " << std::endl;
    MaxKCoverEngine lazyEngine(k);
    lazyEngine.useLazyGreedy(data);
    std::cout << "Runtime: " << runTrial(lazyEngine, data, theta) << " ms" << std::endl;

    std::cout << "Stochastic Naive Greedy: " << std::endl;
    MaxKCoverEngine stochasticNaiveEngine(k);
    stochasticNaiveEngine.useNaiveGreedy(data)->useStochasticGreedy(epsilon);
    std::cout << "Runtime: " << runTrial(stochasticNaiveEngine, data, theta) << " ms" << std::endl;

    std::cout << "Stochastic Lazy Greedy: " << std::endl;
    MaxKCoverEngine stochasticLazyEngine(k);
    stochasticLazyEngine.useLazyGreedy(data);
    std::cout << "Runtime: " << runTrial(stochasticLazyEngine, data, theta) << " ms" << std::endl;
}