# include <vector>
# include <utility>
# include <chrono>

class Timer 
{
    private:
    std::vector<std::chrono::duration<double, std::milli>> timer;
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point end;

    public:
    Timer(){}
    ~Timer(){}

    void startTimer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    void endTimer()
    {
        end = std::chrono::high_resolution_clock::now();
        timer.push_back(end - start);
    }

    double resolveTimerDEBUG()
    {
        double totalTime = 0;
        for (const auto time : timer)
        {
            totalTime += time.count();
        }
        return totalTime;
    }

    double resolveTimer()
    {
        double totalTime = 0;
        for (const auto time : timer)
        {
            totalTime += time.count();
        }
        return totalTime;
    }
};