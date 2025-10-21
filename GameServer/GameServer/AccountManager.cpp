#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void AccountManager::ProcessLogin()
{
	lock_guard<mutex> guard(_mutex);

	// 내부에서 UserManager Lock도 잡아야해서 deadlock 유발 가능
	User* user = UserManager::Instance()->GetUser(100);
}
