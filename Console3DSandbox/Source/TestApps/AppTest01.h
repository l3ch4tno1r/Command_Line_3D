#pragma once

#include "Source/Core/Application.h"
#include "Source/Core/Console.h"

namespace LCN
{
	class AppTest : public Application<AppTest>
	{
	private:
		using Base = Application<AppTest>;
		friend class Base;

		void Run();
	};
}

typedef LCN::AppTest LCNApp;