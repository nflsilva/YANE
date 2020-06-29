#pragma once
#include "yane.hpp"


class i_test
{
public:
	std::string _name;
	
public:

	virtual bool run_test() = 0;
	std::string get_name(){
		return _name;
	}
};