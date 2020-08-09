#include <iostream>
#include <string>
#include <list>
#include <stack>
#include <memory>

#include "Utilities/ErrorHandling.h"

const int indentScale = 2;

class Directory;

class Component
{
protected:
	std::string m_Name;

public:
	Component(const char* name) :
		m_Name(name)
	{}

	virtual ~Component() {}

	virtual void ls() = 0;

	virtual void ls_r(int indent) = 0;

	const std::string& Name() const
	{
		return m_Name;
	}

	class NavCursor
	{
	private:
		std::stack<Directory*> m_Parents;
		std::list<std::unique_ptr<Component>>::iterator it;
		Component* m_Current;

	public:
		NavCursor(Component* component) :
			m_Current(component)
		{}

		Component& operator*()
		{
			return *m_Current;
		}

		Component* operator->()
		{
			return m_Current;
		}

		void GoRight();

		void GoLeft();

		void GoUp();
	};

	NavCursor GetNavCursor()
	{
		return NavCursor(this);
	}
};

class File : public Component
{
public:
	File(const char* name) :
		Component(name)
	{}

	~File()
	{}

	void ls() override
	{
		std::cout << this->m_Name << std::endl;
	}

	void ls_r(int indent = 0) override
	{
		std::string _indent(indentScale * indent, ' ');

		std::cout << _indent << "F - " << this->m_Name << std::endl;
	}
};

class Directory : public Component
{
private:
	using ComponentPtrList = std::list<std::unique_ptr<Component>>;
	ComponentPtrList m_Elements;

public:
	Directory(const char* name) :
		Component(name)
	{}

	~Directory()
	{}

	File& AddFile(const char* name)
	{
		m_Elements.push_back(std::make_unique<File>(name));

		return static_cast<File&>(*m_Elements.back());
	}

	Directory& AddDirectory(const char* name)
	{
		m_Elements.push_back(std::make_unique<Directory>(name));

		return static_cast<Directory&>(*m_Elements.back());
	}

	void ls() override
	{
		for (const auto& e : this->m_Elements)
			std::cout << e->Name() << std::endl;
	}

	void ls_r(int indent = 0) override
	{
		std::string _indent(indentScale * indent++, ' ');

		std::cout << _indent << "D - " << this->m_Name << std::endl;

		for (auto& e : this->m_Elements)
			e->ls_r(indent);
	}

	ComponentPtrList& Elements()
	{
		return m_Elements;
	}
};

void Component::NavCursor::GoRight()
{
	Directory* dir = dynamic_cast<Directory*>(m_Current);

	if (!dir)
		return;

	m_Parents.push(dir);

	it = dir->Elements().begin();

	m_Current = it->get();
}

void Component::NavCursor::GoLeft()
{
	if (m_Parents.empty())
		return;

	m_Current = m_Parents.top();

	m_Parents.pop();
}

void Component::NavCursor::GoUp()
{
	auto _it = it++;
}

int main()
{
	try
	{
		Directory root("Root");
		Directory& system = root.AddDirectory("System");
		Directory& system32 = system.AddDirectory("System32");

		system.AddFile("sys_info.txt");
		system32.AddFile("core.exe");

		Directory& log = root.AddDirectory("Log");

		log.AddFile("log20200713.csv");

		Directory& misc = root.AddDirectory("Misc");

		misc.AddFile("Le_Chat_Noir.exe");
		misc.AddFile("Console3D.cpp");

		root.ls_r();

		std::cout << "---------------------" << std::endl;

		Component::NavCursor cursor = root.GetNavCursor();

		std::string str;

		do
		{
			std::cin >> str;

			if ("name" == str)
				std::cout << cursor->Name() << std::endl;

			if ("ls" == str)
				cursor->ls();

			if ("right" == str)
				cursor.GoRight();

			if ("left" == str)
				cursor.GoLeft();

			std::cout << "---------------------" << std::endl;
		}
		while (str != "esc");
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
}