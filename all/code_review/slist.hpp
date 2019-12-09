
/*
    SList -

    Discription:    SList (single linked list) is a data-structure
                    based on nodes with data and pointer the the
                    'next' node.
    Date:           8.12.19
    File names:     slist.cpp, slist.hpp

    Reviewer:       Yoav

    Ver 1.0
*/

#ifndef __HRD11_SList_HPP__
#define __HRD11_SList_HPP__

#include <stddef.h>     // size_t

namespace hrd11
{
class SList // Uncopyable
{
public:
    static const int empty = -1;

    explicit SList();
    ~SList();

    void PushFront(int data);
    void PopFront();
    
    /*
        Head():
        Return value:
                        On Success - 'data'
                        On Failure - 'empty' (-1)
    */
    int Head() const;
    size_t Size() const;
    bool IsEmpty() const;

private:
    struct SNode;

    SList(const SList& other);
    SList& operator=(const SList& other);

    SNode* m_head;
};
}

#endif //__HRD11_SList_HPP__

