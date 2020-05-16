#include <iostream>
#include <list>

class ISubject;

class IObserver
{
private:
	ISubject* subject;

	static uint32_t count;

protected:
	uint32_t id;

public:
	IObserver() :
		subject(nullptr),
		id(++count)
	{}

	virtual ~IObserver();

	virtual void Update() = 0;

	void AttachSubject(ISubject* _subject)
	{
		subject = _subject;
	}
};

uint32_t IObserver::count = 0;

class ISubject
{
private:
	std::list<IObserver*> obslist;

public:
	void AddObserver(IObserver* obs)
	{
		obs->AttachSubject(this);

		obslist.push_back(obs);
	}

	void RemoveObserver(IObserver* obs)
	{
		auto it = std::find(obslist.begin(), obslist.end(), obs);

		obslist.erase(it);
	}

	void Notify()
	{
		for (auto& obs : obslist)
			obs->Update();
	}
};

IObserver::~IObserver()
{
	if (subject != nullptr)
		subject->RemoveObserver(this);
}

////////////////////////
//-- Implementation --//
////////////////////////

class ObserverA : public IObserver
{
public:
	void Update()
	{
		std::cout << "ObserverA_ " << id << " updated" << std::endl;
	}
};

class ObserverB : public IObserver
{
public:
	void Update()
	{
		std::cout << "Observer_B " << id << " updated" << std::endl;
	}
};

int main()
{
	ISubject subject;
	ObserverA oA1;
	ObserverB oB1;

	subject.AddObserver(&oA1);
	subject.AddObserver(&oB1);

	subject.Notify();

	std::cin.get();
}

