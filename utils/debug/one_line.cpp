#include "debug.h"

namespace SNet
{
namespace DEBUG
{
    std::string one_line(const std::string& str)
    {
        std::string ret;
        for (int i = 0; i < str.size(); i++)
        {
            if(str[i] == '\r' || str[i] == '\n')
            {
                ret.push_back('\\');
                ret.push_back(str[i] == '\r' ? 'r' : 'n');
            }
            else
            {
                ret.push_back(str[i]);
            }
        }
        return ret;
    }
}
}
