#include "pch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>
#include <format>
#include <ThreadManager.h>

class TestLock
{
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;

		if (true == _queue.empty())
			return -1;

		return _queue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;

		_queue.push(rand() % 100);
	}

	void TestPop()
	{
		READ_LOCK;

		if (true == _queue.empty())
			return;

		_queue.pop();
	}

private:
	queue<int32> _queue;
};

TestLock testLock;

void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}
}

void ThreadRead()
{
	while (true)
	{
		int32 value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}
}

int main()
{
	for (int32 ii = 0; ii < 2; ++ii)
	{
		GThreadManager->Launch(ThreadWrite);
	}

	for (int32 ii = 0; ii < 5; ++ii)
	{
		GThreadManager->Launch(ThreadRead);
	}

	GThreadManager->Join();
}