#include "slist.hpp"

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

//****************************************SNODE*********************************************
SList::SNode::SNode(int data, SNode* next): m_data(data), m_next(next)
{}

SList::SNode::~SNode(){}

SList::SNode::SNode(const SNode& other): m_data(other.m_data), m_next(other.m_next)
{}

SList::SNode& SList::SNode::operator=(const SNode& other)
{
    m_data = other.m_data;
    m_next = other.m_next;

    return *this;
}


//****************************************SLIST*********************************************

SList::SList(): m_head (0) {}

SList::~SList()
{
    SNode* tmp = 0;

    while(m_head)
    {
        tmp = m_head;
        m_head = m_head->m_next;
        delete tmp;
    }
}

void SList::PushFront(int a)
{
    SNode* new_node = new SNode(a, m_head);
    m_head = new_node;
}

void SList::PopFront()
{
    SNode* tmp = m_head;
    if (m_head)
    {
        m_head = m_head->m_next;
        delete tmp;
    }
}

int SList::Head() const
{
    if (0 == m_head)
    {
        return empty;
    }

    return m_head->m_data;
}

size_t SList::Size() const
{
    size_t counter = 0;
    SNode* runner = m_head;
    while (runner)
    {
        ++counter;
        runner = runner->m_next;
    }

    return counter;
}

bool SList::IsEmpty() const
{
    return (0 == m_head);
}
}