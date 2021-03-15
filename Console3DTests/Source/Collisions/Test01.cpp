#include <iostream>

#include <LCN_Collisions/Source/Shapes/Line.h>
#include <LCN_Collisions/Source/Shapes/AABB.h>
#include <LCN_Collisions/Source/Shapes/Point.h>

#include <LCN_Collisions/Source/Collisions/CollisionAlgorithms.h>

int main()
{
	LCN::Line2Df line(
		{ 1, 2, 1 },
		{ 1, 1, 0 });

	LCN::AABB2Df aabb(
		{ 0, 3, 1 },
		4, 3);

	LCN::Point2Df pt;

	auto result = LCN::Collision<LCN::CollisionPolicy::DetectionOnly>(aabb, pt);

	std::cin.get();
}