#pragma once
// Unique pointer implementation by Zixuan Shi

//--------------------------------------------------------------------------------------------------------------------
// UniquePtr is a smart pointer that owns and manages another object through a pointer and 
// disposes of that object when the unique_ptr goes out of scope.
// https://en.cppreference.com/w/cpp/memory/unique_ptr
//--------------------------------------------------------------------------------------------------------------------
template<typename Type, typename Deleter = std::default_delete<Type>>
class UniquePtr
{
private:
	Type* m_pRawPtr;

public:
	// Disallow copy construct and assign 
	UniquePtr(const UniquePtr& other) = delete;
	UniquePtr& operator=(const UniquePtr& other) = delete;

	// Default members
	UniquePtr();
	UniquePtr(Type* pPtr);
	~UniquePtr();
	UniquePtr(UniquePtr&& other) noexcept;
	UniquePtr& operator=(UniquePtr&& other) noexcept; 

	// API
	template<class... Args> static UniquePtr Make(Args&&... args);
	Type* operator->() const;
	Type* Get() const;
};

template<typename Type, typename Deleter>
inline UniquePtr<Type, Deleter>::UniquePtr()
	: m_pRawPtr{ nullptr }
{
}

template<typename Type, typename Deleter>
inline UniquePtr<Type, Deleter>::UniquePtr(Type* pPtr)
	: m_pRawPtr{ pPtr }
{
}

template<typename Type, typename Deleter>
inline UniquePtr<Type, Deleter>::~UniquePtr()
{
	delete m_pRawPtr;
	m_pRawPtr = nullptr;
}

template<typename Type, typename Deleter>
inline UniquePtr<Type, Deleter>::UniquePtr(UniquePtr&& other) noexcept
	: m_pRawPtr{ other.m_pRawPtr }
{
	other.m_pRawPtr = nullptr;
}

template<typename Type, typename Deleter>
inline UniquePtr<Type, Deleter>& UniquePtr<Type, Deleter>::operator=(UniquePtr&& other) noexcept
{
	if (this == &other)
		return *this;

	delete m_pRawPtr;
	m_pRawPtr = other.m_pRawPtr;

	other.m_pRawPtr = nullptr;

	return *this;
}

template<typename Type, typename Deleter>
inline Type* UniquePtr<Type, Deleter>::operator->() const
{
	return m_pRawPtr;
}

template<typename Type, typename Deleter>
inline Type* UniquePtr<Type, Deleter>::Get() const
{
	return m_pRawPtr;
}

template<typename Type, typename Deleter>
template<class ...Args>
inline UniquePtr<Type, Deleter> UniquePtr<Type, Deleter>::Make(Args && ...args)
{
	return UniquePtr(new Type(std::forward<Args>(args)...));
}