#pragma once

int main(int argc, char** argv);

class Application
{
private:
	Application() = default;
	~Application() = default;

	void Run();

	friend int ::main(int argc, char** argv);
};