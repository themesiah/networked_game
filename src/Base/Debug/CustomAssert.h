#ifndef _CUSTOM_ASSERT_H
#define _CUSTOM_ASSERT_H
#pragma once

#ifndef _FINAL

bool CustomAssertFunction(bool expr, int line, char* file, bool* ignoreAlways, const char* format, ...);

#define Assert(exp, description, ...) \
	{	static bool ignoreAlways = false; \
		if (!ignoreAlways) {\
			if (CustomAssertFunction(exp, __LINE__, __FILE__, &ignoreAlways, description, __VA_ARGS__)) {\
				_asm { int 3}\
			}\
		}\
	}


#else
#define Assert(exp, description)
#endif

#endif // _UAB_ASSERT_H