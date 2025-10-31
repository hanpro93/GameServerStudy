#pragma once
#include <mutex>

template<typename T>
class LockFreeQueue
{
	struct Node;

	struct CountedNodePtr
	{
		int32 _externalCount;
		Node* _ptr = nullptr;
	};

	struct NodeCounter
	{
		uint32 _internalCount : 30;
		uint32 _externalCountRemaining : 2;
	};

	struct Node
	{
		Node()
		{
			NodeCounter newCount;
			newCount._internalCount				= 0;
			newCount._externalCountRemaining	= 2;
			_count.store(newCount);

			_next._ptr			= nullptr;
			_next._externalCount	= 0;
		}

		void ReleaseRef()
		{
			NodeCounter oldCounter = _count.load();

			while (true)
			{
				NodeCounter newCounter = oldCounter;
				--newCounter._internalCount;

				if (_count.compare_exchange_strong(oldCounter, newCounter))
				{
					if ((0 == newCounter._internalCount) && (0 == newCounter._externalCountRemaining))
						delete this;

					break;
				}
			}
		}

		atomic<T*>			_data;
		atomic<NodeCounter> _count;
		CountedNodePtr		_next;
	};

public:
	LockFreeQueue()
	{
		CountedNodePtr node;
		node._ptr			= new Node();
		node._externalCount = 1;

		_head.store(node);
		_tail.store(node);
	}

	LockFreeQueue(const LockFreeQueue&) = delete;
	LockFreeQueue operator=(const LockFreeQueue&) = delete;

	void Push(const T& value)
	{
		unique_ptr<T> newData = make_unique<T>(value);

		CountedNodePtr dummy;
		dummy._ptr				= new Node;
		dummy._externalCount	= 1;

		CountedNodePtr oldTail = _tail.load();

		while (true)
		{
			IncreaseExternalCount(_tail, oldTail);

			T* oldData = nullptr;
			if (true == oldTail._ptr->_data.compare_exchange_strong(oldData, newData.get()))
			{
				oldTail._ptr->_next = dummy;
				oldTail = _tail.exchange(dummy);
				FreeExternalCount(oldTail);
				newData.release();
				break;
			}

			oldTail._ptr->ReleaseRef();
		}
	}

	shared_ptr<T> TryPop()
	{
		CountedNodePtr oldeHead = _head.load();

		while (true)
		{
			IncreaseExternalCount(_head, oldeHead);

			Node* ptr = oldeHead._ptr;
			if (ptr == _tail.load()._ptr)
			{
				ptr->ReleaseRef();
				return shared_ptr<T>();
			}

			if (true == _head.compare_exchange_strong(oldeHead, ptr->_next))
			{
				T* result = ptr->_data.exchange(nullptr);
				FreeExternalCount(oldeHead);
				return shared_ptr<T>(result);
			}

			ptr->ReleaseRef();
		}
	}

private:
	static void IncreaseExternalCount(atomic<CountedNodePtr>& counter, CountedNodePtr& oldCounter)
	{
		while (true)
		{
			CountedNodePtr newCounter = oldCounter;
			++newCounter._externalCount;

			if (counter.compare_exchange_strong(oldCounter, newCounter))
			{
				oldCounter._externalCount = newCounter._externalCount;
				break;
			}
		}
	}

	static void FreeExternalCount(CountedNodePtr& oldNodePtr)
	{
		Node* ptr = oldNodePtr._ptr;
		const int32 countIncrease = oldNodePtr._externalCount - 2;

		NodeCounter oldCounter = ptr->_count.load();

		while (true)
		{
			NodeCounter newCounter = oldCounter;
			--newCounter._externalCountRemaining;
			newCounter._internalCount += countIncrease;

			if (ptr->_count.compare_exchange_strong(oldCounter, newCounter))
			{
				if ((0 == newCounter._internalCount) && (0 == newCounter._externalCountRemaining))
					delete ptr;

				break;
			}
		}
	}

	atomic<CountedNodePtr> _head;
	atomic<CountedNodePtr> _tail;
};