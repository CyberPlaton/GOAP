#pragma once

#include "BTNodes.h"
#include "Any.h"

#include <list>
#include <map>
#include <mutex>


/*
* The blackboard stores entries with custom values,
* which can change dynamically.
*/
class BTBlackboard
{
public:
	BTBlackboard(std::string name) : m_Name(name) {}

	template < typename T >
	void set(std::string name, T value, const std::string& type)
	{
		std::scoped_lock lock(*m_Mutex);

		m_Data[name].setValue(value, type);
	}


	/*
	*/
	template < typename T >
	T& getData(const std::string& name, const std::string& type)
	{
		std::scoped_lock lock(*m_Mutex);

		// Check whether the data exists...
		for (auto& it : m_Data)
		{
			if (it.first.compare(name) == 0)
			{
				T& d = *new T();
				if (it.second.getValue(type, d))
				{
					return d;
				}
			}
		}

		return nullptr;
	}



private:

	std::string m_Name;

	std::map<std::string, Any> m_Data;

	mutable std::mutex* m_Mutex = new std::mutex();
};