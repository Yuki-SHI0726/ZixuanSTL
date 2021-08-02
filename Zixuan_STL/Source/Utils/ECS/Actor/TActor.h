#pragma once

#include "ECS/Components/ComponentBase.h"

#include <string>
#include <iostream>
#include <unordered_map>

#define LOG 0

//------------------------------------------------------------------------------------------------------
// Templated Actor class with Data and Ptr
//------------------------------------------------------------------------------------------------------
template<typename DataType, typename PtrType>
class TActor
{
    std::unordered_map<size_t, ComponentBase*> m_componentMap;
    DataType m_data;
    PtrType* m_pPtr;
    std::string m_name;

public:
    TActor();
    TActor(const std::string& name, DataType data, PtrType* pPtr);
    TActor(const TActor& other);
    TActor(TActor&& other) noexcept;
    TActor& operator=(const TActor& other);
    TActor& operator=(TActor&& other) noexcept;
    ~TActor();

    // API
    template <typename Func> void Work(Func&& func);

    // Getter Setters
    void SetData(DataType data) { m_data = data; }
    void SetPtr(PtrType* pPtr) { delete m_pPtr; m_pPtr = pPtr; }
    DataType GetData() const { return m_data; }

    // Utils
    friend std::ostream& operator<<(std::ostream& stream, const TActor<DataType, PtrType>& actor)
    {
        stream << "Actor name: " << actor.m_name << ". Actor data: " << actor.m_data;
        if (actor.m_pPtr)
            stream << ". Actor ptr: " << *actor.m_pPtr;
        return stream;
    }
};

template<typename DataType, typename PtrType>
inline TActor<DataType, PtrType>::TActor()
    : m_componentMap{}
    , m_data{}
    , m_pPtr{ nullptr }
    , m_name{ "Uninitialized" }
{
#if LOG
    std::cout << "Default constructed an Actor: "  << m_name << std::endl;
#endif
}

template<typename DataType, typename PtrType>
inline TActor<DataType, PtrType>::TActor(const std::string& name, DataType data, PtrType* pPtr)
    : m_componentMap{}
    , m_data{ data }
    , m_pPtr{ pPtr }
    , m_name{ name }
{
#if LOG
    std::cout << "Overloaded constructed an Actor: " << m_name << std::endl;
#endif
}

template<typename DataType, typename PtrType>
inline TActor<DataType, PtrType>::TActor(const TActor& other)
    : m_componentMap{}
    , m_data{ other.m_data }
    , m_pPtr{ nullptr }
    , m_name{ other.m_name }
{
    m_componentMap = other.m_componentMap;

    if (other.m_pPtr)
    {
        m_pPtr = new PtrType();
        *m_pPtr = *other.m_pPtr;
    }

#if LOG
    std::cout << "Copy constructed an Actor: " << m_name << std::endl;
#endif
}

template<typename DataType, typename PtrType>
inline TActor<DataType, PtrType>::TActor(TActor&& other) noexcept
    : m_componentMap{}
    , m_data{ other.m_data }
    , m_pPtr{ nullptr }
    , m_name{ other.m_name }
{
    m_componentMap = std::move(other.m_componentMap);
    other.m_componentMap.clear();

    if (other.m_pPtr)
    {
        m_pPtr = other.m_pPtr;
        other.m_pPtr = nullptr;
    }

#if LOG
    std::cout << "Move constructed an Actor: " << m_name << std::endl;
#endif
}

template<typename DataType, typename PtrType>
inline TActor<DataType, PtrType>& TActor<DataType, PtrType>::operator=(const TActor& other)
{
    if (this == &other)
        return *this;

    // Components
    m_componentMap = other.m_componentMap;

    // Data
    m_data = other.m_data;
    m_name = other.m_name;
    
    // Ptr
    delete m_pPtr;
    m_pPtr = new PtrType();
    *m_pPtr = *other.m_pPtr;

#if LOG
    std::cout << "Copy operator= an Actor: " << m_name << std::endl;
#endif

    return (*this);
}

template<typename DataType, typename PtrType>
inline TActor<DataType, PtrType>& TActor<DataType, PtrType>::operator=(TActor&& other) noexcept
{
    if (this == &other)
        return *this;

    // Components
    m_componentMap = std::move(other.m_componentMap);

    // Data
    m_data = other.m_data;
    m_name = other.m_name;

    // Ptr
    delete m_pPtr;
    m_pPtr = other.m_pPtr;
    other = nullptr; 

#if LOG
    std::cout << "Move operator= an Actor: " << m_name << std::endl;
#endif

    return (*this);
}

template<typename DataType, typename PtrType>
inline TActor<DataType, PtrType>::~TActor()
{
    delete m_pPtr;
    m_pPtr = nullptr;

#if LOG
    std::cout << "Destructed an Actor: " << m_name << std::endl;
#endif
}

template<typename DataType, typename PtrType>
template<typename Func>
inline void TActor<DataType, PtrType>::Work(Func&& func)
{
    func(m_data, m_pPtr);
    std::cout << m_name << " actor is working on " << m_data;
    if (m_pPtr)
        std::cout << ". Actor ptr: " << *m_pPtr << std::endl;
}

/*

template<typename Type = int>
class Base
{
public:
    Base() { std::cout << "Base Ctor" << std::endl; }
    ~Base() { std::cout << "Base Dtor" << std::endl; }
    virtual void Work() { std::cout << "Base Working" << std::endl; }
};

template<typename Type = int>
class Child : public Base<Type>
{
public:
    Child() { std::cout << "Child Ctor" << std::endl; }
    ~Child() { std::cout << "Child Dtor" << std::endl; }
    virtual void Work() override final { std::cout << "Child Working" << std::endl; }
};
*/

