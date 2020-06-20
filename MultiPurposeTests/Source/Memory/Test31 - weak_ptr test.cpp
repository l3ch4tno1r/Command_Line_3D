#include <iostream>
#include <memory>
#include <string>

class Entity
{
private:
	std::string m_Name;
	std::weak_ptr<Entity> m_Friend;

public:
	explicit Entity(const char* str) :
		m_Name(str)
	{}

	~Entity()
	{
		std::cout << m_Name << ", over and out !" << std::endl;
	}

	void MakeFriend(const std::shared_ptr<Entity>& other)
	{
		m_Friend = other;
	}

	void PresentSelf() const
	{
		std::cout << "My name is " << m_Name << ".";

		if (!m_Friend.expired())
			std::cout << " My friend is " << m_Friend.lock()->m_Name << ".";

		std::cout << std::endl;
	}
};

int main()
{
	{
		std::shared_ptr<Entity> ptr1 = std::make_shared<Entity>("Matt");
		std::shared_ptr<Entity> ptr2 = std::make_shared<Entity>("Don");

		ptr1->MakeFriend(ptr2);
		ptr2->MakeFriend(ptr1);

		ptr1->PresentSelf();
		ptr2->PresentSelf();
	}

	std::cin.get();
}