#pragma once

#include <mutex>
#include <atomic>

template<typename T>
class LockFreeStack
{
	struct Node;

	struct CountedNodePtr
	{
		int32 externalCount = 0;
		Node* ptr = nullptr;
	};

	struct Node
	{
		Node(const T& value) : _data(make_shared<T>(value))
		{

		}

		shared_ptr<T>	_data;
		CountedNodePtr	_next;
		atomic<int32>	_internalCount = 0;
	};

public:
	void Push(const T& value)
	{
		CountedNodePtr node;
		node.ptr = new Node(value);
		node.externalCount = 1;

		node.ptr->_next = _head;
		while (false == _head.compare_exchange_weak(node.ptr->_next, node))
		{

		}
	}

	shared_ptr<T> TryPop()
	{
		CountedNodePtr oldHead = _head;
		while (true)
		{
			IncreaseHeadCount(oldHead);
			Node* ptr = oldHead.ptr;

			if (nullptr == ptr)
				return shared_ptr<T>();

			if (true == _head.compare_exchange_strong(oldHead, ptr->_next))
			{
				shared_ptr<T> result;
				result.swap(ptr->_data);

				const int32 countIncrease = oldHead.externalCount - 2;

				if (ptr->_internalCount.fetch_add(countIncrease) == -countIncrease)
					delete ptr;

				return result;
			}
			else if (ptr->_internalCount.fetch_sub(1) == 1)
			{
				delete ptr;
			}
		}
	}

	void IncreaseHeadCount(CountedNodePtr& oldCounter)
	{
		while (true)
		{
			CountedNodePtr newCounter = oldCounter;
			++newCounter.externalCount;
			if (_head.compare_exchange_strong(oldCounter, newCounter))
			{
				oldCounter.externalCount = newCounter.externalCount;
				break;
			}
		}
	}
	
private:
	atomic<CountedNodePtr> _head;
};