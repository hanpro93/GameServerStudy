#pragma once
#include <stack>
#include <vector>
#include <map>

using namespace	std;


/***********************
	DeadLockProfiler
***********************/

class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(const int32 here);

	unordered_map<const char*, int32>	_nameToID;
	unordered_map<int32, const char*>	_IDToName;
	stack<int32>						_lockStack;
	map<int32, set<int32>>				_lockHistory;

	Mutex								_lock;

	vector<int32>						_discoveredOrder;
	int32								_discoveredCount;
	vector<bool>						_finished;
	vector<int32>						_parent;
};