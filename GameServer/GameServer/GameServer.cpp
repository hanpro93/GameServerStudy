#include "pch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>

atomic<bool>	ready;
int32			value;

void Producer()
{
	value = 10;

	ready.store(true, memory_order::memory_order_release);
}

void Consumer()
{
	while (false == ready.load(memory_order::memory_order_acquire))
	{

	}

	cout << value << endl;
}

int main()
{
	ready = false;
	value = 0;
	
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();
}