#pragma once
#include <string>
#include <fstream>

namespace Sys
{
	std::string hayphenate(std::string str);
	std::string readWholeFile(std::fstream* fs);
}
