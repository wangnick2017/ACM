#include <iostream>
#include <cstdio>
#include "User.h"
#include "shell.h"
#include "pair.h"
int main()
{
	stl::string<10> a;
	{
		stl::string<10> b("gzp");
		a = b;
	}
	system("pause");
	return 0;
}