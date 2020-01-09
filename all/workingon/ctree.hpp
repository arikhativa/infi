
/*
    Composite Tree -

    Discription:    API for using composite tree
    Date:           8.1.2020
    Folder name:    system_programing/ctree
    File names:     ctree.hpp, ctree.cpp, ctree_test.cpp

    Reviewer:       Aviv

    Ver 1.0
*/

#ifndef __HRD11_CTREE_HPP__
#define __HRD11_CTREE_HPP__

#include <vector>       // vector
#include <iostream>     // ostream
#include <memory>       // shared_ptr
#include <string>       // std::string

#include "dir_handle.hpp"
#include "color.h"

namespace hrd11
{

// Directory and File are composite design pattern
class File
{
public:
    explicit File(const std::string& name, std::string color = RESET);
    virtual ~File() = default; // c++11
    File(const File& other) = delete; // c++11
    File& operator=(const File& other) = delete; // c++11 read on '= delete'

    virtual void Display(std::ostream& os, size_t space) const;

    const std::string& GetCopyOfFileName() const;

protected:
    struct UseDisplay;

    static bool SortByAlpha(std::shared_ptr<File> f1, std::shared_ptr<File> f2);
    void AddError(const std::string& error);

private:
    std::string m_name;
    std::string m_error;
    std::string m_color;

};

class Directory : public File // read about 'final'
{
public:
    // can throw(std::bad_alloc)
    static std::shared_ptr<const Directory> CreateInstance(const std::string& path);

    virtual ~Directory() = default;
    // no cctor and copy=

    void Display(std::ostream& os, size_t space) const override; // c++11

    std::shared_ptr<const Directory> Find(const std::string& dir_name) const;

private:
    Directory(const std::string& name, std::string color = BBLUE);

    void Add(std::shared_ptr<File>);
    void SortDirByAlpha();
    static void AddFilesToDirectory(std::shared_ptr<Directory> dir, const DirHandle& handle);
    static void LookAndCreateSubDirectorys(std::shared_ptr<Directory> dir, const std::string& path);

    std::vector< std::shared_ptr<File> > m_vec;
};

}   // namespace hrd11

#endif // __HRD11_CTREE_HPP__
