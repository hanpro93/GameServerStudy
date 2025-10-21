#include "pch.h"

#include <thread>
#include <format>

void HelloThread()
{
	cout << "Hello Thread" << endl;
}

void HelloThread2(const int32 num)
{
	cout << num << endl;
}

int main()
{
	vector<thread> threadList;

	for (int32 ii = 0; ii < 10; ++ii)
	{
		threadList.push_back(thread(HelloThread2, ii));
	}

	for (int32 ii = 0; ii < 10; ++ii)
	{
		if (true == threadList[ii].joinable())
			threadList[ii].join();

		// threadList[ii].hardware_concurrency() -> 동시에 사용될 수 있는 스레드 개수 반환
		// threadList[ii].get_id() -> 스레드 id반환
	}
}