#pragma once

#include "SharedPtr.h"

template<typename Type, typename Deleter = std::default_delete<Type>>
class WeakPtr
{
private:
	Type* m_pRawPtr;
	int* m_pRefCount;

public:
	// Default members
	WeakPtr();
	~WeakPtr();
	WeakPtr(const WeakPtr& other);
	WeakPtr& operator=(const WeakPtr& other);
	WeakPtr(WeakPtr&& other);
	WeakPtr& operator=(WeakPtr&& other);

	// SharedPtr interactions
	WeakPtr(const SharedPtr<Type, Deleter>& sharedPtr);
	WeakPtr& operator=(const SharedPtr<Type, Deleter>& sharedPtr);
	WeakPtr(SharedPtr<Type, Deleter>&& sharedPtr);
	WeakPtr& operator=(SharedPtr<Type, Deleter>&& sharedPtr);

	// API
	int UseCount() const;
	bool Expired() const;
	SharedPtr<Type, Deleter> Lock() const;
};

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>::WeakPtr()
	: m_pRawPtr{ nullptr }
	, m_pRefCount{ nullptr }
{
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>::~WeakPtr()
{
	// Do nothing
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>::WeakPtr(const WeakPtr& other)
	: m_pRawPtr{ other.m_pRawPtr }
	, m_pRefCount{ other.m_pRefCount }
{
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>& WeakPtr<Type, Deleter>::operator=(const WeakPtr& other)
{
	if (this == &other)
		return *this;

	m_pRawPtr = other.m_pRawPtr;
	m_pRefCount = other.m_pRefCount;

	return *this;
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>::WeakPtr(WeakPtr&& other)
	: m_pRawPtr{ other.m_pRawPtr }
	, m_pRefCount{ other.m_pRefCount }
{
	other.m_pRawPtr = nullptr;
	other.m_pRefCount = nullptr;
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>& WeakPtr<Type, Deleter>::operator=(WeakPtr&& other)
{
	if (this == &other)
		return *this;

	m_pRawPtr = other.m_pRawPtr;
	m_pRefCount = other.m_pRefCount;

	other.m_pRawPtr = nullptr;
	other.m_pRefCount = nullptr;

	return *this;
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>::WeakPtr(const SharedPtr<Type, Deleter>& sharedPtr)
	: m_pRawPtr{ sharedPtr.GetPtr() }
	, m_pRefCount{ sharedPtr.GetRefCount() }
{
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>& WeakPtr<Type, Deleter>::operator=(const SharedPtr<Type, Deleter>& sharedPtr)
{
	m_pRawPtr = sharedPtr.GetPtr();
	m_pRefCount = sharedPtr.GetRefCount();
	return *this;
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>::WeakPtr(SharedPtr<Type, Deleter>&& sharedPtr)
	: m_pRawPtr{ sharedPtr.GetPtr() }
	, m_pRefCount{ sharedPtr.GetRefCount() }
{
	sharedPtr.Clear();
}

template<typename Type, typename Deleter>
inline WeakPtr<Type, Deleter>& WeakPtr<Type, Deleter>::operator=(SharedPtr<Type, Deleter>&& sharedPtr)
{
	m_pRawPtr = sharedPtr.GetPtr();
	m_pRefCount = sharedPtr.GetRefCount();

	sharedPtr.Clear();

	return *this;
}

template<typename Type, typename Deleter>
inline int WeakPtr<Type, Deleter>::UseCount() const
{
	return (m_pRefCount) ? (*m_pRefCount) : (0);
}

template<typename Type, typename Deleter>
inline bool WeakPtr<Type, Deleter>::Expired() const
{
	return UseCount() == 0;
}

template<typename Type, typename Deleter>
inline SharedPtr<Type, Deleter> WeakPtr<Type, Deleter>::Lock() const
{
	SharedPtr<Type, Deleter> sharedPtr;
	sharedPtr.SetPtr(m_pRawPtr);
	return sharedPtr;
}