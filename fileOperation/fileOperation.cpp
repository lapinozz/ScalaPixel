#include "fileOperation.hpp"

#include <sstream>
#include <cstring>

#include <stdlib.h>
#include <stdio.h>

#include "../portable/dirent_portable.h"
#include <sys/stat.h>
#include <sys/types.h>

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems)
{
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}


bool isFolder(std::string path)
{
    struct stat s;
    if(stat(path.c_str(), &s) == 0)
        return (s.st_mode & S_IFDIR );
    else
        return 0; //dosent even exist
//        throw ("error getting stat of: " + path);
}

bool isFile(std::string path)
{
    struct stat s;
    if(stat(path.c_str(), &s) == 0)
        return (s.st_mode & S_IFREG);
    else
        return 0; //dosent even exist
//        throw ("error getting stat of: " + path);
}

std::vector<std::string> getFilesInFolder(std::string path)
{
    std::vector<std::string> files;

    DIR* dirp = opendir(path.c_str());
    dirent* dp;
    while((dirp != NULL) && ((dp = readdir(dirp)) != NULL))
    {
        if (dp->d_type == DT_REG)
            files.push_back(std::string(dp->d_name));
    }

    (void)closedir(dirp);
    return files;
}

std::vector<std::string> getFolderInFolder(std::string path)
{
    std::vector<std::string> folders;
    DIR* dirp = opendir(path.c_str());
    dirent* dp;

    while((dp = readdir(dirp)) != NULL)
    {
        if (dp->d_type == DT_DIR && strcmp(dp->d_name, ".") && strcmp(dp->d_name, ".."))
            folders.push_back(dp->d_name);
    }

    (void)closedir(dirp);
    return folders;
}
