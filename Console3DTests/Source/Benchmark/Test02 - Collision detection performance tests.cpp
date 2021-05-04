#include <iostream>

#include <LCN_Collisions/Source/Collisions/CollisionAlgorithms.h>

#include <Utilities/Source/Benchmarking.h>

#define SEPARATOR(X) std::cout << "\n------------ " << #X << " ------------" << std::endl;

const size_t iterations = 100000;

namespace LCNB = LCN::Benchmark;

void DisplaySessionDuration(const LCNB::Session& session)
{
	long long microsec = session.Duration(LCNB::Microseconds);

	std::cout << "Duration : " << microsec / 1000 << "ms" << std::endl;
	std::cout << "Speed    : " << 1000000 * (uint64_t)iterations / microsec << " iterations / s" << std::endl;
}

namespace LCN
{
	template<typename T, size_t Dim>
	void ComputeCollision2(const AABB<T, Dim>& aabb, const Line<T, Dim>& line, AABBVSLine<T, Dim>& result)
	{
		const auto& origin = line.Origin();
		const auto& direction = line.Direction();
		const auto& min = aabb.Min();
		const auto& max = aabb.Max();

		T tmaxmin = -std::numeric_limits<T>::infinity();
		T tminmax = std::numeric_limits<T>::infinity();

		for (size_t i = 0; i < Dim; ++i)
		{
			if (std::abs(direction[i]) < FUZZ_FACTOR &&
				min[i] <= origin[i] && origin[i] <= max[i])
				continue;

			T t1 = (min[i] - origin[i]) / direction[i];
			T t2 = (max[i] - origin[i]) / direction[i];

			T tmin = std::min(t1, t2);
			T tmax = std::max(t1, t2);

			tmaxmin = std::max(tmaxmin, tmin);
			tminmax = std::min(tminmax, tmax);
		}

		if (!(result.m_Collide = (tmaxmin < tminmax)))
			return;

		result.m_Intersections[0].Distance = tmaxmin;
		result.m_Intersections[1].Distance = tminmax;

		result.m_Intersections[0].Point = tmaxmin * direction + origin;
		result.m_Intersections[1].Point = tminmax * direction + origin;
	}
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

	SEPARATOR(Circle vs line 2D)
	{
		LCN::Circle2Df circle{ { 1, 2 }, 3 };
		LCN::Line2Df   line{ { 0, 0 }, { 1, 1 } };

		LCN::CircleVSLine2Df result;

		LCNB::Session session = LCNB::TimePerformance(iterations, [&]()
		{
			LCN::ComputeCollision(circle, line, result);
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

	SEPARATOR(AABB vs line 2D #2)
	{

		LCN::AABB2Df aabb{ { 4, 2 }, { 10,  6 } };
		LCN::Line2Df line{ { -1,  7 }, {  2, -1 } };

		LCN::AABBVSLine2Df result;

		LCNB::Session session = LCNB::TimePerformance(iterations, [&]()
			{
				LCN::ComputeCollision2(aabb, line, result);
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