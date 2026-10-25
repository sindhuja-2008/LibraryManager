
#ifndef MEMBER_H
#define MEMBER_H

#include <string>

class Member {
private:
    std::string id;
    std::string name;
    std::string email;

public:
    Member(const std::string& id,
           const std::string& name,
           const std::string& email);

    std::string getId() const;
    std::string getName() const;
    std::string getEmail() const;

    void setName(const std::string& name);
    void setEmail(const std::string& email);
};

#endif
