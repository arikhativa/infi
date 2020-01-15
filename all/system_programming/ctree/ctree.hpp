
/*
    Composite Tree -

    Discription:    API for using composite tree
                    ctree is imatating the application tree.
                    it will sotre a data structure represented by tree
                    on the heep.
                    the data will be the directoriys and the files "under"
                    the path the user pass to CreateInstance().
                    the user then can print the tree and look for directoris in
                    it using Display() and Find().
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
    virtual ~File() = default;
    File(const File& other) = delete;
    File& operator=(const File& other) = delete;

    // 'space' should allways be 1.
    virtual void Display(std::ostream& os, ssize_t space = 1) const;
    const std::string& GetRefrenceFileName() const;

protected:
    struct FunctorDisplay;

    static bool SortByAlpha(std::shared_ptr<File> f1, std::shared_ptr<File> f2);
    void AddError(const std::string& error);

private:
    std::string m_name;
    std::string m_error;
    std::string m_color;

};

class Directory : public File
{
public:
    // can throw(std::bad_alloc)
    static std::shared_ptr<const Directory> CreateInstance(const std::string& path);

    virtual ~Directory() = default;
    // no cctor and copy= - class File is uncopyable

    // 'space' should allways be 1.
    void Display(std::ostream& os, ssize_t space = 1) const override;
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
