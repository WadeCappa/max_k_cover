#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <mutex>
#include <iostream>
#include "bitmask.h"
#include <utility>
#include <queue>
#include <cstdlib>
#include <bits/stdc++.h>
#include <cmath>

class MaxKCoverEngine 
{
private:
    template <typename idTy>
    struct CompareMaxHeap {

        bool operator()(const std::pair<idTy, std::unordered_set<idTy>*> a,
                        const std::pair<idTy, std::unordered_set<idTy>*> b) {
            return a.second->size() < b.second->size();
        }
    };

    class NextMostInfluentialFinder
    {
    protected:
        std::vector<unsigned int>* vertex_subset;

    public:
        virtual int findNextInfluential(
            std::vector<unsigned int>& seedSet,
            int current_K_index,
            std::unordered_map<int, std::unordered_set<int>>& selection_sets,
            ripples::Bitmask<int>& covered,
            int theta
        ) = 0;

        virtual NextMostInfluentialFinder* setSubset(std::vector<unsigned int>* subset_of_selection_sets) = 0;
    };

    class LazyGreedy : public NextMostInfluentialFinder
    {
    private:
        std::priority_queue<std::pair<int, std::unordered_set<int>*>, 
                std::vector<std::pair<int, std::unordered_set<int>*>>, 
                CompareMaxHeap<int>>* pq;
        std::unordered_map<int, std::unordered_set<int>*>* allSets;

    public:
        LazyGreedy(std::unordered_map<int, std::unordered_set<int>>& data)
        {
            this->allSets = new std::unordered_map<int, std::unordered_set<int>*>();

            for (const auto & l : data)
            {
                this->allSets->insert({ l.first, new std::unordered_set<int>(l.second.begin(), l.second.end()) });
            }
        }

        NextMostInfluentialFinder* setSubset(std::vector<unsigned int>* subset_of_selection_sets) override
        {
            CompareMaxHeap<int> cmp;
            std::vector<std::pair<int, std::unordered_set<int>*>> data_vec;
            for (const auto & v : *(subset_of_selection_sets))
            {
                data_vec.push_back(std::make_pair(v, this->allSets->at(v)));
            }

            if (this->pq != nullptr)
            {
                delete pq;
            }

            this->pq = new std::priority_queue<std::pair<int, std::unordered_set<int>*>,
                                std::vector<std::pair<int, std::unordered_set<int>*>>,
                                decltype(cmp)> (data_vec.begin(), data_vec.end());

            return this;
        }

        ~LazyGreedy()
        {
            delete allSets;
            delete pq;
        }

        int findNextInfluential(
            std::vector<unsigned int>& seedSet,
            int current_K_index,
            std::unordered_map<int, std::unordered_set<int>>& selection_sets,
            ripples::Bitmask<int>& covered,
            int theta
        ) override
        {
            int totalCovered;
            std::pair<int, std::unordered_set<int>*> l = this->pq->top();
            this->pq->pop();       

            std::unordered_set<int> temp;

            // remove RR IDs from l that are already covered. 
            for (int e: *(l.second)) {
                if (e > theta || e < 0) {
                    std::cout << "e is greater than theta, e = " << e << " , theta = " << theta << std::endl;
                }
                else if (covered.get(e)) {
                    temp.insert(e);
                }
            }            

            for (const int e : temp) 
            {
                l.second->erase(e); 
            }
            
            // calculate marginal gain
            auto marginal_gain = l.second->size();

            // calculate utiluty of next best
            auto r = this->pq->top(); 
            
            // if marginal of l is better than r's utility, l is the current best     
            if (marginal_gain >= r.second->size()) {
                seedSet[current_K_index] = l.first;
                
                for (int e : *(l.second)) {
                    if (e > theta || e < 0) {
                        std::cout << "e is greater than theta, e = " << e << " , theta = " << theta << std::endl;
                    }
                    else if (!covered.get(e)) {
                        totalCovered++;
                        covered.set(e);
                    }
                }
            }
            // else push l's marginal into the heap 
            else {
                this->pq->push(l);  
                return findNextInfluential(
                    seedSet, current_K_index, selection_sets, covered, theta
                );
            }

            return totalCovered;
        }
    };

    class NaiveGreedy : public NextMostInfluentialFinder
    {
    public:
        NaiveGreedy(std::unordered_map<int, std::unordered_set<int>>& data) {}

        NextMostInfluentialFinder* setSubset(std::vector<unsigned int>* subset_of_selection_sets) override
        {
            this->vertex_subset = subset_of_selection_sets;
            return this;
        } 

        int findNextInfluential(
            std::vector<unsigned int>& seedSet,
            int current_K_index,
            std::unordered_map<int, std::unordered_set<int>>& selection_sets,
            ripples::Bitmask<int>& covered,
            int theta
        ) override
        {
            int max = 0;
            int max_key = -1;
            int totalCovered = 0;

            for ( const auto & vertex : *this->vertex_subset )
            {
                if (selection_sets[vertex].size() > max)
                {
                    max = selection_sets[vertex].size();
                    max_key = vertex;
                }
            }
            seedSet[current_K_index] = max_key;

            for (int e: selection_sets[max_key]) {
                if (!covered.get(e)) {
                    covered.set(e);
                    totalCovered++;
                }
            }

            # pragma omp parellel for
            for( int i = 0; i < this->vertex_subset->size(); i++ ) {
                if (selection_sets.find(this->vertex_subset->at(i)) != selection_sets.end()) 
                {
                    auto RRRSets = selection_sets[this->vertex_subset->at(i)];

                    std::set<int> temp;
                    if (this->vertex_subset->at(i) != max_key) {
                        for (int e: RRRSets) {
                            if (covered.get(e)) {
                                temp.insert(e);
                            }
                        }
                        for (int e: temp) {
                            selection_sets[this->vertex_subset->at(i)].erase(e); 
                        }

                    }
                }
            }

            selection_sets.erase(max_key);
            return totalCovered;
        }
    };

    int k;
    double epsilon;
    bool usingStochastic = false;
    std::vector<unsigned int>* vertices;
    NextMostInfluentialFinder* finder;

    void generateSubset(std::vector<unsigned int>* vertices, size_t size, std::vector<unsigned int> seedSet)
    {
        // for i from 0 to n−2 do
        //     j ← random integer such that i ≤ j < n
        //     exchange a[i] and a[j]
        std::unordered_set<int> seeds(seedSet.begin(), seedSet.end());

        for (int i = 0; i < size; i++) 
        {
            int j = getRandom(i, vertices->size()-1);
            while (seeds.find(vertices->at(j)) != seeds.end())
            {
                j = getRandom(i, vertices->size()-1);
            }
            std::swap(vertices->at(i), vertices->at(j));
        }
    }

    int getRandom(int min, int max)
    {
        return min + (rand() % static_cast<int>(max - min + 1));
    }

    size_t getSubsetSize(size_t n, int k, double epsilon)
    {
        // new set R = (n/k)*log(1/epsilon),
        return ((size_t)std::round((double)n/(double)k) * std::log10(1/epsilon)) + 1;
    }

public:
    MaxKCoverEngine(int k) 
    {
        this->k = k;
    };

    ~MaxKCoverEngine() {
        if (this->finder != nullptr)
            delete this->finder;

        if (this->vertices != nullptr)
            delete this->vertices;
    }

    MaxKCoverEngine* useStochasticGreedy(double e)
    {
        this->epsilon = e;
        this->usingStochastic = true;
        return this;
    }

    MaxKCoverEngine* useLazyGreedy(std::unordered_map<int, std::unordered_set<int>>& data)
    {
        finder = new LazyGreedy(data);

        return this;
    }

    MaxKCoverEngine* useNaiveGreedy(std::unordered_map<int, std::unordered_set<int>>& data)
    {
        this->vertices = new std::vector<unsigned int>();
        for( const auto& t : data ) { this->vertices->push_back(t.first); }

        this->finder = new NaiveGreedy(data);

        return this;
    }

    // TODO: This should not mutate the data object. It should make a copy so that the original sets can be sent to the global node. Otherwise the algorithm breaks.
    std::pair<std::vector<unsigned int>, int> run_max_k_cover(std::unordered_map<int, std::unordered_set<int>> data, int theta)
    {
        std::vector<unsigned int> res(this->k, -1);
        ripples::Bitmask<int> covered(theta);

        std::vector<unsigned int>* all_vertices = new std::vector<unsigned int>();  
        for (const auto & l : data) { all_vertices->push_back(l.first); }
        std::vector<unsigned int>* current_subset = new std::vector<unsigned int>(all_vertices->begin(), all_vertices->end());
        this->finder->setSubset(current_subset);

        int uniqueCounted = 0;
        for (int currentSeed = 0; currentSeed < k; currentSeed++)
        {
            if (this->usingStochastic)
            {
                size_t subset_size = this->getSubsetSize(data.size(), this->k, this->epsilon);
                generateSubset(all_vertices, subset_size, res);
                delete current_subset;
                current_subset = new std::vector<unsigned int>(all_vertices->begin(), all_vertices->begin() + subset_size);
                this->finder->setSubset(current_subset);
            }

            uniqueCounted += finder->findNextInfluential(
                res, currentSeed, data, covered, theta
            );
        }
        
        delete all_vertices;
        delete current_subset;
        return std::make_pair(res, uniqueCounted);
    }
};