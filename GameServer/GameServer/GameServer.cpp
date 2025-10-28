#include "pch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>
#include <format>

//__declspec(thread) int32 value;
thread_local int32 LThreadID = 0;


void ThreadMain(int32 threadID)
{
	LThreadID = threadID;

	while (true)
	{
		cout << format("나는 스레드 {}번 이야!!", LThreadID) << endl;
	}
}

int main()
{
	vector <thread> threads;

	for (int32 ii = 0; ii < 10; ++ii)
	{
		int32 threadID = ii + 1;
		threads.push_back(thread(ThreadMain, threadID));
	}

	for (auto& t : threads)
		t.join();
}