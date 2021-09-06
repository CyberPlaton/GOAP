#pragma once

#include <string>
#include <iostream>
#include <any>

class Any
{
public:
	Any(std::any value, const std::string& type)
	{
		this->value = value;
		this->type = type;
	}


	std::string getType() { return type; }


	/*
	* Get the value of data.
	* Value is not changed.
	*/
	template < typename T >
	bool getValue(const std::string& type, T* data_holder)
	{
		if (this->type.compare(type) != 0)
		{
			return false;
		}
		else
		{
			// Store value in specified storage.
			data_holder = std::any_cast<T>(value);
			return true;
		}
	}

	/*
	* Change the stored value.
	*/
	template < typename T >
	void setValue(T value, const std::string& type)
	{
		this->value = std::any(value);
		this->type = type;
	}


private:

	std::any value;
	std::string type;
};