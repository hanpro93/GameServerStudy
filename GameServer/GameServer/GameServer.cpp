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

LockFreeQueue<int32>	q;
LockFreeStack<int32>	s;

void Push()
{
	while (true)
	{
		int32 value = rand() % 10;
		q.Push(value);

		this_thread::sleep_for(10ms);
	}
}

void Pop()
{
	while (true)
	{
		auto data = q.TryPop();
		if (nullptr != data)
			cout << (*data) << endl;
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