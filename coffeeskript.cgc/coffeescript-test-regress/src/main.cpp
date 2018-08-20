
#include <duktape.hpp>
#include <coffee.hpp>
#include <iostream>
#include "tin.h"
#include "catch.hpp"

bool strBeginsWith(const char* pre, const char* str)
{
	const size_t lenpre = strlen(pre);
	const size_t lenstr = strlen(str);

	return lenpre <= lenstr && !strncmp(pre, str, lenpre);
}

bool strClosedWith(const char* end, const char* str)
{
  const size_t lenend = strlen(end);
  const size_t lenstr = strlen(str);
	return lenend <= lenstr && !strcmp(end, str + (lenstr - lenend));
}


TEST_CASE( "open it", "[coffee]" )
{
	tin(nullptr, [](const char* name, const size_t size, const void* data)
	{
		if (strBeginsWith("coffeescript-", name))
			if (strClosedWith(".js", name))
				SECTION(name)
				{
					// create a duktape
					duk::vm vm;
					
					auto message = std::string("loaded coffeescript ") + name;
					
					INFO(message);
				}
	});
}


TEST_CASE( "eval it", "[coffee]" )
{
	tin(nullptr, [](const char* name, const size_t size, const void* data)
	{
		if (strBeginsWith("coffeescript-", name))
			if (strClosedWith(".js", name))
				SECTION(name)
				{
					// create a duktape
					duk::vm vm;

					if (!duk_peval_lstring(vm, (const char*) data, size))
						std::cout << "evaled " << name << std::endl;
					else
					{
						std::cout << "failed " << name << std::endl;
						auto message = ((std::string("eval failed for ") + name) + " with message: ") + duk_safe_to_string(vm, -1);
						FAIL(message);
					}
				}
	});
}


// TEST_CASE( "coffee::install it", "[coffee]" )
// {
// 	tin(nullptr, [](const char* name, const size_t size, const void* data)
// 	{
// 		SECTION(name)
// 		{ 
// 			FAIL("missing");

// 			// create a duktape
// 			duk::vm vm;

// 			// load and exec the coffeescript
// 		}
// 	});
// }
