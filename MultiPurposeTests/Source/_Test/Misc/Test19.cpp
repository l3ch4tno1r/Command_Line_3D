#include <iostream>
#include <memory>
#include <string>

class IVoiture
{
public:
	virtual double prix() = 0;
	virtual std::string description() = 0;
};

class Aston_martin : public IVoiture
{
public:
	double prix() override
	{
		return 999.99l; 
	}

	std::string description() override
	{
		return std::string("Aston Martin");
	}
};

class Option_voiture : public IVoiture
{
protected:
	std::unique_ptr<IVoiture> voiture_;
	double prix_option_;
	std::string description_option_;

public:
	Option_voiture(std::unique_ptr<IVoiture> voiture, double prix_option, std::string&& description_option) :
		voiture_(std::move(voiture)),
		prix_option_(prix_option),
		description_option_(std::move(description_option))
	{}

	double prix() override 
	{
		return voiture_->prix() + prix_option_;
	}
	
	std::string description() override
	{
		return voiture_->description() + "\n - " + description_option_;
	}
};

class Option_clim : public Option_voiture
{
public:
	Option_clim(std::unique_ptr<IVoiture> voiture) :
		Option_voiture(std::move(voiture), 1.0, "Climatisation")
	{}
};

class Option_parachute : public Option_voiture
{
public:
	Option_parachute(std::unique_ptr<IVoiture> voiture) :
		Option_voiture(std::move(voiture), 10.0, "Parachute")
	{}
};

class Option_amphibie : public Option_voiture
{
public:
	Option_amphibie(std::unique_ptr<IVoiture> voiture) :
		Option_voiture(std::move(voiture), 100.0, "Amphibie")
	{}
};

int main()
{
	auto voiture = std::unique_ptr<IVoiture>(std::make_unique<Aston_martin>());

	voiture = std::make_unique<Option_clim>(std::move(voiture));
	voiture = std::make_unique<Option_parachute>(std::move(voiture));
	voiture = std::make_unique<Option_amphibie>(std::move(voiture));

	std::cout << voiture->description() << "\n";

	std::cin.get();

	return 0;
}