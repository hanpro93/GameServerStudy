#include "pch.h"
#include <thread>
#include <mutex>
#include <windows.h>
#include <chrono>

mutex			gLock;
queue<int32>	gNumQueue;

// Condition Variable 은 User-Level 오브젝트임
condition_variable gCV;

void Producer()
{
	while (true)
	{
		// lock 잡기
		// 변수 수정
		// lock 풀기
		// 통지

		{
			unique_lock<mutex> lock(gLock);
			gNumQueue.push(100);
		}

		gCV.notify_one();
	}
}

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(gLock);
		gCV.wait(lock, []() { return false == gNumQueue.empty(); });
		// lock 잡고
		// 조건 확인
		// 조건 만족시 이어서 코드진행
		// 조건 불만족시 lock을 풀고 대기상태

		int32 data = gNumQueue.front();
		gNumQueue.pop();
		cout << data << endl;
	}
}

int main()
{
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();
}