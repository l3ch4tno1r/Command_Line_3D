#include <iostream>
#include <cstring>

template<typename E>
class StrExpression
{
public:
	size_t Size() const
	{
		return static_cast<const E&>(*this).Size();
	}

	char operator[](uint32_t i) const
	{
		return static_cast<const E&>(*this)[i];
	}
};

class StrPtr : public StrExpression<StrPtr>
{
private:
	const char* m_Ptr;

public:
	StrPtr(const char* ptr) :
		m_Ptr(ptr)
	{
		std::cout << "StrPtr" << std::endl;
	}

	size_t Size() const
	{
		return strlen(m_Ptr);
	}

	char operator[](uint32_t i) const
	{
		return m_Ptr[i];
	}
};

class String : public StrExpression<String>
{
private:
	char*  m_Str;
	size_t m_Size;

public:
	String() :
		m_Str(nullptr),
		m_Size(0)
	{}

	String(const char* str)
	{
		std::cout << "char* constructor" << std::endl;

		m_Size = strlen(str);
		m_Str  = new char[m_Size + 1];

		memcpy(m_Str, str, m_Size);

		m_Str[m_Size] = '\0';
	}

	String(const String& other)
	{
		std::cout << "Copy constructor" << std::endl;

		m_Size = other.m_Size;
		m_Str  = new char[m_Size + 1];

		memcpy(m_Str, other.m_Str, m_Size);

		m_Str[m_Size] = '\0';
	}

	String(String&& other)
	{
		std::cout << "Move constructor" << std::endl;

		m_Size = other.m_Size;
		m_Str  = other.m_Str;

		other.m_Size = 0;
		other.m_Str  = nullptr;
	}

	template<typename E>
	String(const StrExpression<E>& e)
	{
		std::cout << "Template Expression constructor" << std::endl;

		m_Size = e.Size();
		m_Str  = new char[m_Size + 1];

		for (uint32_t i = 0; i < m_Size; ++i)
			m_Str[i] = e[i];

		m_Str[m_Size] = '\0';
	}

	~String()
	{
		if(m_Str != nullptr)
			delete[] m_Str;
	}

	const char* c_str() const
	{
		return m_Str;
	}
	
	String& operator+=(const String& other)
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

	size_t Size() const
	{
		return m_Size;
	}

	char& operator[](uint32_t i)
	{
		return m_Str[i];
	}

	char operator[](uint32_t i) const
	{
		return m_Str[i];
	}
};

/////////////////////////////
//-------------------------//
/////////////////////////////
template<typename EL, typename ER>
class StrConcat : public StrExpression<StrConcat<EL, ER>>
{
private:
	const EL& el;
	const ER& er;

public:
	StrConcat(const EL& el, const ER& er) :
		el(el),
		er(er)
	{}

	size_t Size() const
	{
		return el.Size() + er.Size();
	}

	char operator[](uint32_t i) const
	{
		if (i < el.Size())
			return el[i];
		else
			return er[i - el.Size()];
	}
};

template<typename EL, typename ER>
StrConcat<EL, ER> operator+(const StrExpression<EL>& el, const StrExpression<ER>& er)
{
	return StrConcat<EL, ER>(static_cast<const EL&>(el), static_cast<const ER&>(er));
}

/////////////////////////////
//-------------------------//
/////////////////////////////
std::ostream& operator<<(std::ostream& stream, const String& str)
{
	const char* ptr = str.c_str();

	while (*ptr != '\0')
		stream << *(ptr++);

	return stream;
}

template<typename E>
std::ostream& operator<<(std::ostream& stream, const StrExpression<E>& strexp)
{
	for (uint32_t i = 0; i < strexp.Size(); ++i)
		stream << strexp[i];

	return stream;
}

int main()
{
	String str1 = "Hello world !";
	String str2 = " How are you ?";
	String str3 = " My name is John.";

	std::cout << "-----------------" << std::endl;

	auto str4 = str1 + " Test";

	std::cout << str4 << std::endl;

	std::cin.get();
}