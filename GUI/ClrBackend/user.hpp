#ifndef USER_HPP
#define USER_HPP
#include "string.hpp"

class User {
public:
        tstring<20> password;
        tstring<19> email;
        tstring<11> phone;
        tstring<12> name;
        char privilege;
        unsigned int id;
};

#endif
