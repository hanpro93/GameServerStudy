#include "pch.h"
#include "Lock.h"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	const uint32 lockThreadID = ((_lockFlag.load() & WRITE_THREAD_MASK) >> 16);
	if (LThreadID == lockThreadID)
	{
		++_writeCount;
		return;
	}

	const int64		beginTick	= ::GetTickCount64();
	const uint32	desired		= ((LThreadID << 16) & WRITE_THREAD_MASK);

	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = EMPTY_FLAG;
			if (true == _lockFlag.compare_exchange_weak(OUT expected, desired))
			{
				++_writeCount;
				return;
			}
		}

		if (ACQUIRE_TIME_TICK <= ::GetTickCount64() - beginTick)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	if (0 != (_lockFlag.load() & READ_COUNT_MASK))
		CRASH("INVALID_UNLOCK_ORDER")
	{
	}

	const int32 lockCount = --_writeCount;
	if (0 == lockCount)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	const uint32 lockThreadID = ((_lockFlag.load() & WRITE_THREAD_MASK) >> 16);
	if (LThreadID == lockThreadID)
	{
		++_writeCount;
		return;
	}

	const int64	beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (true == _lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}
	}

	if (ACQUIRE_TIME_TICK <= ::GetTickCount64() - beginTick)
		CRASH("LOCK_TIMEOUT");

	this_thread::yield();
}

void Lock::ReadUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	if (0 == (_lockFlag.fetch_sub(1) & READ_COUNT_MASK))
		CRASH("MULTIPLE_UNLOCK")
}
