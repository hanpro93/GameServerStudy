#include "pch.h"

#include <thread>
#include <format>

atomic<int32> sum = 0;

void Add()
{
	for (int32 ii = 0; ii < 100'0000; ++ii)
	{
		sum.fetch_add(1);
	}
}

void Sub()
{
	for (int32 ii = 0; ii < 100'0000; ++ii)
	{
		sum.fetch_add(-1);
	}
}

int main()
{
	thread thread1(Add);
	thread thread2(Sub);

	thread1.join();
	thread2.join();

	cout << sum << endl;
}