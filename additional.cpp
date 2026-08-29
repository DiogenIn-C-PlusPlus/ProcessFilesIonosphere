#include "additional.h"

void PrintVec(const std::vector<double>& vec, std::ostream& out)
{
    std::vector<double> temp_vec = vec;
    std::sort(temp_vec.begin(), temp_vec.end());
    for(size_t i = 0; i < temp_vec.size(); ++i)
    {
        out <<  temp_vec[i] << " ";
    }
    out  << std::endl;
}

void TranslateSymbolToZero(std::string& line, char symbol)
{
  for(char& c : line)
    {
      if(c == symbol)
        {
          c = 48;
        }
    }
}
