#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// 위로 얼려서 AccountLock을 먼저 잡으면 순서 보장이 되어 Deadlock 발생 안함
	Account* account = AccountManager::Instance()->GetAccount(100);

	lock_guard<mutex> guard(_mutex);

	// AccountManager에서 lock을 또 잡아 Deadlock 유발가능
	// Account* account = AccountManager::Instance()->GetAccount(100);
}
