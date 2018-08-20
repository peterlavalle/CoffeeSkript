
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




// https://github.com/catchorg/Catch2/blob/master/docs/tutorial.md#top

const char path_to_script[] = "coffeescript-test-2/src/script.coffee";


TEST_CASE( "script.coffee sizes coded match decompressed", "[tin]" )
{
	bool touched = false;
	tin("script.coffee", &touched, [](void* self, const char* name, const size_t size, const void* data)
	{
		REQUIRE(nullptr != self);
		(*reinterpret_cast<bool*>(self)) = true;
		REQUIRE(std::string("script.coffee") == name);
		REQUIRE(131 == size);
		REQUIRE(nullptr != data);
	});

	REQUIRE(touched);
}

TEST_CASE( "script.coffee sizes coded match file", "[tin]" )
{
	const auto expected = fstring(path_to_script);

	REQUIRE(131 == expected.size());
}



TEST_CASE( "script.coffee text matches", "[tin]" )
{
	bool touched = false;
	tin("script.coffee", &touched, [](void* self, const char* name, const size_t size, const void* data)
	{
		const auto expected = fstring(path_to_script);
		const auto actual = std::string((const char*)data, size);

		REQUIRE(nullptr != self);
		(*reinterpret_cast<bool*>(self)) = true;
		REQUIRE(expected == actual);
	});
	REQUIRE(touched);
}
