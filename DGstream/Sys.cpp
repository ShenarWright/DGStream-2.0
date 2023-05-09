#include "Sys.h"

namespace Sys
{

    std::string hayphenate(std::string str)
    {
        for (int i = 0; i < str.size(); i++)
        {
            if (str[i] == ' ')
                str[i] = '-';
        }
        return str;
    }
}
