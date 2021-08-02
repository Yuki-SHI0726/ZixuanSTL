#pragma once
// Shared pointer implementation by Zixuan Shi

//--------------------------------------------------------------------------------------------------------------------
// std::shared_ptr is a smart pointer that retains shared ownership of an object through a pointer. 
// Several shared_ptr objects may own the same object. 
// The object is destroyed and its memory deallocated when either of the following happens:
// the last remaining shared_ptr owning the object is destroyed;
// the last remaining shared_ptr owning the object is assigned another pointer via operator= or reset().
//--------------------------------------------------------------------------------------------------------------------
template<typename Type, typename Deleter = std::default_delete<Type>>
class SharedPtr
{
private:
	Type* m_pRawPtr;
	int* m_pRefCount;

public:
	// Default members
	SharedPtr();
	SharedPtr(Type* pPtr);
	~SharedPtr();
	SharedPtr(const SharedPtr& other);
	SharedPtr& operator=(const SharedPtr& other);
	SharedPtr(SharedPtr&& other) noexcept;
	SharedPtr& operator=(SharedPtr&& other) noexcept;

	// API
	template<class... Args> static SharedPtr Make(Args&&... args);
	Type* operator->() const{ return m_pRawPtr; }
	Type* GetPtr() const { return m_pRawPtr; }
	int* GetRefCount() const { return m_pRefCount; }
	int UseCount() const { return (m_pRefCount) ? (*m_pRefCount) : (0); }
	void SetPtr(Type* pPtr) { m_pRawPtr = pPtr; }
	void Clear();
};

template<typename Type, typename Deleter>
inline SharedPtr<Type, Deleter>::SharedPtr()
	: m_pRawPtr{ nullptr }
	, m_pRefCount{ nullptr }
{
}

template<typename Type, typename Deleter>
inline SharedPtr<Type, Deleter>::SharedPtr(Type* pPtr)
	: m_pRawPtr{ pPtr }
	, m_pRefCount{ new int(1) }
{
}

template<typename Type, typename Deleter>
inline SharedPtr<Type, Deleter>::~SharedPtr()
{
	if (m_pRefCount)
	{
		--*m_pRefCount;
		if (*m_pRefCount == 0)
		{
			delete m_pRefCount;
			m_pRefCount = nullptr;

			delete m_pRawPtr;
			m_pRawPtr = nullptr;
		}
	}
}

template<typename Type, typename Deleter>
inline SharedPtr<Type, Deleter>::SharedPtr(const SharedPtr& other)
	: m_pRawPtr{ other.m_pRawPtr }
	, m_pRefCount{ other.m_pRefCount }
{
	++(*m_pRefCount);
}

template<typename Type, typename Deleter>
inline SharedPtr<Type, Deleter>& SharedPtr<Type, Deleter>::operator=(const SharedPtr& other)
{
	if (this == &other)
		return *this;

	delete m_pRawPtr;
	delete m_pRefCount;
	m_pRawPtr = other.m_pRawPtr;
	m_pRefCount = other.m_pRefCount;

	++(*m_pRefCount);

	return *this;
}

template<typename Type, typename Deleter>
inline SharedPtr<Type, Deleter>::SharedPtr(SharedPtr&& other) noexcept
	: m_pRawPtr{ other.m_pRawPtr }
	, m_pRefCount{ other.m_pRefCount }
{
	other.m_pRawPtr = nullptr;
	other.m_pRefCount = nullptr;
}

template<typename Type, typename Deleter>
inline SharedPtr<Type, Deleter>& SharedPtr<Type, Deleter>::operator=(SharedPtr&& other) noexcept
{
	if (this == &other)
		return *this;

	delete m_pRawPtr;
	delete m_pRefCount;
	m_pRawPtr = other.m_pRawPtr;
	m_pRefCount = other.m_pRefCount;

	other.m_pRawPtr = nullptr;
	other.m_pRefCount = nullptr;

	return *this;
}

template<typename Type, typename Deleter>
inline void SharedPtr<Type, Deleter>::Clear()
{
	m_pRawPtr = nullptr;
	m_pRefCount = nullptr;
}

template<typename Type, typename Deleter>
template<class ...Args>
inline SharedPtr<Type, Deleter> SharedPtr<Type, Deleter>::Make(Args && ...args)
{
	return SharedPtr(new Type(std::forward<Args>(args)...));
}
