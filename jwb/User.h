#ifndef USER_HPP
#define USER_HPP
#include "string.h"
#include "vector.h"
class User
{
public:
	stl::string<20> ID, name, password, email, phone;
	int privilege;
	User()
	{
		privilege = 0;
	}
	User(const User &other)
	{
		ID = other.ID;
		name = other.name;
		password = other.password;
		email = other.email;
		phone = other.phone;
		privilege = other.privilege;
	}
	User operator=(const User &other)
	{
		ID = other.ID;
		name = other.name;
		password = other.password;
		email = other.email;
		phone = other.phone;
		privilege = other.privilege;
	}
};
#endif