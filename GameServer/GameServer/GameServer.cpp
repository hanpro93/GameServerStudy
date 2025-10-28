#include "pch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>

int32 buffer[10000][10000];

int main()
{
	memset(buffer, 0, sizeof(buffer));

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;
		for (int32 ii = 0; ii < 10000; ++ii)
		{
			for (int32 jj = 0; jj < 10000; ++jj)
			{
				sum += buffer[ii][jj];
			}
		}

		uint64 end = GetTickCount64();
		cout << "Elapsed Tick " << (end - start) << endl;
	}

	{
		uint64 start = GetTickCount64();

		int64 sum = 0;
		for (int32 ii = 0; ii < 10000; ++ii)
		{
			for (int32 jj = 0; jj < 10000; ++jj)
			{
				sum += buffer[jj][ii];
			}
		}

		uint64 end = GetTickCount64();
		cout << "Elapsed Tick " << (end - start) << endl;
	}
}