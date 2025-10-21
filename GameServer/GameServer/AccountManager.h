#pragma once
#include <mutex>

class Account
{
	// TODO
};

class AccountManager
{
public:
	static AccountManager* Instance()
	{
		static AccountManager instance;
		return &instance;
	}

	Account* GetAccount(const int32 id)
	{
		lock_guard<mutex> gurad(_mutex);

		//계정 반환 해야함
		return nullptr;
	}

	void ProcessLogin();

private:
	mutex _mutex;
};

