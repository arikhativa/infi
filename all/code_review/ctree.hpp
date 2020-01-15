
/*
    Composite Tree -

    Discription:    API for using composite tree
    Date:           8.1.2020
    Folder name:    system_programing/ctree
    File names:     ctree.hpp, ctree.cpp, ctree_test.cpp

    Reviewer:

    Ver 1.0
*/

#ifndef __HRD11_CTREE_HPP__
#define __HRD11_CTREE_HPP__


#include <cstddef>      // size_t
#include <vector>       // vector
#include <iostream>     // ostream
#include <memory>       // shared_ptr

#include "colors.h"

namespace hrd11
{

class DirHandle;

// Directory and File are composite design pattern
class File
{
public:
    explicit File(const std::string& name, const std::string& color = NORM);
    virtual ~File() = default; // c++11
    File(const File& other) = delete; // c++11
    File& operator=(const File& other) = delete; // c++11 read on '= delete'

    virtual void Display(std::ostream& os, int space = 0) const;

    // for sorting files by alpha-bet
    static bool IsBefore(std::shared_ptr<File> file1, std::shared_ptr<File> file2);
    std::string GetName() const;    // return last file name from path.

private:
    std::string m_name;
    std::string m_color;
};

class Directory : public File
{
private:
    struct DisplayTree;

public:
    static std::shared_ptr<Directory> CreateInstance(const std::string& path);
    virtual ~Directory() = default;
    // no cctor and copy=

    void RecFillDirectory(const DirHandle& dir, const std::string& path);
    void Display(std::ostream& os, int space = 0) const override; // c++11

    std::shared_ptr<const File> Find(const std::string& path) const;

private:
    Directory(const std::string& path, const std::string& color = BBLUE);
    void Add(std::shared_ptr<File>);

    std::vector< std::shared_ptr<File> > m_vec;
    static size_t m_num_dirs;
    static size_t m_num_files;
};

}   // namespace hrd11

#endif // __HRD11_CTREE_HPP__
