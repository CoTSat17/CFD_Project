#include <point.h>




Point point_create(float x, float y, float z) {
	Point point;
	point.x = x;
	point.y = y;
	point.z = z;

	return point;
}



float point_dist(Point node_1, Point node_2) {


	float dist = sqrtf(powf(node_1.x - node_2.x, 2)
		+ powf(node_1.y - node_2.y, 2)
		+ powf(node_1.z - node_2.z, 2));

	return dist;
}


