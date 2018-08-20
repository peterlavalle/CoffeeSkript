
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
 
#include "tin.h"
#include "catch.hpp"

std::string fstring(const std::string& path)
{
	auto file = fopen(path.c_str(), "rb");
	fseek(file, 0, SEEK_END);
	std::vector<char> data((size_t) ftell(file));
	fseek(file, 0, SEEK_SET);
	fread(data.data(), data.size(), 1, file);
	fclose(file);
	return std::string(data.data(), data.size());
}

#include "tinfl.h"
