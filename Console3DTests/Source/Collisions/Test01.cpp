#include <iostream>

#include <LCN_Collisions/Source/Shapes/Line.h>
#include <LCN_Collisions/Source/Shapes/AABB.h>
#include <LCN_Collisions/Source/Shapes/Point.h>

#include <LCN_Collisions/Source/Collisions/CollisionFunctions.h>

using namespace LCN;

#define LOG(X) std::cout << #X << " = " << X << std::endl;

int main()
{
	LCN::Line2Df line(
		{ 1, 2, 1 },
		{ 1, 1, 0 });

	LCN::AABB2Df aabb1(
		{ 0, 3, 1 },
		4, 3);

	LCN::Point2Df pt1 = {  1,  1, 1 };
	LCN::Point2Df pt2 = { -1,  1, 1 };
	LCN::Point2Df pt3 = { -1,  4, 1 };
	LCN::Point2Df pt4 = {  1,  4, 1 };
	LCN::Point2Df pt5 = {  5,  4, 1 };
	LCN::Point2Df pt6 = {  5,  2, 1 };
	LCN::Point2Df pt7 = {  5, -1, 1 };
	LCN::Point2Df pt8 = {  1, -1, 1 };
	LCN::Point2Df pt9 = { -1, -1, 1 };

	LOG(DetectCollision(aabb1, pt1));
	LOG(DetectCollision(aabb1, pt2));
	LOG(DetectCollision(aabb1, pt3));
	LOG(DetectCollision(aabb1, pt4));
	LOG(DetectCollision(aabb1, pt5));
	LOG(DetectCollision(aabb1, pt6));
	LOG(DetectCollision(aabb1, pt7));
	LOG(DetectCollision(aabb1, pt8));
	LOG(DetectCollision(aabb1, pt9));

	LCN::AABB2Df aabb2 = {
		{ 2, 4, 1 },
		4, 2
	};

	LCN::AABB2Df aabb3 = {
		{ 5, 1, 1 },
		4, 3
	};

	LOG(DetectCollision(aabb1, aabb2));
	LOG(DetectCollision(aabb1, aabb3));
	LOG(DetectCollision(aabb2, aabb3));

	std::cin.get();
}