#include "pch.h"
#include "DeadLockProfiler.h"

/***********************
	DeadLockProfiler
***********************/

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	int32	lockID = 0;
	auto	findID = _nameToID.find(name);
	if (_nameToID.end() == findID)
	{
		lockID				= static_cast<int32>(_nameToID.size());
		_nameToID[name]		= lockID;
		_IDToName[lockID]	= name;
	}
	else
	{
		lockID = findID->second;
	}

	if (true == _lockStack.empty())
	{
		_lockStack.push(lockID);
		return;
	}

	const int32 prevID = _lockStack.top();
	if (prevID == lockID)
	{
		_lockStack.push(lockID);
		return;
	}

	set<int32>& history = _lockHistory[prevID];
	if (history.end() != history.find(lockID))
	{
		_lockStack.push(lockID);
		return;
	}

	history.insert(lockID);
	CheckCycle();
	_lockStack.push(lockID);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (true == _lockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	int32 lockID = _nameToID[name];
	if (_lockStack.top() != lockID)
		CRASH("INVALID_UNLOCK");

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameToID.size());
	
	_discoveredOrder	= vector<int32>(lockCount, -1);
	_discoveredCount	= 0;
	_finished			= vector<bool>(lockCount, false);
	_parent				= vector<int32>(lockCount, -1);

	for (int32 lockID = 0; lockID < lockCount; ++lockID)
		Dfs(lockID);

	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(const int32 here)
{
	if (-1 != _discoveredOrder[here])
		return;

	_discoveredOrder[here] = _discoveredCount++;

	const auto findIt = _lockHistory.find(here);
	if (_lockHistory.end() == findIt)
	{
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;
	for (const int32 there : nextSet)
	{
		if (-1 == _discoveredOrder[there])
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		if (false == _finished[there])
		{
			cout << format("{} -> {}", _IDToName[here], _IDToName[there]) << endl;

			int32 now = here;
			while (true)
			{
				cout << format("{} -> {}", _IDToName[_parent[now]], _IDToName[now]) << endl;
				now = _parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}
	
	_finished[here] = true;
}
