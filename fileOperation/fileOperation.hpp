#pragma once

#include <string>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);

bool isFolder(std::string path);
bool isFile(std::string path);

std::vector<std::string> getFilesInFolder(std::string path);
std::vector<std::string> getFolderInFolder(std::string path);
