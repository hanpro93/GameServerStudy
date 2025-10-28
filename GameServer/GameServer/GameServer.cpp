#include "pch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>

// 가시성, 코드 재배치
int32 x		= 0;
int32 y		= 0;
int32 r1	= 0;
int32 r2	= 0;

volatile bool ready;

void ThreadFunc1()
{
	while (false == ready)
	{
	}

	// 아래 코드가 코드 재배치가 일어 날 수 있음
	y	= 1;
	r1	= x;
}

void ThreadFunc2()
{
	while (false == ready)
	{
	}

	// 아래 코드가 코드 재배치가 일어 날 수 있음
	x	= 1;
	r2	= y;
}

int main()
{
	int32 count = 0;

	while (true)
	{
		ready = false;
		++count;

		x = y = r1 = r2 = 0;

		thread t1(ThreadFunc1);
		thread t2(ThreadFunc2);

		ready = true;

		t1.join();
		t2.join();

		if ((0 == r1) && (0 == r2))
			break;
	}

	cout << count << "번만에 빠져나옴!!" << endl;
}