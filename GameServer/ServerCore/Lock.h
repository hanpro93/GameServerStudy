#pragma once
#include "Types.h"

/*-----------------
	RW SpinLock
-----------------*/

class Lock
{
	enum : uint32 
	{
		ACQUIRE_TIME_TICK	= 10000,
		MAX_SPIN_COUNT		= 50000,
		WRITE_THREAD_MASK	= 0xFFFF'0000,
		READ_COUNT_MASK		= 0x0000'FFFF,
		EMPTY_FLAG			= 0x0000'0000,
	};

public:
	void WriteLock();
	void WriteUnLock();
	void ReadLock();
	void ReadUnLock();


private:
	Atomic<uint32>	_lockFlag;
	uint16			_writeCount = 0;
};

/*----------------
	LockGuards
----------------*/

class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) {_lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnLock(); }

private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnLock(); }

private:
	Lock& _lock;
};
