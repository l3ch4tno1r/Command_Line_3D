#include <iostream>

#include <LCN_Collisions/Source/Collisions/CollisionCore.h>

#define LOG(X) std::cout << #X << " = " << X << std::endl;
#define SEPARATOR(X) std::cout << "------------ " << #X << " ------------" << std::endl;

int main()
{
	SEPARATOR(1);
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
			{ 0, 0, 0, 1 },
			{ 0, 1, 1, 0 }
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
			{ 0, 0, 0, 1 },
			{ 1, 1, 1, 0 }
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
			LCN::Line3Df({ 0, 0, 0, 1, }, { 1, 1, 1, 0 }),
			LCN::Line3Df({ 0, 0, 0, 1, }, { 0, 1, 1, 0 }),
			LCN::Line3Df({ 0, 0, 0, 1, }, { 1, 0, 1, 0 }),
			LCN::Line3Df({ 0, 0, 0, 1, }, { 1, 1, 0, 0 }),
		};

		LCN::Plane3Df plane = {
			{ 3, 0, 0, 1 },
			{ 1, 0, 0, 0 }
		};

		for (const auto& line : lines)
		{
			LCN::PlaneVSLineCollision3f result;

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

	std::cin.get();
}