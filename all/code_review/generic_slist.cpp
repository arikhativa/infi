#include "generic_slist.hpp"

namespace hrd11
{

template <class T>
struct GSList::GSNode
{
    explicit GSNode(T data, GSNode* next = 0);
    ~GSNode();
    GSNode(const GSNode& other);
    GSNode& operator=(const GSNode& other);

    T m_data;
    GSNode* m_next;
};

template <class T>
GSList::GSNode::GSNode(T data, GSNode* next): m_data(data), m_next(next)
{}

GSList::GSNode::~GSNode(){}

GSList::GSNode::GSNode(const GSNode& other): m_data(other.m_data), m_next(other.m_next)
{}

GSList::GSNode& GSList::GSNode::operator=(const GSNode& other)
{
    m_data = other.m_data;
    m_next = other.m_next;

    return *this;
}

GSList::GSList(): m_head (0) {}

GSList::~GSList()
{
    GSNode* tmp = 0;

    while(m_head)
    {
        tmp = m_head;
        m_head = m_head->m_next;
        delete tmp;
    }
}

void GSList::PushFront(int a)
{
    GSNode* new_node = new GSNode(a, m_head);
    m_head = new_node;
}

void GSList::PopFront()
{
    GSNode* tmp = m_head;
    if (m_head)
    {
        m_head = m_head->m_next;
        delete tmp;
    }
}

int GSList::Head() const
{
    if (0 == m_head)
    {
        return empty;
    }

    return m_head->m_data;
}

size_t GSList::Size() const
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

bool GSList::IsEmpty() const
{
    return (0 == m_head);
}
}
