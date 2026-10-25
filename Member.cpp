
#include "Member.h"

Member::Member(const std::string& id,
               const std::string& name,
               const std::string& email)
    : id(id), name(name), email(email) {
}

std::string Member::getId() const {
    return id;
}

std::string Member::getName() const {
    return name;
}

std::string Member::getEmail() const {
    return email;
}

void Member::setName(const std::string& name) {
    this->name = name;
}

void Member::setEmail(const std::string& email) {
    this->email = email;
}
