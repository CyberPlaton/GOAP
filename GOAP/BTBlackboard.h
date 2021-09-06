#pragma once

#include "BTCommon.h"
#include "BTNodes.h"
#include "Any.h"


/*
* The blackboard stores entries with custom values,
* which can change dynamically.
*/
class BTBlackboard
{
public:
	BTBlackboard(std::string name) : m_Name(name) {}
	
	
	~BTBlackboard()
	{
		/*
		* Blackboard is responsible for freeing memory used by its data storage.
		*/
		while (!m_Data.empty())
		{
			m_Data.erase(m_Data.begin());
		}

		m_Data.clear();
		m_Name.clear();
		delete m_Mutex;
	}

	template < typename T >
	void set(std::string name, T value, const std::string& type)
	{
		std::scoped_lock lock(*m_Mutex);

		// Does the entry already exist
		if (m_Data.find(name) != m_Data.end())
		{
			// Change entry
			m_Data.at(name).set<T>(value, type);
		}
		else
		{
			// Create new entry
			Any a(value, name);
			m_Data.emplace(name, a);
		}

	}


	/*
	* Return entrydata with given name.
	* 
	* If the entry does not exist, nothing is returned, thus using it may
	* cause undefined behavior.
	*/
	template < typename T >
	T getData(const std::string& name)
	{
		std::scoped_lock lock(*m_Mutex);

		// Check whether the data exists...
		for (auto& it : m_Data)
		{
			if (it.first.compare(name) == 0)
			{
				return it.second.as<T>();
			}
		}
	}


private:

	std::string m_Name;

	std::map<std::string, Any> m_Data;

	mutable std::mutex* m_Mutex = new std::mutex();
};