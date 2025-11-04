#include "pch.h"

#include <thread>(
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>
#include <format>
#include <ThreadManager.h>

bool IsPrime(const int64 number)
{
	if (1 >= number)
		return false;

	if ((3 >= number))
		return true;

	for (int64 divideNumber = 2; (divideNumber * divideNumber) <= number; ++divideNumber)
	{
		if (0 == (number % divideNumber))
			return false;
	}

	return true;
}

int64 CountPrime(const int64 start, const int64 end)
{
	int64 count = 0;

	for (int64 number = start; number <= end; ++number)
	{
		if (true == IsPrime(number))
			++count;
	}

	return count;
}

int main()
{
	const int64 MAX_NUMBER = 1'000'000;

	vector<thread> threads;

	const int64 coreCount	= thread::hardware_concurrency();
	const int64 jobCount	= (MAX_NUMBER / coreCount) + 1;

	atomic<int64> primeCount = 0;

	for (int64 ii = 0; ii < coreCount; ++ii)
	{
		const int64 start	= (ii * jobCount) + 1;
		const int64 end		= min(MAX_NUMBER, ((ii + 1) * jobCount));

		threads.push_back(thread([start, end, &primeCount]()
			{
				primeCount += CountPrime(start, end);
			}));
	}

	for (thread& t : threads)
		t.join();

	cout << primeCount << endl;
}