#pragma once
#ifndef SIP_ASSERT_H_
#define SIP_ASSERT_H_

#include <Siv3D.hpp>

namespace sip
{
	static inline int soft_assert(const char* filename, int line)
	{
		String fmt = U"\n#################### !! ASSERT !! ####################\n"
					 U" file : {} ( line : {} )\n"_fmt(Unicode::Widen(filename), line);
		Console << fmt;
		// 強制ブレイクポイント
#ifdef _WINDOWS
		__debugbreak();
#else //_WINDOWS
		for (;;) { NULL; }
#endif//_WINDOWS
		return 1;
	}
}

#ifdef _DEBUG
#define SASSERT(b) (!!(b) || (soft_assert(__FILE__, __LINE__)))
#else //_DEBUG
#define SASSERT(b) (!!(b))
#endif //_DEBUG

#endif // !SIP_ASSERT_H_
