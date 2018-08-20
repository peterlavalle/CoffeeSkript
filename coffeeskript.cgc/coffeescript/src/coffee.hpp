
#pragma once

#include "duktape.h"
#include "duktape.hpp"

namespace coffee
{
	void compile(duk_context*);
	void install(duk_context*, const size_t, const void*);
}
