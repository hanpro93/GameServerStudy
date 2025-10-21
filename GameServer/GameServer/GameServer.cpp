#include <thread>
#include <mutex>

#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void Func()
{
	for (int32 ii = 0; ii < 100; ++ii)
	{
		UserManager::Instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 ii = 0; ii < 100; ++ii)
	{
		AccountManager::Instance()->ProcessLogin();
	}
}

int main()
{
	thread t1(Func);
	thread t2(Func2);

	t1.join();
	t2.join();

	cout << "Jobs Done" << endl;

	// mutex m1;
	// mutex m2;
	// lock(m1, m2) // m1잠그고 m2잠금 순서보장
	// lock_guard<mutex> g1(m1, std::adopt_lock); // 이미 잠겨잇으니까 풀어주기만 해
	// lock_guard<mutex> g2(m2, std::adopt_lock); // 이미 잠겨잇으니까 풀어주기만 해
}