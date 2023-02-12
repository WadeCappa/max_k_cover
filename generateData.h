# include <unordered_map>
# include <unordered_set>
# include <string>
# include <iostream>
# include <fstream>
# include <sstream>

class DataExtractor
{
    private:
    static std::pair<int,int> insertIntegers(std::string line, std::unordered_set<int>& set)
    {
        std::stringstream ss(line);
        int max = 0;
        int total_sets = 0;
    
        std::string temp;
        int found;
        while (!ss.eof()) 
        {
            ss >> temp;
    
            if (std::stringstream(temp) >> found)
            {
                set.insert(found);
                max = std::max(found, max);
                total_sets++;
            }
    
            temp = "";
        }

        return std::make_pair(max, total_sets);
    }

    public:
    static std::pair<int,int> extract(std::string filename, std::unordered_map<int, std::unordered_set<int>>& data)
    {
        std::ifstream infile(filename);
        std::string line;

        int vertexID = 0;
        int theta = 0;
        int total_sets = 0;

        while (std::getline(infile, line))
        {
            std::pair<int, int> res = DataExtractor::insertIntegers(line, data[vertexID++]);
            theta = std::max(res.first, theta);
            total_sets += res.second;
        }

        infile.close();
        return std::make_pair(theta + 1, total_sets);
    }
};  