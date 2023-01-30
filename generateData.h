# include <unordered_map>
# include <unordered_set>
# include <string>
# include <iostream>
# include <fstream>
# include <sstream>

class DataExtractor
{
    private:
    static int insertIntegers(std::string line, std::unordered_set<int>& set)
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
                set.insert(found);
                max = std::max(found, max);
            }
    
            temp = "";
        }

        return max;
    }

    public:
    static int extract(std::string filename, std::unordered_map<int, std::unordered_set<int>>& data)
    {
        std::ifstream infile(filename);
        std::string line;

        int vertexID = 0;
        int theta = 0;

        while (std::getline(infile, line))
        {
            theta = std::max(DataExtractor::insertIntegers(line, data[vertexID++]), theta);
        }

        infile.close();
        return theta;
    }
};  