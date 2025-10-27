#include "pch.h"
#include <thread>
#include <mutex>
#include <windows.h>
#include <chrono>

mutex			gLock;
queue<int32>	gNumQueue;
HANDLE			gHandle;

void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(gLock);
			gNumQueue.push(100);
		}

		::SetEvent(gHandle); // Signal 상태로 바꿔줌
		this_thread::sleep_for(100ms);
	}
}

void Consumer()
{
	while (true)
	{
		::WaitForSingleObject(gHandle, INFINITE); // Signal 상태일 때 깨어남

		unique_lock<mutex> lock(gLock);
		if (false == gNumQueue.empty())
		{
			int32 data = gNumQueue.front();
			gNumQueue.pop();
			cout << data << endl;
		}
	}
}

int main()
{
	// 커널 오브젝트
	// Usage Count(몇 명의 스레드에 서 관리할 것인가
	// Signal / Non-Signal
	// Auto / Manual

	gHandle = ::CreateEvent(NULL/*보안속성*/, FALSE/*bManualRest*/, FALSE/*bInitialState*/, NULL);

	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(gHandle);
}