#include "pch.h"

#include <thread>(
#include <atomic>
#include <mutex>
#include <chrono>
#include <future>
#include <windows.h>
#include <format>
#include <ThreadManager.h>

#include "RefCounting.h"

class Wraight : public RefCountable
{
public:
	int32	_hp		= 150;
	int32	_posX	= 0;
	int32	_posY	= 0;
};

using WraightRef = TSharedPtr<Wraight>;

class Missile : public RefCountable
{
public:
	void SetTarget(WraightRef target)
	{
		_target = target;
	}
	
	bool Update()
	{
		if (_target == nullptr)
			return true;

		int32 posX = _target->_posX;
		int32 posY = _target->_posY;

		if (0 == _target->_hp)
		{
			_target = nullptr;
			return true;
		}
	}

private:
	WraightRef _target = nullptr;
};

using MissileRef = TSharedPtr<Missile>;

int main()
{
	WraightRef wraight(new Wraight());
	wraight->ReleaseRef();
	MissileRef missile(new Missile());
	missile->ReleaseRef();

	missile->SetTarget(wraight);

	wraight->_hp	= 0;
	wraight			= nullptr;
	
	while (true)
	{
		if (missile != nullptr)
			missile->Update();
	}

	missile = nullptr;

	return 0;
}