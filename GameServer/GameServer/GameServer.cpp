#include "pch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>
#include <format>
#include <ThreadManager.h>

void ThreadMain()
{
	while (true)
	{
		cout << format("나는 {}번 스레드야!!!", LThreadID) << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	for (int32 ii = 0; ii < 5; ++ii)
	{
		GThreadManager->Launch(ThreadMain);
	}

	GThreadManager->Join();
}