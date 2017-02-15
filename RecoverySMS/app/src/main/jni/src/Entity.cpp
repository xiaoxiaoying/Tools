#include <string>
#include <map>
using namespace std;

#include "../include/Entity.h"
using namespace hudun::sqlite;

Entity::Entity(uint16_t obtainWay) noexcept
{
    this->obtainWay     = obtainWay;
    this->reset();
}

Entity::Entity(Entity const& other) noexcept
{
    this->obtainWay     = other.obtainWay;
    this->validFlags    = other.validFlags;
    this->nullFlags     = other.nullFlags;
    this->deletedStatus = other.deletedStatus;
}

Entity& Entity::operator=(Entity const& other) noexcept
{
    if(this != &other)
    {
        this->obtainWay     = other.obtainWay;
        this->validFlags    = other.validFlags;
        this->nullFlags     = other.nullFlags;
        this->deletedStatus = other.deletedStatus;
    }
    return *this;
}

void Entity::setNullValue(uint64_t fieldFlag) noexcept
{
    this->nullFlags |= fieldFlag;
    this->validFlags |= fieldFlag;
}

void Entity::setDeletedStatus(uint16_t deletedStatus) noexcept
{
    this->deletedStatus = deletedStatus;
}

void Entity::addAuxiliary(string const& name, string const& value) noexcept
{
    this->auxiliaries.emplace(name, value);
}

bool Entity::isValid(uint64_t fieldFlags) const noexcept
{
    return ((this->validFlags & fieldFlags) == fieldFlags);
}

bool Entity::isNull(uint64_t fieldFlag) const noexcept
{
    return ((this->nullFlags & fieldFlag) == fieldFlag);
}

bool Entity::isValidAndNotNull(uint64_t fieldFlag) const noexcept
{
    return (((this->validFlags & fieldFlag) == fieldFlag) && ((this->nullFlags & fieldFlag) != fieldFlag));
}

uint16_t Entity::getObtainWay() const noexcept
{
    return this->obtainWay;
}

uint64_t Entity::getValidFlags() const noexcept
{
    return this->validFlags;
}

uint16_t Entity::getDeletedStatus() const noexcept
{
    return this->deletedStatus;
}

string Entity::getAuxiliary(string const& name, bool& exist) const noexcept
{
    map<string, string>::const_iterator it = this->auxiliaries.find(name);
    if(it == this->auxiliaries.end())
    {
        exist = false;
        return "";
    }
    else
    {
        exist = true;
        return it->second;
    }
}

const map<string, string>& Entity::getAuxiliaries() const noexcept
{
    return this->auxiliaries;
}

void Entity::reset() noexcept
{
    this->validFlags    = 0;
    this->nullFlags     = 0;
    this->deletedStatus = Entity::DELETEDSTATUS_UNKNOWN;
}

