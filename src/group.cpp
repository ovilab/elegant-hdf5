#include "group.h"

#include <sstream>
#include <iostream>
#include <regex>

#include "object.h"
#include "dataset.h"
#include "attribute.h"

using namespace std;

namespace h5cpp {

Group::Group()
    : Object()
{
}

Group::Group(const Group &other)
    : Object()
{
    constructFromOther(other);
}

//Group::Group(Group &&other)
//    : Object(move(other))
//{
//#ifdef H5CPP_VERBOSE
//    DVLOG(1) << "Move constructor group";
//#endif
//}

Group::~Group()
{
    close();
}

Group::Group(const Object &other)
{
    constructFromOther(other);
}

Group& Group::operator=(const Object &other)
{
    Object::operator =(other);
    return *this;
}

Group& Group::operator=(const Group &other)
{
    Object::operator =(other);
    return *this;
}

//Group& Group::operator=(Group &&other)
//{
//    Object &otherObject = other;
//    Object::operator=(move(otherObject));
//    return *this;
//}

Group::Group(hid_t id, hid_t parentID, string name)
    : Object(id, parentID, name)
{
}

vector<string> Group::keys() const
{
    if(!isValid()) {
        DVLOG(1) << "Object is not valid. Cannot request list of keys. " << *this;
        return vector<string>();
    }
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

vector<Object> Group::items() const
{
    vector<Object> returnedItems;
    for(auto key : keys()) {
        returnedItems.push_back(item(key));
    }
    return returnedItems;
}

Object Group::item(string key) const
{
    if(!isValid()) {
        throw(std::runtime_error("Requested key from from invalid group object"));
    }
    if(!hasKey(key)) {
        return Object(0, m_id, key);
    }
    hid_t id = H5Oopen(m_id, key.c_str(), H5P_DEFAULT);
    DVLOG(1) << "Open object " << key << " as " << id;
    return Object(id, m_id, key);
}

Object Group::operator[](string key) const
{
    return item(key);
}

vector<string> split(const string &s, char delim)
{
    vector<string> elements;
    string item;

    stringstream ss(s);
    while (getline(ss, item, delim)) {
        if(!item.empty()) {
            elements.push_back(item);
        }
    }
    return elements;
}

string parentPath(string &path)
{
    vector<string> splitPath = split(path, '/');
    int pathCount = splitPath.size();
    if(pathCount < 2) {
        return "";
    }
    stringstream fullPath;
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
        throw(std::runtime_error("Cannot create group in invalid object"));
    }
    if(hasKey(name)) {
        if(item(name).type() == Type::Group) {
            DVLOG(1) << "WARNING: Group already exists with name " << name;
        } else {
            throw(std::runtime_error("Cannot create group. A non-group object already exists with that name."));
        }
        return Group();
    }
    string parentPathName = parentPath(name);
    if(!hasKey(parentPathName)) {
        createGroup(parentPathName);
    }
    hid_t groupID = H5Gcreate(m_id, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    if(groupID < 1) {
        throw runtime_error("Could not create group");
    }
    DVLOG(1) << "Created group with id " << groupID;
    return Group(groupID, m_id, name);
}

bool Group::hasKey(string name) const
{
    if(!isValid()) {
        return false;
    }
    vector<string> paths = split(name, '/');
    stringstream fullPath;
    for(const string &part : paths) {
        fullPath << part;
        if(H5Lexists(m_id, fullPath.str().c_str(), H5P_DEFAULT) != true) {
            return false;
        }
        fullPath << "/";
    }
    return true;
}

}
