#include "pch.h"
#include <thread>
#include <mutex>

class SleepLock
{
public:
	void lock()
	{
		bool expected	= false;
		bool desired	= true;
		
		while (false == _locked.compare_exchange_strong(expected, desired))
		{
			expected = false;

			this_thread::sleep_for(std::chrono::milliseconds(0));
			//this_thread::sleep_for(100ms);
			//this_thread::yield() == this_thread::sleep_for(0ms);
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
SleepLock	sleepLock;

void Add()
{
	for (int32 ii = 0; ii < 10'000; ++ii)
	{
		lock_guard<SleepLock> lock(sleepLock);
		++sum;
	}
}

void Sub()
{
	for (int32 ii = 0; ii < 10'000; ++ii)
	{
		lock_guard<SleepLock> lock(sleepLock);
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