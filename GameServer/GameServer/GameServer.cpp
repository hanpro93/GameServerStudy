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

class Missile : public RefCountable
{
public:
	void SetTarget(Wraight* target)
	{
		_target = target;
		_target->AddRef();
	}
	
	void Update()
	{
		int32 posX = _target->_posX;
		int32 posY = _target->_posY;

		if (0 == _target->_hp)
		{
			_target->ReleaseRef();
			_target = nullptr;
		}
	}

private:
	Wraight* _target = nullptr;
};

int main()
{
	Wraight* wraight = new Wraight();
	Missile* missile = new Missile();

	missile->SetTarget(wraight);

	wraight->_hp = 0;
	wraight->ReleaseRef();
	wraight = nullptr;
	
	while (true)
	{
		if (nullptr != missile)
			missile->Update();
	}

	missile->ReleaseRef();
	missile = nullptr;

	return 0;
}