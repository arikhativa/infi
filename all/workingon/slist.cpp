
#include "slist.hpp"    // SList
#include <stddef.h>     // size_t

namespace hrd11
{
template <typename type>
struct SList<type>::SNode
{
    explicit SNode(type data, SNode* next = 0);
    ~SNode();
    SNode(const SNode& other);
    SNode& operator=(const SNode& other);

    type m_data;
    SNode* m_next;
};
}

using namespace hrd11;

// *********************************************************** SNode Functions:
template <typename type>
SList<type>::SNode::SNode(type data, SNode* next): m_data(data), m_next(next)
{
}

template <typename type>
SList<type>::SNode::~SNode()
{
    m_data = 0;
    m_next = 0;
}

template <typename type>
SList<type>::SNode::SNode(const SNode& other)
{
    m_data = other.m_data;
    m_next = other.m_next;
}

template <typename type>
typename SList<type>::SNode& SList<type>::SNode::operator=(const SNode& other)
{
    m_data = other.m_data;
    m_next = other.m_next;
}

// *********************************************************** SList Functions:
template <typename type>
SList<type>::SList(): m_head(0)
{
}

template <typename type>
SList<type>::~SList()
{
    SNode* node = 0;
    SNode* next = 0;

    if (!m_head)
    {
        return ;
    }

    node = m_head;

    while (node)
    {
        next = node->m_next;
        delete node;
        node = next;
    }

    m_head = 0;
}

template <typename type>
void SList<type>::PushFront(type data)
{
    SNode* node = new SNode(data, m_head);

    m_head = node;
}

template <typename type>
void SList<type>::PopFront()
{
    SNode* node = 0;

    if (m_head)
    {
        node = m_head;
        m_head = node->m_next;
        delete node;
    }
}

template <typename type>
type SList<type>::Head() const
{
    if (m_head)
    {
        return m_head->m_data;
    }

    return empty;
}

template <typename type>
size_t SList<type>::Size() const
{
    SNode* run = 0;
    size_t size = 0;

    if (m_head)
    {
        run = m_head;
        while (run)
        {
            ++size;
            run = run->m_next;
        }
    }

    return size;
}

template <typename type>
bool SList<type>::IsEmpty() const
{
    if (m_head)
    {
        return false;
    }

    return true;
}
