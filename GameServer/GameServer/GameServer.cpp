#include "pch.h"

#include <thread>
#include <mutex>

vector<int32> numList;

// 상호 배타적 lock
mutex numListlock;

void Push()
{
	for (int32 ii = 0; ii < 10000; ++ii)
	{
		lock_guard<mutex> lock(numListlock);
		//unique_lock<mutex> lock(numListlock, defer_lock); -> lock 에 옵션 부여가능
		numList.push_back(ii);
	}
}

int main()
{
	thread thread1(Push);
	thread thread2(Push);

	thread1.join();
	thread2.join();

	cout << numList.size() << endl;
}