#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

namespace LCNUtilities
{
	class ConfigManager
	{
		class Parameter
		{
		private:
			string value;

		public:
			Parameter();

			Parameter(const string &astr);

			template<class T>
			T Value() const;

			template<class T>
			void Value(const T &val);
		};

	private:
		unordered_map<string, Parameter> configmap;

		ConfigManager();

		~ConfigManager();

	public:
		ConfigManager(const ConfigManager&) = delete;
		ConfigManager& operator=(ConfigManager&) = delete;

		static ConfigManager& AppSettings() noexcept;

		Parameter& operator[](const string& key);
	};

	template<class T>
	inline T ConfigManager::Parameter::Value() const
	{
		T result;
		stringstream ss;

		if (value.empty()) throw exception("This parameter is empty.");

		ss << value;
		ss >> result;

		if (ss.fail()) throw exception("Failed to convert parameter to required type.");

		return result;
	}

	template<class T>
	inline void ConfigManager::Parameter::Value(const T &val)
	{
		stringstream ss;

		ss << val;
		ss >> value;
	}
}

#endif