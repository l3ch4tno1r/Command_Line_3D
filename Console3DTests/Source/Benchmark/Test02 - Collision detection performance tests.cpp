#include <iostream>

#include <LCN_Collisions/Source/Collisions/CollisionAlgorithms.h>

#include <Utilities/Source/Benchmarking.h>

#define SEPARATOR(X) std::cout << "------------ " << #X << " ------------" << std::endl;

const size_t iterations = 100000;

namespace LCNB = LCN::Benchmark;

void DisplaySessionDuration(const LCNB::Session& session)
{
	long long microsec = session.Duration(LCNB::Microseconds);

	std::cout << "Duration : " << microsec / 1000 << "ms" << std::endl;
	std::cout << "Speed    : " << 1000000 * (uint64_t)iterations / microsec << " iterations / s" << std::endl;
}

int main()
{

	SEPARATOR(Sphere vs line 3D)
	{
		LCNB::Session session = LCNB::TimePerformance(iterations, []()
		{
			LCN::Sphere3Df sphere { { 1, 2, 2 }, 3 };
			LCN::Line3Df   line   { { 0, 0, 0 }, { 1, 1, 1 } };

			LCN::SphereVSLine3Df result;

			LCN::ComputeCollision(sphere, line, result);
		});

		DisplaySessionDuration(session);
	}
		
	SEPARATOR(Sphere vs line 3D #2)
	{
		LCN::Sphere3Df sphere{ { 1, 2, 2 }, 3 };
		LCN::Line3Df   line{ { 0, 0, 0 }, { 1, 1, 1 } };

		LCN::SphereVSLine3Df result;

		LCNB::Session session = LCNB::TimePerformance(iterations, [&]()
		{
			LCN::ComputeCollision(sphere, line, result);
		});

		DisplaySessionDuration(session);
	}

	SEPARATOR(AABB vs line 2D)
	{
		LCN::AABB2Df aabb{ { 4, 2 }, { 10,  6 } };
		LCN::Line2Df line{ { -1,  7 }, {  2, -1 } };

		LCN::AABBVSLine2Df result;

		LCNB::Session session = LCNB::TimePerformance(iterations, [&]()
		{
			LCN::ComputeCollision(aabb, line, result);
		});

		DisplaySessionDuration(session);
	}

	SEPARATOR(Hyperplane vs line 3D)
	{
		LCN::Hyperplane3Df plane3D{ {0, 0, 0}, {0, 0, 1} };
		LCN::Line3Df       line{ { 0, 0, 1 }, { 1, 1, 1 } };

		LCN::HyperplaneVSLine3Df result;

		LCNB::Session session = LCNB::TimePerformance(iterations, [&]()
		{
			LCN::ComputeCollision(plane3D, line, result);
		});

		DisplaySessionDuration(session);
	}

	std::cin.get();
}