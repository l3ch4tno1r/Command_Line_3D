#include <iostream>

#include <LCN_Collisions/Source/Collisions/CollisionCore.h>

#define LOG(X) std::cout << #X << " = " << X << std::endl;
#define SEPARATOR(X) std::cout << "------------ " << #X << " ------------" << std::endl;

template<typename Type1, typename Type2>
struct ResultType
{
	using Type = typename ResultType<Type2, Type1>::Type;
};

template<>
struct ResultType<LCN::Hyperplane2Df, LCN::Line2Df>
{
	using Type = LCN::HyperplaneVSLine2Df;
};

int main()
{
	SEPARATOR(1)
	{
		//LCN::CollisionDetect collision;
		//
		//LCN::Point2Df pt1 = {  1, 1, 1 };
		//LCN::Point2Df pt2 = { -1, 1, 1 };
		//
		//LCN::AABB2Df aabb1(
		//	{ 0, 3, 1 },
		//	4, 3);
		//
		//LOG(collision(pt1, aabb1));
		//LOG(collision(pt2, aabb1));
		//LOG(collision(aabb1, pt1));
		//LOG(collision(aabb1, pt2));
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

	SEPARATOR(Sphere V2)
	{
		LCN::Sphere3Df sphere{ { 1, 2, 2 }, 3 };
		LCN::Line3Df   line{ { 0, 0, 0 }, { 1, 1, 1 } };

		auto result = LCN::ComputeCollision(sphere, line);

		if (result)
		{
			for (const auto& intersection : *result)
				std::cout << intersection.Point << std::endl;
		}
	}

	SEPARATOR(AABB 2D V2)
	{
		LCN::AABB2Df aabb1{ { 0,  0 }, { 4, 3 } };
		LCN::AABB2Df aabb2{ { 2,  2 }, { 6, 4 } };
		LCN::AABB2Df aabb3{ { 5, -2 }, { 9, 1 } };

		//std::cout << aabb1.Length() << std::endl; // Expect static_assert failure
		std::cout << aabb1.Width() << std::endl; // Expect 4
		std::cout << aabb1.Height() << std::endl; // Expect 3

		std::cout << LCN::DetectCollision(aabb1, aabb2) << std::endl; // Expect true
		std::cout << LCN::DetectCollision(aabb1, aabb3) << std::endl; // Expect false
		std::cout << LCN::DetectCollision(aabb2, aabb3) << std::endl; // Expect false

		// Expectation : { { 2, 2 }, { 4, 3} }
		auto result = LCN::ComputeCollision(aabb1, aabb2);

		if (result)
		{
			std::cout
				<< result->Result().Min() << '\n'
				<< result->Result().Max() << '\n';
		}
		else
			std::cout << "No collision" << std::endl;

		// Expectation : no collision
		result = LCN::ComputeCollision(aabb1, aabb3);

		if (result)
		{
			std::cout
				<< result->Result().Min() << '\n'
				<< result->Result().Max() << '\n';
		}
		else
			std::cout << "No collision" << std::endl;
	}

	SEPARATOR(Hyperplane 2D V2)
	{
		LCN::Hyperplane2Df plane2D{ {0, 0}, {0, 1} };
		LCN::Line2Df       line2D{ { 0, 1 }, { 1, 1 } };

		std::cout << LCN::DetectCollision(plane2D, line2D) << std::endl;

		auto result2D1 = LCN::ComputeCollision(line2D, plane2D);

		if(result2D1)
			std::cout << result2D1->Result() << std::endl;
	}

	SEPARATOR(AABB vs Line 2D V2)
	{
		LCN::AABB2Df aabb{ { 4, 2 }, { 10,  6 } };

		LCN::Line2Df lines[] = {
			{ { -1,  7 }, {  2, -1 } }, // Collision
			{ {  0,  4 }, {  1,  0 } }, // Collision
			{ { 14,  5 }, { -3, -1 } }, // Collision
			{ { 11,  0 }, {  0,  1 } }, // No Collision
			{ {  5, -1 }, {  1,  4 } }  // Collision
		};

		for (const auto& line : lines)
		{
			std::cout << "----------" << std::endl;

			auto result = LCN::ComputeCollision(aabb, line);

			if (!result)
			{
				std::cout << "No collision" << std::endl;

				continue;
			}

			for (const auto& inter : *result)
				std::cout << "Face #" << inter.FaceId << " :\n" << inter.Point << std::endl;
		}
	}

	std::cin.get();
}