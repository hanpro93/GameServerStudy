#pragma 

class RefCountable
{
public:
	RefCountable() : _refCount(1) { };
	virtual ~RefCountable() { };

	int32 GetRefCount() { return _refCount; }
	int32 AddRef() { return ++_refCount; }
	int32 ReleaseRef()
	{
		int32 refCount = --_refCount;
		if (refCount == 0)
			delete this;
		
		return refCount;
	}

private:
	int32 _refCount = 0;
};

template<typename T>
class TSharedPtr
{
public:
	TSharedPtr(){}
	TSharedPtr(T* ptr) { Set(ptr); }

	TSharedPtr(const TSharedPtr& rhs) { Set(rhs); }
	TSharedPtr(TSharedPtr&& rhs) { _ptr = rhs._ptr; rhs._ptr = nullptr;	}

	template<typename U>
	TSharedPtr(const TSharedPtr<U>& rhs) { Set(static_cast<T*>(rhs._ptr)); }

	~TSharedPtr() {	Release(); }

	TSharedPtr& operator=(const TSharedPtr& rhs)
	{
		if (_ptr != rhs._ptr)
		{
			Release();
			Set(rhs._ptr);
		}
		
		return *this;
	}


	TSharedPtr& operator=(TSharedPtr&& rhs)
	{
		Release();
		_ptr		= rhs._ptr;
		rhs._ptr	= nullptr;

		return *this;
	}

	bool		operator==(const TSharedPtr& rhs) const { return _ptr == rhs._ptr; }
	bool		operator==(const T* ptr) const { return _ptr == ptr; }
	bool		operator!=(const TSharedPtr& rhs) const { return _ptr != rhs._ptr; }
	bool		operator!=(const T* ptr) const { return _ptr != ptr; }
	bool		operator<(const TSharedPtr& ptr) const { return _ptr < ptr; }
	T*			operator*() { return _ptr; }
	const T*	operator*() const { return _ptr; }
	T*			operator->() { return _ptr; }
	const T*	operator->() const { return _ptr; }
				operator T* () const { return _ptr; }

	bool IsNull() { return (_ptr == nullptr); }

private:
	inline void Set(T* ptr)
	{
		_ptr = ptr;
		if (_ptr != nullptr)
			_ptr->AddRef();
	}

	inline void Release()
	{
		if (_ptr != nullptr)
		{
			_ptr->ReleaseRef();
			_ptr = nullptr;
		}
	}

	T* _ptr = nullptr;
};
