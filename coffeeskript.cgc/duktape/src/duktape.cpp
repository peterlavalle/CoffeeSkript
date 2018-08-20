
#include "duktape.hpp"

#include <assert.h>

static const char duk_code[] = "\xff" "\xff" "code"; 
static const char duk_done[] = "\xff" "\xff" "done"; 
static const char duk_kind[] = "\xff" "\xff" "kind"; // redundant in object?
static const char duk_this[] = "\xff" "\xff" "this"; 

void duk::vm::object_push(void* pointer, void(*cleanup)(vm&, void*), const char* kind)
{
	const auto idx = duk_push_bare_object(_vm);

	duk_push_pointer(_vm, pointer);
	duk_put_prop_string(_vm, idx, duk_this);

	duk_push_pointer(_vm, reinterpret_cast<void*>(cleanup));
	duk_put_prop_string(_vm, idx, duk_done);

	duk_push_string(_vm, (nullptr == kind) ? typeid(void*).name() : kind);
	duk_put_prop_string(_vm, idx, duk_kind);

	duk_push_c_lightfunc(_vm, [](duk_context* ctx) -> duk_ret_t {

		duk_get_prop_string(ctx, 0, duk_this);
		void* self = duk_to_pointer(ctx, 1);

		duk_get_prop_string(ctx, 0, duk_done);
		auto code = reinterpret_cast<void(*)(vm&, void*)>(duk_to_pointer(ctx, 2));

		duk_pop_3(ctx);
		code(duk::vm::wrap(ctx), self);

		return 0;
	}, 1, 2, 0);
	duk_set_finalizer(_vm, idx);
}

duk::vm& duk::vm::wrap(duk_context*& duk)
{
	duk::vm& wrapped = *reinterpret_cast<duk::vm*>(&duk);

	assert(static_cast<duk_context*>(wrapped) == duk);

	return wrapped;
}

duk::vm::vm(void) :
	_vm(duk_create_heap_default())
{	
}

duk::vm::~vm(void)
{
	duk_destroy_heap(_vm);
}

void duk::vm::method_put(const char* name, duk_int_t nargs, duk_ret_t (*code)(duk::vm&, void*), const char* kind)
{
	duk_push_c_function(_vm, [](duk_context* ctx) -> duk_ret_t {

		duk_push_this(ctx);
		duk_get_prop_string(ctx, -1, duk_this);
		duk_get_prop_string(ctx, -2, duk_kind);
		auto this_self = duk_to_pointer(ctx, -2);
		auto this_kind = duk_to_string(ctx, -1);
		duk_pop_3(ctx);

		duk_push_current_function(ctx);
		duk_get_prop_string(ctx, -1, duk_code);
		duk_get_prop_string(ctx, -2, duk_kind);
		auto call_code = reinterpret_cast<duk_ret_t (*)(duk::vm&, void*)>(duk_to_pointer(ctx, -2));
		auto call_kind = duk_to_string(ctx, -1);
		duk_pop_3(ctx);

		assert(strcmp(this_kind, call_kind) || this_kind == call_kind);

		auto& wrap = duk::vm::wrap(ctx);
		assert(static_cast<duk_context*>(wrap) == ctx);

		return (this_kind != call_kind)
			? duk_error(ctx, 1983, "Tried to use a method on the wrong type of object")
			: call_code(wrap, this_self);
	}, nargs);

	// TODO ? set the name?

	duk_push_string(_vm, (nullptr == kind) ? typeid(void*).name() : kind);
	duk_put_prop_string(_vm, -2, duk_kind);

	duk_push_pointer(_vm, reinterpret_cast<void*>(code));
	duk_put_prop_string(_vm, -2, duk_code);

	duk_put_prop_string(_vm, -2, name);
}
		