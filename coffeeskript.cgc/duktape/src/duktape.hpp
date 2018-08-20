
#pragma once

#include <functional>
#include <typeinfo>

#include "duktape.h"

namespace duk
{
	/// kind of a smart-pointer for the ducktape vm
	class vm final
	{
		duk_context* _vm;
	public:
		vm(const vm&) = delete;
		void operator=(const vm&) = delete;

		static vm& wrap(duk_context*& duk);

		vm(void);

		~vm(void);

		void object_push(void* pointer, void(*cleanup)(vm&, void*), const char* kind = nullptr);
		
		void method_put(const char*, duk_int_t nargs, duk_ret_t (*method)(vm&, void*), const char* kind = nullptr);
		
		operator duk_context*(void) { return _vm; }

		template<typename T, typename ... ARGS>
		void object_new(ARGS&&...);

		template<typename T>
		void object_push(T* pointer, void(*cleanup)(vm&, T&))
		{
			object_push(
				reinterpret_cast<void*>(pointer),
				reinterpret_cast<void(*)(vm&, void*)>(cleanup),
				typeid(T).name());
		}

		template<typename T>
		void method_put(const char* name, duk_int_t nargs, duk_ret_t (*method)(vm&, T&))
		{
			method_put(
				name,
				nargs,
				reinterpret_cast<duk_ret_t(*)(vm&, void*)>(method),
				typeid(T).name());
		}

		// template<typename T>
		// void method_set(const char* name, duk_int_t nargs, std::function<duk_ret_t(vm&, T&)> lambda)
		// {
		// 	method_put(
		// 		name,
		// 		nargs,
		// 		reinterpret_cast<duk_ret_t(*)(vm&, void*)>(method),
		// 		typeid(T).name());
		// }
	};
}
