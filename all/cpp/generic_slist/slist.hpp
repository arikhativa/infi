
/*
    SList -

    Discription:    SList (single linked list) is a data-structure
                    based on nodes with data and pointer the the
                    'next' node.
    Date:           8.12.19
    File names:     slist.cpp, slist.hpp

    Reviewer:   Shahar

    Ver 1.1 - changed const
    Ver 1.0
*/

#ifndef __HRD11_SList_HPP__
#define __HRD11_SList_HPP__

#include <stddef.h>     // size_t

namespace hrd11
{
template <typename T>
class SList             // Uncopyable
{
public:
    static const T empty = 0;

    explicit SList();
    ~SList();

    void PushFront(T data);
    void PopFront();

    /*
        Head():
        Return value:
                        On Success - 'data'
                        On Failure - 'empty' (-1)
    */
    T Head() const;
    size_t Size() const;
    bool IsEmpty() const;

private:
    struct SNode;

    SList(const SList& other);
    SList& operator=(const SList& other);

    SNode* m_head;
};


// ********************************************************* SNode Defenishion:
template <typename T>
struct SList<T>::SNode
{
    explicit SNode(T data, SNode* next = 0);
    ~SNode();
    SNode(const SNode& other);
    SNode& operator=(const SNode& other);

    T m_data;
    SNode* m_next;
};


// *********************************************************** SNode Functions:
template <typename T>
SList<T>::SNode::SNode(T data, SNode* next): m_data(data), m_next(next)
{
}

template <typename T>
SList<T>::SNode::~SNode()
{
    m_data = 0;
    m_next = 0;
}

template <typename T>
SList<T>::SNode::SNode(const SNode& other): m_data(other.m_data), m_next(other.m_next)
{}


template <typename T>
typename SList<T>::SNode& SList<T>::SNode::operator=(const SNode& other)
{
    m_data = other.m_data;
    m_next = other.m_next;
}


// *********************************************************** SList Functions:
template <typename T>
SList<T>::SList(): m_head(0)
{}

template <typename T>
SList<T>::~SList()
{
    SNode* node = m_head;
    SNode* next = 0;

    while (node)
    {
        next = node->m_next;
        delete node;
        node = next;
    }

    m_head = 0;
}

template <typename T>
void SList<T>::PushFront(T data)
{
    SNode* node = new SNode(data, m_head);

    m_head = node;
}

template <typename T>
void SList<T>::PopFront()
{
    SNode* node = 0;

    if (!(IsEmpty()))
    {
        node = m_head;
        m_head = node->m_next;
        delete node;
    }
}

template <typename T>
T SList<T>::Head() const
{
    if (!(IsEmpty()))
    {
        return m_head->m_data;
    }

    return empty;
}

template <typename T>
size_t SList<T>::Size() const
{
    SNode* run = m_head;
    size_t size = 0;

    while (run)
    {
        ++size;
        run = run->m_next;
    }

    return size;
}

template <typename T>
bool SList<T>::IsEmpty() const
{
    return !(m_head);
}
}

#endif //__HRD11_SList_HPP__
