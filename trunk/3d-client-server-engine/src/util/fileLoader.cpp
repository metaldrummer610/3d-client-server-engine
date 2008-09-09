#include "fileLoader.h"
#include <fstream>
#include <iostream>
using namespace std;

std::map<std::string, std::string> FileLoader::loadConfigFile() {
	std::map<std::string, std::string> properties;

	std::fstream configFile;

	std::string line;

	configFile.open("resources/config.config");

	if (configFile.is_open()) {
		while (!configFile.eof()) {
			getline(configFile, line);

			int pos = line.find("=");

			std::string a = line.substr(0, pos);
			std::string b = line.substr(pos + 1);

			properties.insert(std::pair<std::string, std::string>(a, b));
		}
	} else {
		std::cout << "could not open file" << std::endl;
	}

	configFile.close();

	return properties;
}
