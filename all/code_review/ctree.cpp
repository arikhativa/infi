#include <sys/types.h>  // DIR*
#include <dirent.h>     // DIR*, struct dirent*, ENOTDIR
#include <errno.h>      // errno EACCES
#include <algorithm>    // for_each, sort

#include "ctree.hpp"
#include "colors.h"


namespace hrd11
{
size_t Directory::m_num_dirs = 0;
size_t Directory::m_num_files = 0;

class DirHandle
{
public:
    explicit DirHandle(const std::string& path);
    ~DirHandle();

    const struct dirent* GetEntry() const;
    bool IsOpen() const;

private:
    // no cctor and copy=
    DirHandle(const DirHandle& other);
    DirHandle& operator=(const DirHandle& other);

    DIR* m_handle;
};

// --------------------- Functors -----------------------------

struct Directory::DisplayTree
{
    DisplayTree(std::ostream& os, int space) : m_os(os), m_space(space)
    {}

    void operator() (std::shared_ptr<File> file)
    {
        file->Display(m_os, m_space);
    }

    std::ostream& m_os;
    int m_space;
};

// -------------------- File Functions --------------------------
File::File(const std::string& name, const std::string& color) : m_name(name), m_color(color)
{}

void File::Display(std::ostream& os, int space) const
{
    while(space > 1)
    {
        os << "    |";
        --space;
    }

    if(space)
        os << "|--";

    os << m_color;  // print by file color
    if(!space)
        os << m_name << std::endl; // print full path

    else
        os << GetName() << std::endl;
    os << NORM;     // reset print color
}

bool File::IsBefore(std::shared_ptr<File> file1, std::shared_ptr<File> file2)
{
    return file1->GetName()[0] < file2->GetName()[0];
}

std::string File::GetName() const
{
    // Get new string from last '/' (not included) until the end.
    return m_name.substr(m_name.rfind("/") + 1);
}

// -------------------- Directory Functions --------------------------

Directory::Directory(const std::string& path, const std::string& color) :
File(path, color)
{}

std::shared_ptr<Directory> Directory::CreateInstance(const std::string& path)
{
    DirHandle dir(path);

    if(!dir.IsOpen())
    {
        return std::shared_ptr<Directory>(new Directory(path + " [error opening dir]", NORM));
    }

    std::shared_ptr<Directory> root(new Directory(path));
    root->RecFillDirectory(dir, path);
    std::sort(root->m_vec.begin(), root->m_vec.end(), File::IsBefore);

    return root;
}

void Directory::RecFillDirectory(const DirHandle& dir, const std::string& path)
{
    const struct dirent* new_file_struct = dir.GetEntry();

    while(new_file_struct)
    {
        if('.' != new_file_struct->d_name[0])
        {
            if(DT_DIR == new_file_struct->d_type)
            {
                ++m_num_dirs;
                Add(CreateInstance(path + "/" + new_file_struct->d_name));
                // recursive call to add a new directory to current directory.
            }

            else
            {
                ++m_num_files;
                Add(std::shared_ptr<File>(new File(new_file_struct->d_name)));
                // add file to directory.
            }
        }

        new_file_struct = dir.GetEntry(); // get next entry.
    }
}

void Directory::Display(std::ostream& os, int space) const
{
    File::Display(os, space);

    ++space;
    std::for_each(m_vec.begin(), m_vec.end(), DisplayTree(os, space));
    --space;

    if(!space) // summary
        os << "\n\ndirectories: " << m_num_dirs << "    files: " << m_num_files << "\n";
}

std::shared_ptr<const File> Directory::Find(const std::string& path) const
{
    size_t i = 0;
    std::shared_ptr<Directory> tmp;

    for(i = 0; i < m_vec.size(); ++i)
    {
        // size_t(-1) - return val of std::string.find()
        if(size_t(-1) != path.find(m_vec[i]->GetName())) // curr-file-name is part of 'path'.
        {
            // check if file is found.
            if(m_vec[i]->GetName() == path.substr(path.rfind("/") + 1))
            {
                return m_vec[i];
            }

            // check if directory.
            tmp = std::dynamic_pointer_cast<Directory>(m_vec[i]);
            if(tmp)
            {
                // recursive call to check inside directory
                return tmp->Find(path);
            }
        }
    }

    return 0;
}

void Directory::Add(std::shared_ptr<File> new_file)
{
    m_vec.push_back(new_file);
}

// -------------------- DirHandle Functions --------------------------

DirHandle::DirHandle(const std::string& path) : m_handle(opendir(path.c_str()))
{}

DirHandle::~DirHandle()
{
    closedir(m_handle);
}

const struct dirent* DirHandle::GetEntry() const
{
    return readdir(m_handle);
}

bool DirHandle::IsOpen() const
{
    return m_handle != 0;
}

} // namespace hrd11
