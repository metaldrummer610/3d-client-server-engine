#ifndef _FILE_LOADER_H
#define _FILE_LOADER_H

#include <string>
#include <map>

class FileLoader{
public:
	std::map<std::string, std::string> loadConfigFile();
};

#endif
