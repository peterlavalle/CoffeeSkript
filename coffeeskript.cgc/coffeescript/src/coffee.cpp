
#include "tin.h"

#include "duktape.h"
#include "coffee.hpp"

#include <assert.h>

#include <iostream>

#include "pal_base.h"

// https://duktape.org/api.html
// https://coffeescript.org/#nodejs-usage
	

// void coffee_print_globals(duk_context* dvm)
// {
// 	const auto atop = duk_get_top(dvm);
// 	//duk_push_global_object(dvm);
// 	duk_enum(dvm, -1, 0);
// 	std::cout << ">>> table" << std::endl;
// 	while (duk_next(dvm, -1 /*enum_idx*/, 0 /*get_value*/))
// 	{
// 		std::cout << ">>> >>> key = `" << duk_get_string(dvm, -1) << "`" << std::endl;
// 		duk_pop(dvm);  
// 	}
// 	//duk_pop_2(dvm);  
// 	duk_pop(dvm);  
// 	assert( duk_get_top(dvm) == atop );
// 	std::cout << "<<< table" << std::endl;
// }

void coffee::install(duk_context* duk, const size_t len, const void* src)
{
	// https://duktape.org/api.html#duk_compile_lstring_filename
	// https://duktape.org/api.html#duk_compile
	// https://duktape.org/api.html#duk_push_lstring

	TODO("assert (durring debug) that there's no existing CoffeeScript installed");

	// compile it
	duk_push_string(duk, "coffeescript.js");
	duk_compile_lstring_filename(duk, DUK_COMPILE_EVAL, (const char*)src, len);

	// execute it
	duk_call(duk, 0);
	duk_pop(duk);
}

void coffee::compile(duk_context* dvm)
{
// 	stack has;
// 	- source
// 	- name

	// need to push compile function first
	duk_get_global_string(dvm, "CoffeeScript");
	assert(duk_is_object(dvm, -1));
	assert(duk_has_prop_string(dvm, -1, "compile"));

	duk_get_prop_string(dvm, -1, "compile");
	duk_remove(dvm, -2);

	assert(duk_is_function(dvm, -1));

	//
	TODO("perfrom a pCompile and include the script's name ... i think that needs a table as a parameter?");


	//

	// stack
	// 	- source.coffee
	// 	- name
	//	- compile()

	duk_dup(dvm, -3);
	duk_call(dvm, 1);

	// stack
	// 	- source.coffee
	// 	- name
	// 	- source.js

	duk_remove(dvm, -3);
	duk_swap_top(dvm, -2);

	// stack
	// 	- source.js
	// 	- name
	duk_compile(dvm, 0);
}
