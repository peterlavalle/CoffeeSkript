


#include <duktape.hpp>
#include <coffee.hpp>
#include <list>
#include <string>

#include "coffeescript-test.hpp"

#include "pal_base.h"

extern const char* coffeescript_test_2_wrong; // if this symbol is undefined; we're linking the wrong tin.c.o file

TEST_CASE( "test cascaded", "[coffee]" )
{
	// create a vm
		duk::vm vm;
	// SECTION( "install coffee-script" )
	{
		tin("coffeescript-1.12.8.js", [&](const char* name, const size_t size, const void* data)
		{
			coffee::install(vm, size, data);
		});
	}

	std::list<std::string> logged;
	// SECTION( "create log object" )
	{
		const auto top = duk_get_top(vm);
		vm.object_push<std::list<std::string>>(&logged, [](duk::vm&, std::list<std::string>& logged)
		{
			// there's nothing to do here
		});
		vm.method_put<std::list<std::string>>("out", 1, [](duk::vm& ctx, std::list<std::string>& logged)
		{
			const auto top = duk_get_top(ctx);
			
			const auto str = duk_to_string(ctx, 0);

			static const std::string wrong = coffeescript_test_2_wrong;
			// TODO("Why can't I read from the global variable!?");

			if (wrong == str)
				FAIL("Wrong string detected in callback!");

			logged.push_back(duk_to_string(ctx, 0));
			return 0;
		});
		duk_put_global_string(vm, "log");
		REQUIRE( duk_get_top(vm) == top );
	}

	// SECTION( "load the coffee-script" )
	{
		tin("script.coffee",[&](const char* name, const size_t size, const void* data)
		{
			// push the coffee-script source
			duk_push_lstring(vm, (const char*) data, size);

			// push the name
			duk_push_string(vm, name);

			// run the compile
			coffee::compile(vm);
		});
	}

	// SECTION( "invoke the coffee-script" )
	{
		// right ... run it
		if (duk_pcall(vm, 0)) 
		{
			std::cout << "err = " << duk_safe_to_string(vm,-1) << std::endl;
			FAIL( duk_safe_to_string(vm,-1) );
		}
	}

	// SECTION( "validate the results" )
	{
		REQUIRE( 1 == logged.size() );

		REQUIRE( *(logged.begin()) == "this is the coffeescript-test-2 script" );
	}
}




