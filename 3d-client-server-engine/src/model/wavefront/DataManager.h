#ifndef DATA_MANAGER_HH
#define DATA_MANAGER_HH

#include "memory.h"
#include <map>
#include <iostream>
#include <functional>

class DataManager
{
protected:
	DataManager() {}
	virtual ~DataManager() {}

	template <typename T,typename InputData>
	static T* getDataObject(InputData data)
	{
		const char* str = T::toString(data).c_str();
		
		if( managedData.find(str) == managedData.end() )
		{
			return NULL;
		}else{
			return (T*)managedData[str];	
		}
	}

	template <typename T>
	void putDataObject(T* t)
	{
		const char* str = t->toString().c_str();
		managedData[ str ] = t;
	}
	
	static std::map<std::string, IMMObject*> managedData;
};

#endif//DATA_MANAGER_HH
