#pragma once

#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack() {}

	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(const T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_CV.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_CV.wait(lock, [this] {return false == _stack.empty(); });
		value = std::move(_stack.top());
		_stack.pop();
	}

private:
	stack<T>			_stack;
	mutex				_mutex;
	condition_variable	_CV;
};

template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : _data(value)
		{
			
		}

		T		_data;
		Node*	_next = nullptr;
	};

public:
	void Push(const T& value)
	{
		Node* node	= new Node(value);
		node->_next = _head;

		while (false == _head.compare_exchange_weak(node->_next, node))
		{

		}
	}

	bool TryPop(T& value)
	{
		++_popCount;

		Node* oldHead = _head;

		while ((nullptr != oldHead) && (false == _head.compare_exchange_weak(oldHead, oldHead->_next)))
		{

		}

		if (nullptr == oldHead)
		{
			--_popCount;
			return false;
		}

		value = oldHead->_data;
		TryDelete(oldHead);
		
		return true;
	}
	
private:
	void TryDelete(Node* oldHead)
	{
		if (1 == _popCount)
		{
			Node* node = _pendingList.exchange(nullptr);

			if (--_popCount == 0)
			{
				DeleteNodes(node);
			}
			else if(nullptr != node)
			{
				ChainPendingNodeList(node);
			}

			delete oldHead;
		}
		else
		{
			ChainPendingNode(oldHead);
			--_popCount;
		}
	}

	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->_next = _pendingList;
		
		while (false == _pendingList.compare_exchange_weak(last->_next, first))
		{

		}
	}

	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		while (nullptr != last->_next)
			last = last->_next;

		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}

	static void DeleteNodes(Node* node)
	{
		while (nullptr != node)
		{
			Node* next = node->_next;
			delete node;
			node = next;
		}
	}

	atomic<Node*>	_head			= nullptr;
	atomic<Node*>	_pendingList	= nullptr;
	atomic<uint32>	_popCount		= 0;
};