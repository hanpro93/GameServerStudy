#pragma once


/*-----------
	crash
-----------*/
#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(nullptr != crash);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)					\
{											\
	if (!(expr))							\
	{										\
		CRASH("ASSERT_CRASH");				\
		__analysis_assume(expr);			\
	}										\
}