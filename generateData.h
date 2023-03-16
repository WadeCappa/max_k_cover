# include <map>
# include <set>
# include <string>
# include <iostream>
# include <fstream>
# include <sstream>

class DataExtractor
{
    private:
    static std::pair<int,int> insertIntegers(std::string line, std::vector<int>& set)
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
                set.push_back(found);
                max = std::max(found, max);
                total_sets++;
            }
    
            temp = "";
        }

        return std::make_pair(max, total_sets);
    }

    static int getMaxTheta(std::string line)
    {
        std::stringstream ss(line);
        int max = 0;
    
        std::string temp;
        int found;
        while (!ss.eof()) 
        {
            ss >> temp;
    
            if (std::stringstream(temp) >> found)
            {
                max = std::max(found, max);
            }
    
            temp = "";
        }

        return max;
    }

    public:
    static std::pair<int,int> getMetaData(std::string filename)
    {
        std::ifstream infile(filename);
        std::string line;

        int line_count = 0;
        int theta = 0;

        while (std::getline(infile, line))
        {
            theta = std::max(theta, DataExtractor::getMaxTheta(line));
            line_count++;
        }

        infile.close();
        return std::make_pair(line_count, theta + 1);
    }

    static std::pair<int,int> extract(std::string filename, std::map<int, std::vector<int>>& data)
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

    static std::pair<int,int> extract_subset(
        std::string filename, 
        std::map<int, std::vector<int>>& data,
        std::function<bool(int)> filter_function
    )
    {
        std::ifstream infile(filename);
        std::string line;

        int vertexID = 0;
        int theta = 0;
        int total_sets = 0;

        while (std::getline(infile, line))
        {
            if (filter_function(vertexID))
            {
                std::pair<int, int> res = DataExtractor::insertIntegers(line, data[vertexID]);
                theta = std::max(res.first, theta);
                total_sets += res.second;
            }

            vertexID++;
        }

        infile.close();
        return std::make_pair(theta + 1, total_sets);
    }
};  