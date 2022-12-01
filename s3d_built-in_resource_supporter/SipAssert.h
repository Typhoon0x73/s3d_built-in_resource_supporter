#pragma once
#ifndef SIP_ASSERT_H_
#define SIP_ASSERT_H_

#include <Siv3D.hpp>

#ifdef _WINDOWS
#include <Windows.h>
#include <consoleapi2.h>
#include <consoleapi3.h>
#endif

namespace sip
{

#ifdef _WINDOWS

	static inline void ChangeConsoleColorRed(void)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	}

	static inline void ChangeConsoleColorWhite(void)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}

	static inline void ChangeConsoleColorYellow(void)
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	}

	static inline int soft_assert(const char* filename, int line)
	{
		ChangeConsoleColorRed();
		String fmt = U"#################### !! ASSERT !! ####################\n"
					 U" file : {} ( line : {} )"_fmt(Unicode::Widen(filename), line);
		Console << fmt;
		ChangeConsoleColorWhite();
		__debugbreak();
		return 1;
	}

#else

	static inline int soft_assert(const char*, int)
	{
		String fmt = U"#################### !! ASSERT !! ####################\n"
					 U" file : {} ( line : {} )"_fmt(Unicode::Widen(filename), line);
		Console << fmt;
		for (;;) { NULL; }
		return 1;
	}

#endif

}

#ifdef _DEBUG
#define SASSERT(b) (!!(b) || (soft_assert(__FILE__, __LINE__)))
#else //_DEBUG
#define SASSERT(b) (!!(b))
#endif //_DEBUG

#endif // !SIP_ASSERT_H_
