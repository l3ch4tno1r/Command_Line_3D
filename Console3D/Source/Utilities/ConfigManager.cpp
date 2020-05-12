#include <iostream>
#include <fstream>
#include <sstream>

#include "ConfigManager.h"

namespace LCNUtilities
{
	// Construcor / Destructor
	ConfigManager::ConfigManager()
	{
		ifstream configfile("config.txt", ios::in);

		if (configfile)
		{
			string line;

			while (getline(configfile, line))
			{
				if (line.size() != 0)
				{
					stringstream ssdata(line);
					string key, value;

					ssdata >> key >> value;

					configmap[key].Value(value);
				}
			}

			configfile.close();
		}
	}

	ConfigManager::~ConfigManager()
	{}

	// Accessors

	ConfigManager& ConfigManager::AppSettings() noexcept
	{
		return appsettings;
	}

	ConfigManager::Parameter& ConfigManager::operator[](const string& key)
	{
		return configmap[key];
	}

	// Instanciation

	ConfigManager ConfigManager::appsettings;

	ConfigManager::Parameter::Parameter() : value()
	{}

	ConfigManager::Parameter::Parameter(const string &astr) : value(astr)
	{}
}
