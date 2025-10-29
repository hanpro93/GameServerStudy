#include "pch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>
#include <format>
#include "ConcurrentQueue.h"
#include "ConcurrentStack.h"

LockQueue<int32>		q;
LockFreeStack<int32>	s;

void Push()
{
	while (true)
	{
		int32 value = rand() % 10;
		s.Push(value);
	}
}

void Pop()
{
	while (true)
	{
		int32 data = 0;
		if (true == s.TryPop(OUT data))
			cout << data << endl;
	}
}

int main()
{
	thread t1(Push);
	thread t2(Pop);
	thread t3(Pop);

	t1.join();
	t2.join();
	t3.join();
}