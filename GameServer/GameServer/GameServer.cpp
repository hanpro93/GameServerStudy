#include "pch.h"
#include <thread>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		bool expected	= false;
		bool desired	= true;
		
		//CAS 의사코드
		/*
		if (expected == _locked)
		{
			expected	= _locked;
			_locked		= desired;

			return true;
		}
		else
		{
			expected = _locked;
			return false;
		}
		*/
		while (false == _locked.compare_exchange_strong(expected, desired))
		{
			expected = false;
		}		
	}

	void unlock()
	{
		_locked.store(false);
	}

private:
	atomic<bool> _locked = false;
};

int32		sum = 0;
SpinLock	spinLock;

void Add()
{
	for (int32 ii = 0; ii < 10'000; ++ii)
	{
		lock_guard<SpinLock> lock(spinLock);
		++sum;
	}
}

void Sub()
{
	for (int32 ii = 0; ii < 10'000; ++ii)
	{
		lock_guard<SpinLock> lock(spinLock);
		--sum;
	}
}

int main()
{
	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}