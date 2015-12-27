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

Group::Group(Group &&other)
    : Object(move(other))
{
    cerr << "Move constructor group" << endl;
}

Group::Group(const Object &other)
{
    constructFromOther(other);
}

Group& Group::operator=(const Object &other)
{
    constructFromOther(other);
    return *this;
}

Group& Group::operator=(const Group &other)
{
    constructFromOther(other);
    return *this;
}

Group& Group::operator=(Group &&other)
{
    Object &otherObject = other;
    Object::operator=(move(otherObject));
    return *this;
}

Group::Group(hid_t id, hid_t parentID, string name)
    : Object(id, parentID, name)
{
}

void Group::constructFromOther(const Object &other)
{
    if(other.id() > 0) {
        H5O_info_t info;
        herr_t err = H5Oget_info(other.id(), &info);
        if(err < 0) {
            cerr << "ERROR: Could not convert object to group. Could not get object info." << endl;
            return;
        }
        if(info.type != H5O_TYPE_GROUP) {
            cerr << "ERROR: Could not convert object to group. Object is not group. Type info: " << info.type << endl;
            return;
        }
        m_id = H5Gopen(other.id(), ".", H5P_DEFAULT);
    #ifdef H5CPP_VERBOSE
        cerr << "Open group from other " << other << " to get " << m_id << endl;
    #endif
    } else {
        m_id = other.id();
    }
    m_name = other.name();
    m_parentID = other.parentID();
}

Group::~Group()
{
}

void Group::close()
{
    if(m_id != 0) {
        H5Gclose(m_id);
#ifdef H5CPP_VERBOSE
        cerr << "Close group " << m_id << endl;
#endif
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

std::vector<Object> Group::items() const
{
    std::vector<Object> returnedItems;
    for(auto key : keys()) {
        returnedItems.push_back(item(key));
    }
    return returnedItems;
}

Object Group::item(string key) const
{
    if(!hasKey(key)) {
        return Object(0, m_id, key);
    }
    hid_t id = H5Oopen(m_id, key.c_str(), H5P_DEFAULT);
#ifdef H5CPP_VERBOSE
    cerr << "Open object " << id << endl;
#endif
    return Object(id, m_id, key);
}

std::vector<std::string> Group::attributeKeys() const
{
    vector<string> returnedAttributes;
    hsize_t idx = 0;
    H5_index_t l = H5_INDEX_NAME; // TODO: Move to function param?
    H5Aiterate(m_id, l, H5_ITER_INC, &idx,
               [&](hid_t g_id, const char *name, const H5A_info_t *info, void *namesVoid) -> herr_t {
        (void)g_id;
        (void)info;
        vector<string> &names = *(vector<string>*)(namesVoid);

        string nameString(name);
        names.push_back(nameString);

        return 0;
    }, &returnedAttributes);

    return returnedAttributes;
}

Object Group::operator[](string key) const
{
    return item(key);
}

Attribute Group::operator()(string key) const
{
    return attribute(key);
}

Attribute Group::attribute(string key) const
{
    if(!hasAttribute(key)) {
        return Attribute(0, m_id, key);
    }
    return Attribute(m_id, key);
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
    if(hasKey(name)) {
        cerr << "ERROR: Cannot create group. An object already exists with name " << name << endl;
        return Group();
    }
    string parentPathName = parentPath(name);
    if(!hasKey(parentPathName)) {
        createGroup(parentPathName);
    }
    hid_t groupID = H5Gcreate(m_id, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
#ifdef H5CPP_VERBOSE
    cerr << "Created group with id " << groupID << endl;
#endif
    return Group(groupID, m_id, name);
}

bool Group::hasKey(string name) const
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

std::vector<Attribute> Group::attributes() const
{
    std::vector<Attribute> returnedAttributes;
    for(std::string key : attributeKeys()) {
        returnedAttributes.emplace_back(attribute(key));
    }
    return returnedAttributes;
}

bool Group::hasAttribute(string name) const
{
    if(H5Aexists(m_id, name.c_str()) != true) {
        return false;
    }
    return true;
}

}
