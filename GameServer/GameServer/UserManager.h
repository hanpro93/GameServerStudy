#pragma once
#include <mutex>

class User
{
	// TODO
};

class UserManager
{
public:
	static UserManager* Instance()
	{
		static UserManager instance;
		return &instance;
	}

	User* GetUser(const int32 id)
	{
		lock_guard<mutex> gurad(_mutex);

		//유저 반환 해야함
		return nullptr;
	}

	void ProcessSave();

private:
	mutex _mutex;
};

