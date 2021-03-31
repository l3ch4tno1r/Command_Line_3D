#include <iostream>

#include <LCN_Collisions/Source/Collisions/CollisionCore.h>

#define LOG(X) std::cout << #X << " = " << X << std::endl;

int main()
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

	std::cin.get();
}