#include "group.h"

#include <sstream>
#include <iostream>
#include <regex>

#include "object.h"
#include "dataset.h"

using namespace std;

namespace hdf5 {

Group::Group()
    : Object()
{
}

Group::Group(const Object &other)
    : Object(other)
{
    constructFromOther(other);
}

void Group::operator=(const Object &other)
{
    constructFromOther(other);
}

void Group::operator=(const Group &other)
{
    constructFromOther(other);
}

Group::Group(hid_t id, string name)
    : Object(id, name)
{
    //    m_id = H5Gopen(id, name.c_str(), H5P_DEFAULT);
}

void Group::constructFromOther(const Object &other)
{
    m_id = H5Gopen(other.id(), ".", H5P_DEFAULT);
}

Group::~Group()
{
    if(m_id != 0) {
        cout << "Closing group with ID " << m_id << endl;
        H5Gclose(m_id);
        m_id = 0;
    }
}

std::vector<std::string> Group::keys() const
{
    vector<string> returnedKeys;
    hsize_t idx = 0;
    H5_index_t l = H5_INDEX_NAME;
    H5Literate(m_id, l, H5_ITER_INC, &idx,
               [&](hid_t g_id, const char *name, const H5L_info_t *info, void *namesVoid) -> herr_t {
        (void)g_id;
        (void)info;
        vector<string> &names = *(vector<string>*)(namesVoid);

        string nameString(name);
        names.push_back(nameString);

        return 0;
    }, &returnedKeys);

    return returnedKeys;
}

Object Group::operator[](string key) const
{
    if(!contains(key)) {
        cerr << "ERROR: Cannot find object for key " << key << endl;
        return Object();
    }
    hid_t id = H5Oopen(m_id, key.c_str(), H5P_DEFAULT);
    return Object(id, key);
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elements;
    std::string item;

    std::stringstream ss(s);
    while (std::getline(ss, item, delim)) {
        if(!item.empty()) {
            elements.push_back(item);
        }
    }
    return elements;
}

std::string parentPath(string &path)
{
    vector<string> splitPath = split(path, '/');
    int pathCount = splitPath.size();
    if(pathCount < 2) {
        return "";
    }
    std::stringstream fullPath;
    int count = 0;
    for(const string &part : splitPath) {
        fullPath << part;
        count += 1;
        if(count >= pathCount - 1) {
            break;
        }
        fullPath << "/";
    }
    return fullPath.str();
}


Group Group::createGroup(string name)
{
    if(!isValid()) {
        cerr << "ERROR: Cannot create group in invalid object " << *this << endl;
        return Group();
    }
    if(contains(name)) {
        cerr << "ERROR: Cannot create group. An object already exists with name " << name << endl;
        return Group();
    }
    string parentPathName = parentPath(name);
    if(!contains(parentPathName)) {
        createGroup(parentPathName);
    }
    hid_t groupID = H5Gcreate(m_id, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    return Group(groupID, name);
}

bool Group::contains(string name) const
{
    std::vector<std::string> paths = split(name, '/');
    std::stringstream fullPath;
    for(const std::string &part : paths) {
        fullPath << part;
        if(H5Lexists(m_id, fullPath.str().c_str(), H5P_DEFAULT) != true) {
            return false;
        }
        fullPath << "/";
    }
    return true;
}

}
