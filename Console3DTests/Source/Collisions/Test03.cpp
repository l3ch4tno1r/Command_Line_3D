#include <iostream>

#include <LCN_Collisions/Source/Collisions/CollisionCore.h>

#define LOG(X) std::cout << #X << " = " << X << std::endl;
#define SEPARATOR(X) std::cout << "------------ " << #X << " ------------" << std::endl;

int main()
{
	SEPARATOR(1)
	{
		LCN::CollisionDetect collision;
		
		LCN::Point2Df pt1 = {  1, 1, 1 };
		LCN::Point2Df pt2 = { -1, 1, 1 };
		
		LCN::AABB2Df aabb1(
			{ 0, 3, 1 },
			4, 3);
		
		LOG(collision(pt1, aabb1));
		LOG(collision(pt2, aabb1));
		LOG(collision(aabb1, pt1));
		LOG(collision(aabb1, pt2));
	}

	SEPARATOR(2)
	{
		LCN::CollisionDetect collision;
		
		LCN::Line3Df line = {
			{ 0, 0, 0 },
			{ 0, 1, 1 }
		};
		
		LCN::Plane3Df plane = {
			{ 3, 0, 0, 1 },
			{ 1, 0, 0, 0 }
		};
		
		LOG((plane.Normal() | line.Direction()));
		
		auto result1 = collision(plane, line);
		auto result2 = collision(line, plane);
	}

	SEPARATOR(3)
	{
		LCN::Line3Df line = {
			{ 0, 0, 0 },
			{ 1, 1, 1 }
		};

		LCN::Plane3Df plane = {
			{ 3, 0, 0, 1 },
			{ 1, 0, 0, 0 }
		};

		//auto result1 = ComputeCollision(line, plane);
		//
		//std::cout << result1.Result() << std::endl;
	}

	SEPARATOR(4)
	{
		LCN::Line3Df lines[] = {
			LCN::Line3Df({ 0, 0, 0 }, { 1, 1, 1 }),
			LCN::Line3Df({ 0, 0, 0 }, { 0, 1, 1 }),
			LCN::Line3Df({ 0, 0, 0 }, { 1, 0, 1 }),
			LCN::Line3Df({ 0, 0, 0 }, { 1, 1, 0 }),
		};

		LCN::Plane3Df plane = {
			{ 3, 0, 0, 1 },
			{ 1, 0, 0, 0 }
		};

		for (const auto& line : lines)
		{
			LCN::PlaneVSLine3Df result;

			ComputeCollision(line, plane, result);
			ComputeCollision(plane, line, result);

			if (!result)
			{
				std::cout << "No collision" << std::endl;

				continue;
			}

			std::cout << result.Result() << std::endl;
		}
	}

	SEPARATOR(Circle)
	{
		LCN::Circle<float> circle { { 3, 2 }, 3 };

		LCN::Line2Df lines[] = {
			{ {0, 0}, {1,  1} }, // Yes
			{ {6, 0}, {1,  1} }, // No
			{ {0, 0}, {1,  0} }, // Yes
			{ {7, 0}, {0,  1} }, // No
			{ {0, 4}, {1, -1} }, // Yes
		};

		for (const auto& line : lines)
		{
			LCN::CircleVSLine2Df result;

			LCN::ComputeCollision(circle, line, result);

			std::cout << "-------------" << std::endl;

			LOG(result);

			if (!result)
				continue;

			for (const auto& vect : result)
				std::cout << vect << std::endl;
		}
	}

	SEPARATOR(Sphere)
	{
		LCN::Sphere3Df sphere { { 1, 2, 2 }, 3 };
		LCN::Line3Df   line   { { 0, 0, 0 }, { 1, 1, 1 } };

		LCN::SphereVSLine3Df result;

		LCN::ComputeCollision(sphere, line, result);

		if (result)
			for (const auto& vec : result)
				std::cout << vec << std::endl;
	}

	SEPARATOR(AABB)
	{
		LCN::AABB2Df aabb1{ { 0, 3 }, 4, 3 };
		LCN::AABB2Df aabb2{ { 2, 4 }, 4, 2 };

		LCN::AABBVSAABB<float, 2> result;

		// Expectation : { { 2, 3 }, 2, 1 }
		LCN::ComputeCollision(aabb1, aabb2, result);

		std::cout << result.Result().TopLeft() << std::endl;

		LOG(result.Result().Width());
		LOG(result.Result().Height());
	}

	std::cin.get();
}