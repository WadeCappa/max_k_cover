
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

    std::cout << t.resolveTimer() << std::endl;

    return res;
}

std::pair<std::vector<unsigned int>, int> runNaive(std::unordered_map<int, std::unordered_set<int>> data, int theta, int k)
{
    std::cout << "Naive Greedy runtime: ";
    MaxKCoverEngine naiveEngine(k);
    naiveEngine.useNaiveGreedy(data);
    return runTrial(naiveEngine, data, theta);
}

std::pair<std::vector<unsigned int>, int> runLazy(std::unordered_map<int, std::unordered_set<int>> data, int theta, int k)
{
    std::cout << "Lazy Greedy runtime: ";
    MaxKCoverEngine lazyEngine(k);
    lazyEngine.useLazyGreedy(data);
    return runTrial(lazyEngine, data, theta);
}

std::pair<std::vector<unsigned int>, int> runNaiveStochastic(std::unordered_map<int, std::unordered_set<int>> data, int theta, int k, double epsilon)
{
    std::cout << "Stochastic Naive Greedy runtime ";
    MaxKCoverEngine stochasticNaiveEngine(k);
    stochasticNaiveEngine.useNaiveGreedy(data)->useStochasticGreedy(epsilon);
    return runTrial(stochasticNaiveEngine, data, theta);
}

std::pair<std::vector<unsigned int>, int> runLazyStochastic(std::unordered_map<int, std::unordered_set<int>> data, int theta, int k, double epsilon)
{
    std::cout << "Stochastic Lazy Greedy runtime: ";
    MaxKCoverEngine stochasticLazyEngine(k);
    stochasticLazyEngine.useLazyGreedy(data)->useStochasticGreedy(epsilon);
    return runTrial(stochasticLazyEngine, data, theta);
}

int main(int argc, char** argv) {
    srand(time(NULL));

    int k = atoi(argv[1]);
    double epsilon = std::atof(argv[2]);
    std::string filename = argv[3];

    std::unordered_map<int, std::unordered_set<int>> data;
    int theta = DataExtractor::extract(filename, data) + 1;

    std::cout << "number of vertices: " << data.size() << std::endl;
    std::cout << "theta: " << theta << std::endl;
    std::cout << "k: " << k << std::endl;
    std::cout << "epsilon: " << epsilon << std::endl;

    // std::cout << "data size:" << data.size() << std::endl;

    // runNaive(data, theta, k);
    // std::cout << "data size:" << data.size() << std::endl;

    // runLazy(data, theta, k);
    // std::cout << "data size:" << data.size() << std::endl;

    // runNaiveStochastic(data, theta, k, epsilon);
    // std::cout << "data size:" << data.size() << std::endl;

    // runLazyStochastic(data, theta, k, epsilon);
    // std::cout << "data size:" << data.size() << std::endl;

    MaxKCoverEngine engine(k);
    std::pair<std::vector<unsigned int>, ssize_t> res = engine.useStochasticGreedy(epsilon)->useLazyGreedy(data)->run_max_k_cover(data, theta);

    // for (const auto & i : res.first) {
    //     std::cout << i << ", ";
    // }
    // std::cout << std::endl;

    std::cout << res.second << std::endl;
}