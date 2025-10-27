#include "pch.h"

#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>

int64 Calculate()
{
	int64 sum = 0;

	for (int32 ii = 0; ii< 100'000; ++ii)
	{
		sum += ii;
	}

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Secret Message");
}


void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// std::future
	{
		// deferred -> 지연 실행
		// async -> 별도의 쓰레드를 만들어서 실행
		// deferred | async -> 둘중 알아서 실행
		std::future<int64> future = std::async(std::launch::async, Calculate);


		int64 sum = future.get();

		cout << sum << endl;
	}

	// std::promise
	{
		// 미래에 결과물의 반환해줄꺼라 약속
		std::promise<string>	promise;
		std::future<string>		future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		cout << message << endl;

		t.join();
	}

	// std::packaged_task
	{
		std::packaged_task<int64(void)>	task(Calculate);
		std::future<int64>				futrue = task.get_future();

		thread t(TaskWorker, std::move(task));

		int64 sum = futrue.get();
		cout << sum << endl;

		t.join();
	}
}