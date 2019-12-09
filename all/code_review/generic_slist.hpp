
/*
    SList -

    Discription:    SList (single linked list) is a data-structure
                    based on nodes with data and pointer the the
                    'next' node.
    Date:           8.12.19
    File names:     SList.cpp, SList.hpp

    Reviewer:       Yoav

    Ver 1.0
*/

#ifndef __HRD11_GSList_HPP__
#define __HRD11_GSList_HPP__

#include <stddef.h>     // size_t

namespace hrd11
{
template <class T>
class SList // Uncopyable
{
public:
    static const int empty = -1;

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
    struct GSNode;

    SList(const SList& other);
    SList& operator=(const SList& other);

    GSNode* m_head;
};

template <class T>
SList<T>::SList(): m_head (0) {}

template <class T>
SList<T>::~SList()
{
    GSNode* tmp = 0;

    while(m_head)
    {
        tmp = m_head;
        m_head = m_head->m_next;
        delete tmp;
    }
}

template <class T>
void SList<T>::PushFront(T a)
{
    GSNode* new_node = new GSNode(a, m_head);
    m_head = new_node;
}

template <class T>
void SList<T>::PopFront()
{
    GSNode* tmp = m_head;
    if (m_head)
    {
        m_head = m_head->m_next;
        delete tmp;
    }
}

template <class T>
T SList<T>::Head() const
{
    if (0 == m_head)
    {
        return empty;
    }

    return m_head->m_data;
}

template <class T>
size_t SList<T>::Size() const
{
    size_t counter = 0;
    GSNode* runner = m_head;
    while (runner)
    {
        ++counter;
        runner = runner->m_next;
    }

    return counter;
}

template <class T>
bool SList<T>::IsEmpty() const
{
    return (0 == m_head);
}

//****************************************SNODE*********************************************

template <class T>
struct SList<T>::GSNode
{
    explicit GSNode(T data, GSNode* next = 0);
    ~GSNode();
    GSNode(const GSNode& other);
    GSNode& operator=(const GSNode& other);

    T m_data;
    GSNode* m_next;
};

template <class T>
SList<T>::GSNode::GSNode(T data, GSNode* next): m_data(data), m_next(next)
{}

template <class T>
SList<T>::GSNode::~GSNode(){}

template <class T>
SList<T>::GSNode::GSNode(const GSNode& other): m_data(other.m_data), m_next(other.m_next)
{}

template <class T>

typename SList<T>::GSNode& SList<T>::GSNode::operator=(const GSNode& other)
{
    m_data = other.m_data;
    m_next = other.m_next;

    return *this;
}



}

#endif //__HRD11_GSList_HPP__
