
#ifndef pal_base_h
#define pal_base_h

	#ifdef _DEBUG
		#include <stdio.h>
		// pops out a message ONCE that matches my "problemMatcher"
		// ... which will show up nicely in checks
		#define TODO(NOTE) \
			do {\
			static int TODO = 1;\
			if (TODO)\
			{\
				TODO = 0;\
				fprintf(stderr, "TODO!" __FILE__ ":%d:0: warning: " NOTE "\n", __LINE__);\
			}\
			} while (false)

	#else

		// there's no TODO without debug

	#endif//_DEBUG

#endif//pal_base_h
