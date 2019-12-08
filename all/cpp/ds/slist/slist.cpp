
#include "slist.hpp"    // SList
#include <stddef.h>     // size_t

namespace hrd11
{
struct SList::SNode
{
    explicit SNode(int data, SNode* next = 0);
    ~SNode();
    SNode(const SNode& other);
    SNode& operator=(const SNode& other);

    int m_data;
    SNode* m_next;
};
}

using namespace hrd11;

// *********************************************************** SNode Functions:
SList::SNode::SNode(int data, SNode* next): m_data(data), m_next(next)
{
}

SList::SNode::~SNode()
{
    m_data = 0;
    m_next = 0;
}

SList::SNode::SNode(const SNode& other)
{
    m_data = other.m_data;
    m_next = other.m_next;
}

SList::SNode& SList::SNode::operator=(const SNode& other)
{
    m_data = other.m_data;
    m_next = other.m_next;
}

// *********************************************************** SList Functions:
SList::SList(): m_head(0)
{
}

SList::~SList()
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

void SList::PushFront(int data)
{
    SNode* node = new SNode(data, m_head);

    m_head = node;
}

void SList::PopFront()
{
    SNode* node = 0;

    if (m_head)
    {
        node = m_head;
        m_head = node->m_next;
        delete node;
    }
}

int SList::Head() const
{
    if (m_head)
    {
        return m_head->m_data;
    }

    return empty;
}

size_t SList::Size() const
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

bool SList::IsEmpty() const
{
    if (m_head)
    {
        return false;
    }

    return true;
}
