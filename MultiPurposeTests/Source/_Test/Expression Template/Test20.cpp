#include <iostream>
#include <cstring>

class lcnString
{
private:
	char*  m_Str;
	size_t m_Size;

public:
	lcnString() :
		m_Str(nullptr),
		m_Size(0)
	{}

	lcnString(const char* str)
	{
		m_Size = strlen(str);
		m_Str  = new char[m_Size + 1];

		memcpy(m_Str, str, m_Size);

		m_Str[m_Size] = '\0';
	}

	~lcnString()
	{
		delete[] m_Str;
	}

	const char* c_str() const
	{
		return m_Str;
	}

	size_t Size() const
	{
		return m_Size;
	}

	lcnString& operator+=(const lcnString& other)
	{
		char* temp = m_Str;

		m_Str = new char[m_Size + other.m_Size + 1];

		memcpy(m_Str, temp, m_Size);
		memcpy(m_Str + m_Size, other.m_Str, other.m_Size);

		m_Size += other.m_Size;

		m_Str[m_Size] = '\0';

		delete[] temp;

		return (*this);
	}
};

std::ostream& operator<<(std::ostream& stream, const lcnString& str)
{
	const char* ptr = str.c_str();

	while (*ptr != '\0')
		stream << *(ptr++);

	return stream;
}

int main()
{
	lcnString str = "Hello world !";

	str += " How are you ?";

	std::cout << str << std::endl;

	std::cin.get();
}