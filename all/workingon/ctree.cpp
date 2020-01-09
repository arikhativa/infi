

#include <stdexcept>    // std::runtime_error
#include <algorithm>    // std::for_each(), std::sort()
#include <string>       // std::string
#include <iostream>     // ostream
#include <typeinfo>     // typeid

#include <cstring>      // strcmp

#include "color.h"
#include "ctree.hpp"
#include "dir_handle.hpp"


namespace hrd11
{

// ************************************ Globals *******************************

static ssize_t s_num_of_dirs = -1;
static ssize_t s_num_of_files;

// ************************************ File **********************************

File::File(const std::string& name, std::string color) :
        m_name(name),
        m_error(""),
        m_color(color)
{}

void File::Display(std::ostream& os, size_t space) const
{
    int sss = space;

    ++s_num_of_files;

    while(0 < sss - 2)
    {
        os << "|   ";
        --sss;
    }
    while(0 < sss - 1)
    {
        os << "|-- ";
        --sss;
    }

    os << m_color << m_name << RESET;
    os << m_error;
    os << std::endl;
}

const std::string& File::GetCopyOfFileName() const
{
    return m_name;
}

void File::AddError(const std::string& error)
{
    m_error += error;
}

// ************************************ File::UseDisplay **********************

struct File::UseDisplay
{
    UseDisplay(std::ostream& os, size_t space) : m_os(os), m_space(space)
    {}

    void operator()(std::shared_ptr<File> file)
    {
        file->Display(m_os, m_space);
    }

    std::ostream& m_os;
    size_t m_space;
};

// ************************************ Directory *****************************

// ************************************ Directory  Public *********************

std::shared_ptr<const Directory> Directory::CreateInstance(const std::string& path)
{
    std::shared_ptr<Directory> ret (new Directory(path));

    // std::shared_ptr<Directory> ret = std::make_shared<Directory> (path);

    {
        DirHandle handle(path);

        if (handle.IsOpen())
        {
            AddFilesToDirectory(ret, handle);
        }
        else
        {
            ret->AddError(" [error opening dir]");
        }
    }

    LookAndCreateSubDirectorys(ret, path);

    return ret;
}

void Directory::Display(std::ostream& os, size_t space) const
{
    File::Display(os, space);

    --s_num_of_files;
    ++s_num_of_dirs;

    std::for_each(m_vec.begin(), m_vec.end(), File::UseDisplay(os, space + 1));

    if (1 == space)
    {
        os << std::endl;
        os << s_num_of_dirs << " directories, ";
        os << s_num_of_files << " files";
        os << std::endl;
    }
}

std::shared_ptr<const Directory> Directory::Find(const std::string& dir_name) const
{
    using namespace std;

    vector<shared_ptr<File>>::const_iterator start = m_vec.begin();
    vector<shared_ptr<File>>::const_iterator end = m_vec.end();
    shared_ptr<const Directory>* ttt = 0;

    File* base = 0;
    Directory* dc_value = 0;

    while (start != end)
    {
        base = (*start).get();
        dc_value = dynamic_cast<Directory*> (base);

        if (dc_value)
        {
            ttt = (std::shared_ptr<const Directory>*)&(*start);

            if ((*start)->GetCopyOfFileName() == dir_name)
            {
                return *ttt;
            }
            else
            {
                if (*ttt = (*ttt)->Find(dir_name))
                {
                    return *ttt;
                }
            }
        }
        ++start;
    }

    return 0;
}

// ************************************ Directory  Private ********************

Directory::Directory(const std::string& name, std::string color) :
        File(name, color)
{}

void Directory::Add(std::shared_ptr<File> ptr)
{
    m_vec.push_back(ptr);
}

void Directory::AddFilesToDirectory(
    std::shared_ptr<Directory> dir, const DirHandle& handle)
{
    const struct dirent *entry = NULL;

    entry = handle.GetEntry();

    if (!entry)
    {
        dir->AddError(" [error opening dir]");
    }

    while (entry != NULL)
    {
        if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
        {
            if (DT_DIR == entry->d_type)
            {
                dir->Add(std::shared_ptr<File>(new Directory(entry->d_name)));
            }
            else
            {
                dir->Add(std::shared_ptr<File>(new File(entry->d_name)));
            }
        }

        entry = handle.GetEntry();
    }
}

bool File::SortByAlpha(std::shared_ptr<File> f1, std::shared_ptr<File> f2)
{
    int i = 0;
    const std::string& s1 = f1->m_name;
    const std::string& s2 = f2->m_name;

    for (i = 0; s1[i] && s1[i] == s2[i]; ++i)
    {}

    return !(tolower(s1[i]) > tolower(s2[i]));
}

void Directory::SortDirByAlpha()
{
    using namespace std;


    sort(m_vec.begin(), m_vec.end(), SortByAlpha);
}

void Directory::LookAndCreateSubDirectorys(
    std::shared_ptr<Directory> dir, const std::string& path)
{
    using namespace std;

    shared_ptr<Directory>* ptr_for_casting;
    string new_path;
    vector<shared_ptr<File>>::const_iterator start = dir->m_vec.begin();
    vector<shared_ptr<File>>::const_iterator end = dir->m_vec.end();
    File* base = 0;
    Directory* dc_value = 0;

    dir->SortDirByAlpha();

    while (start != end)
    {
        base = (*start).get();
        dc_value = dynamic_cast<Directory*> (base);

        if (dc_value)   // successful downcast
        {
            ptr_for_casting = (shared_ptr<Directory>*)(&(*start));

            new_path = path + "/" + (*ptr_for_casting)->GetCopyOfFileName();

            Directory::AddFilesToDirectory(*ptr_for_casting, DirHandle(new_path));

            LookAndCreateSubDirectorys(*ptr_for_casting, new_path);
        }

        ++start;
    }
}


}   // namespace hrd11
