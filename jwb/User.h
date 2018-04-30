#ifndef USER_HPP
#define USER_HPP
#include "string.h"
#include "vector.h"
class User
{
private:
	stl::string<20> ID, name, password, email, phone;
	int privilege;
public:
	User()
	{
		privilege = 0;
	}
	void setID(const stl::string<20> &s)
	{
		ID = s;
	}
	void setName(const stl::string<20> &s)
	{
		name = s;
	}
	void setPassword(const stl::string<20> &s)
	{
		password = s;
	}
	void setEmail(const stl::string<20> &s)
	{
		email = s;
	}
	void setPhone(const stl::string<20> &s)
	{
		phone = s;
	}
	void setPrivilege(const int &x)
	{
		privilege = x;
	}
};
#endif